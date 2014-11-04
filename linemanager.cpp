#include "linemanager.h"
#include "staffGamesConstants.h"
#include "note.h"
#include "staffscene.h"

#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QWidget>
#include <cmath>
#include <QGraphicsScene>

lineManager::lineManager() :
    m_selectedLine(0)
{
    setPens();
    createStaff();
    createLedgerLines();
    m_staffLines.at(m_selectedLine)->setSelected(true);
}

void lineManager::setPens()
{
    m_blackLineBrush.setColor(Qt::black);
    m_blackLineBrush.setStyle(Qt::SolidPattern);
    m_whiteLineBrush.setColor(Qt::white);
    m_whiteLineBrush.setStyle(Qt::SolidPattern);
}

void lineManager::createStaff()
{
    QList<QBrush> brushes;
    brushes.push_back(m_blackLineBrush);
    brushes.push_back(m_whiteLineBrush);
    QList<QRectF> lineTemplates = createLineTemplates();

    int Yoffset = staffLayout::blackLineHeight + staffLayout::whiteLineHeight;
    QPen pen;
    pen.setStyle(Qt::NoPen);

    for(int i=0; i<staffLayout::numStaffLines*2; i+=2){
        for(int parity=0; parity<2; ++parity){
            int line = i+parity;
            m_staffLines.push_back(new StaffLine(lineTemplates.at(parity), this) );
            m_staffLines.at(line)->setDefaultBrush( brushes.at(parity) );
            m_staffLines.at(line)->setPen(pen);
            m_staffLines.at(line)->setData(objectPropertyKeys::type, objectPropertyTypes::lineType);
            m_staffLines.at(line)->setData(objectPropertyKeys::name, line);
        }
        offsetRects(lineTemplates, Yoffset);
    }
}

QList<QRectF> lineManager::createLineTemplates()
{
    QRectF blackLine(0, 0, staffLayout::lineLength, staffLayout::blackLineHeight);
    QRectF whiteLine(0, staffLayout::blackLineHeight, staffLayout::lineLength, staffLayout::whiteLineHeight);
    QList<QRectF> lineTemplates;    
    lineTemplates.push_back(blackLine);
    lineTemplates.push_back(whiteLine);

    return lineTemplates;
}

void lineManager::offsetRects(QList<QRectF> &recs, int Yoffset)
{
    for(int i=0; i<recs.size(); ++i){
        QRectF rec = recs.at(i);
        rec.moveTop(rec.top() + Yoffset);
        recs.replace(i, rec);
    }
}

void lineManager::createLedgerLines()
{
    createUpperLedgers();
    createLowerLedgers();
}

void lineManager::createUpperLedgers()
{
    QList<QBrush> brushes;
    brushes.push_back(m_blackLineBrush);
    brushes.push_back(m_whiteLineBrush);
    QList<QRectF> lineTemplates = createLineTemplates();
    int Yoffset = -(staffLayout::blackLineHeight + staffLayout::whiteLineHeight);
    QPen pen;

    offsetRects(lineTemplates, Yoffset);
    pen.setStyle(Qt::NoPen);
    for(int i=0; i<staffLayout::numLedgerNotes; i+=2){
        for(int parity=0; parity<2; ++parity){
            int line = i+parity;
            int lineNumber = -( i+(2-parity) );
            m_upperLines.push_back(new LedgerLine(lineTemplates.at(parity), this) );
            m_upperLines.at(line)->setDefaultBrush( brushes.at(parity) );
            m_upperLines.at(line)->setPen(pen);
            m_upperLines.at(line)->setData(objectPropertyKeys::type, objectPropertyTypes::lineType);
            m_upperLines.at(line)->setData(objectPropertyKeys::name,lineNumber);
            m_upperLines.at(line)->setOpacity(0);

        }
        offsetRects(lineTemplates, Yoffset);
    }
}

void lineManager::createLowerLedgers()
{
    QList<QBrush> brushes;
    brushes.push_back(m_blackLineBrush);
    brushes.push_back(m_whiteLineBrush);
    QList<QRectF> lineTemplates = createLineTemplates();
    int Yoffset = staffLayout::blackLineHeight + staffLayout::whiteLineHeight;
    QPen pen;

    for(int i=0; i<staffLayout::numStaffLines; ++i){
        offsetRects(lineTemplates, Yoffset);
    }
    pen.setStyle(Qt::NoPen);
    for(int i=0; i<staffLayout::numLedgerNotes; i+=2){
        for(int parity=0; parity<2; ++parity){
            int line = i+parity;
            int lineNumber = line+m_staffLines.size();
            m_lowerLines.push_back(new LedgerLine(lineTemplates.at(parity), this) );
            m_lowerLines.at(line)->setDefaultBrush( brushes.at(parity) );
            m_lowerLines.at(line)->setPen(pen);
            m_lowerLines.at(line)->setData(objectPropertyKeys::type, objectPropertyTypes::lineType);
            m_lowerLines.at(line)->setData(objectPropertyKeys::name,lineNumber);
            m_lowerLines.at(line)->setOpacity(0);
        }
        offsetRects(lineTemplates, Yoffset);
    }
}

QRectF lineManager::boundingRect() const
{    
    return QRectF(0, 0, 0, 0);
}

bool lineManager::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    bool isEventConsumed = false;
    QString type = watched->data(objectPropertyKeys::type).toString();

    auto f_checkNoteCollision = [&](){
        QPointF circleCentre = watched->boundingRect().center();

        circleCollision(watched);
        circleCentre = mapFromItem(watched, circleCentre);
        updateUpperLedgers(circleCentre);
        updateLowerLedgers(circleCentre);
    };

    auto f_selectLine = [&](){
        StaffScene* thisScene = qobject_cast<StaffScene*>(scene());
        if(thisScene){
            circleCollision(watched);
            thisScene->selectLine(m_selectedLine);
        }
    };

    if(type == objectPropertyTypes::noteType){
        if(event->type() == QEvent::GraphicsSceneMouseMove){
            f_checkNoteCollision();
            return isEventConsumed;
        }
        else if(event->type() == QEvent::GraphicsSceneMouseRelease){
            f_selectLine();
            return isEventConsumed;
        }
    }
    return isEventConsumed;
}

void lineManager::circleCollision(QGraphicsItem *circle)
{
    QList<QGraphicsItem*> items = this->scene()->collidingItems(circle);
    int numItems = items.size();
    if(numItems != 0){
        QList<int> collidedLineNumbers = getCollidedLineNumbers(&items);
        if(collidedLineNumbers.size() != 0){
#ifdef QT_DEBUG
            qDebug() << "\tSELECTED LINE NUMBERS: " << collidedLineNumbers;
#endif
            int newSelectedLine = collidedLineNumbers.at(0);
            if(newSelectedLine != m_selectedLine)
                selectLine(newSelectedLine);
        }
    }
}

QList<int> lineManager::getCollidedLineNumbers(QList<QGraphicsItem *> *collidedItems)
{
    QVariant type;
    QGraphicsItem* item;
    QList<int> lineNumbers;
    int lineNumber;
    int numItems = (*collidedItems).size();
    for(int i=0; i<numItems; ++i){
        item = (*collidedItems).at(i);
        type = item->data(objectPropertyKeys::type);
        if(type == objectPropertyTypes::lineType){
            lineNumber = item->data(objectPropertyKeys::name).toInt();
            lineNumbers.push_back(lineNumber);
        }
    }        
    return lineNumbers;
}

void lineManager::selectLine(int newSelectedLine)
{     
    setSelectLine(newSelectedLine, true);
    setSelectLine(m_selectedLine, false);
    m_selectedLine = newSelectedLine;
#ifdef QT_DEBUG
    qDebug() << newSelectedLine;
#endif
}

void lineManager::setSelectLine(int selectedLine, bool select)
{
    line(selectedLine)->setSelected(select);
}

void lineManager::updateUpperLedgers(QPointF circleCentre)
{
    int upperLineSize = m_upperLines.size();
    int selectLineBoundry = m_selectedLine > 0 ? 0 : m_selectedLine;
    auto f_turnLinesOn = [&](){
        for(int i=-1; i>=selectLineBoundry; --i){
            LedgerLine* currentLine = (LedgerLine*)line(i);
            QPointF circleCentre_relTo_line = mapToItem(currentLine, circleCentre);
            currentLine->setCentreX(circleCentre_relTo_line.x());
            currentLine->setOpacity(1);
        }
    };
    auto f_turnLinesOff = [&](){
        for(int i=selectLineBoundry-1; i>=-upperLineSize; --i){
            LedgerLine* currentLine = (LedgerLine*)line(i);
            currentLine->setOpacity(0);
        }
    };

    f_turnLinesOn();
    f_turnLinesOff();

}

void lineManager::updateLowerLedgers(QPointF circleCentre)
{   
    int lastLedgerLineNumber = m_lowerLines.last()->data(objectPropertyKeys::name).toInt();
    int selectLineBoundry = m_selectedLine >=m_staffLines.size() ? m_selectedLine : m_staffLines.size()-1;
    auto f_turnLinesOn = [&](){
        for(int i=m_staffLines.size(); i<=selectLineBoundry; ++i){
            LedgerLine* currentLine = (LedgerLine*)line(i);
            QPointF circleCentre_relTo_line = mapToItem(currentLine, circleCentre);
            currentLine->setCentreX(circleCentre_relTo_line.x());
            currentLine->setOpacity(1);
        }
    };
    auto f_turnLinesOff = [&](){
        for(int i=selectLineBoundry+1; i<=lastLedgerLineNumber; ++i){
            LedgerLine* currentLine = (LedgerLine*)line(i);
            currentLine->setOpacity(0);
        }
    };

    f_turnLinesOn();
    f_turnLinesOff();
}

StaffLine *lineManager::line(int lineNumber)
{
    if(lineNumber<0){
        int index = -lineNumber;
        if(isOdd(index))
            return m_upperLines.at(index);
       else
            return m_upperLines.at(index-2);
    }
    else if(lineNumber >= m_staffLines.size()){
        return m_lowerLines.at( lineNumber-m_staffLines.size() );
    }
    else{
        return m_staffLines.at(lineNumber);
    }
}

bool lineManager::isOdd(int number)
{
    return number%2;
}

void lineManager::setCorrectState(int lineNumber, bool isCorrect)
{
    line(lineNumber)->setCorrectState(isCorrect);
}

void lineManager::unsetCorrectState(int lineNumber)
{
    line(lineNumber)->unsetCorrectState();
}
