#include "linemanager.h"
#include "projectConstants.hh"
#include "note.h"
#include "staffscene.h"

#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QWidget>
#include <cmath>
#include <QGraphicsScene>



lineManager::lineManager() :
    m_numBlackLedgerLines(3), m_numBlackStaffLines(5), m_selectedLine(0)
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

    int Yoffset = projectConstants::blackWidth + projectConstants::whiteWidth;
    QPen pen;
    pen.setStyle(Qt::NoPen);

    for(int i=0; i<m_numBlackStaffLines*2; i+=2){
        for(int parity=0; parity<2; ++parity){
            int line = i+parity;
            m_staffLines.push_back(new StaffLine(lineTemplates.at(parity), this) );
            m_staffLines.at(line)->setDefaultBrush( brushes.at(parity) );
            m_staffLines.at(line)->setPen(pen);
            m_staffLines.at(line)->setData(projectConstants::keyType, projectConstants::typeStaffLine);
            m_staffLines.at(line)->setData(projectConstants::keyLineNumber, line);            
        }
        offsetRects(lineTemplates, Yoffset);
    }
}

QList<QRectF> lineManager::createLineTemplates()
{
    QRectF blackLine(0, 0, projectConstants::staffLineLength, projectConstants::blackWidth);
    QRectF whiteLine(0, projectConstants::blackWidth, projectConstants::staffLineLength, projectConstants::whiteWidth);
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
    int Yoffset = -(projectConstants::blackWidth + projectConstants::whiteWidth);
    QPen pen;

    offsetRects(lineTemplates, Yoffset);
    pen.setStyle(Qt::NoPen);
    for(int i=0; i<m_numBlackLedgerLines*2; i+=2){
        for(int parity=0; parity<2; ++parity){
            int line = i+parity;
            int lineNumber = -( i+(2-parity) );
            m_upperLines.push_back(new LedgerLine(lineTemplates.at(parity), this) );
            m_upperLines.at(line)->setDefaultBrush( brushes.at(parity) );
            m_upperLines.at(line)->setPen(pen);
            m_upperLines.at(line)->setData(projectConstants::keyType, projectConstants::typeStaffLine);
            m_upperLines.at(line)->setData(projectConstants::keyLineNumber,lineNumber);
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
    int Yoffset = projectConstants::blackWidth + projectConstants::whiteWidth;
    QPen pen;

    for(int i=0; i<m_numBlackStaffLines; ++i){   // mmm magic number...
        offsetRects(lineTemplates, Yoffset);
    }
    pen.setStyle(Qt::NoPen);
    for(int i=0; i<m_numBlackLedgerLines*2; i+=2){
        for(int parity=0; parity<2; ++parity){
            int line = i+parity;
            int lineNumber = line+m_staffLines.size();
            m_lowerLines.push_back(new LedgerLine(lineTemplates.at(parity), this) );
            m_lowerLines.at(line)->setDefaultBrush( brushes.at(parity) );
            m_lowerLines.at(line)->setPen(pen);
            m_lowerLines.at(line)->setData(projectConstants::keyType, projectConstants::typeStaffLine);
            m_lowerLines.at(line)->setData(projectConstants::keyLineNumber,lineNumber);
            m_lowerLines.at(line)->setOpacity(0);
        }
        offsetRects(lineTemplates, Yoffset);
    }
}

QRectF lineManager::boundingRect() const
{    
    //WARNING bounding rect not accurate
    return QRectF(0, 0, 0, 0);
}

bool lineManager::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    bool isEventConsumed = false;
    QString type = watched->data(projectConstants::keyType).toString();

    if(type == projectConstants::typeNote){
        if(event->type() == QEvent::GraphicsSceneMouseMove){
            QPointF circleCentre = watched->boundingRect().center();

            circleCollision(watched);
            circleCentre = mapFromItem(watched, circleCentre);
            updateUpperLedgers(circleCentre);
            updateLowerLedgers(circleCentre);
            return isEventConsumed;
        }
        else if(event->type() == QEvent::GraphicsSceneMouseRelease){
            StaffScene* thisScene = qobject_cast<StaffScene*>(scene());
            if(thisScene){
                circleCollision(watched);
                thisScene->selectLine(m_selectedLine);
            }
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
        type = item->data(projectConstants::keyType);
        if(type == projectConstants::typeStaffLine){
            lineNumber = item->data(projectConstants::keyLineNumber).toInt();
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
    if(selectedLine <0){
        m_upperLines.at( lineNumberToIndex(selectedLine ))->setSelected(select);
    }
    else if(selectedLine >= m_staffLines.size())
        m_lowerLines.at( lineNumberToIndex(selectedLine) )->setSelected(select);
    else
        m_staffLines.at( lineNumberToIndex(selectedLine) )->setSelected(select);
}

void lineManager::updateUpperLedgers(QPointF circleCentre)
{
    int startSelection = m_selectedLine > 0 ? 0 : m_selectedLine;
    for(int i=startSelection; i<0; ++i){
        int index = lineNumberToIndex(i);
        LedgerLine* line = m_upperLines.at(index);
        QPointF circleCentre_relTo_line = mapToItem(line, circleCentre);
        line->setCentreX(circleCentre_relTo_line.x());
        line->setOpacity(1);
    }

    for(int i=startSelection-1; i>=-(m_upperLines.size()); --i){
        int index = lineNumberToIndex(i);
        LedgerLine* line = m_upperLines.at(index);        
        line->setOpacity(0);
    }
}

void lineManager::updateLowerLedgers(QPointF circleCentre)
{
    int startSelection;

    if(m_selectedLine < m_staffLines.size())
        startSelection = -1;
    else
        startSelection = lineNumberToIndex(m_selectedLine);

    for(int i=0; i<startSelection+1; ++i){
        LedgerLine* line = m_lowerLines.at(i);
        QPointF circleCentre_relTo_line = mapToItem(line, circleCentre);
        line->setCentreX(circleCentre_relTo_line.x());
        line->setOpacity(1);
    }
    for(int i=startSelection+1; i<m_lowerLines.size(); ++i){
        LedgerLine* line = m_lowerLines.at(i);        
        line->setOpacity(0);
    }
}

int lineManager::lineNumberToIndex(int lineNumber)
{
    if(lineNumber<0){
        int index = -lineNumber;
        if(isOdd(index))
            return index;
       else
            return index-2;
    }
    if(lineNumber >= m_staffLines.size()){
        return lineNumber-m_staffLines.size();
    }
    else{
        return lineNumber;
    }
}

bool lineManager::isOdd(int number)
{
    return number%2;
}

void lineManager::setCorrectState(int lineNumber, bool isCorrect)
{
    if(lineNumber < 0){
        m_upperLines.at( lineNumberToIndex(lineNumber) )->setCorrectState(isCorrect);
    }
    else if(lineNumber >= m_staffLines.size()){
        m_lowerLines.at( lineNumberToIndex(lineNumber) )->setCorrectState(isCorrect);
    }
    else{
        m_staffLines.at( lineNumberToIndex(lineNumber) )->setCorrectState(isCorrect);
    }
}

void lineManager::unsetCorrectState(int lineNumber)
{
    if(lineNumber < 0){
        m_upperLines.at( lineNumberToIndex(lineNumber) )->unsetCorrectState();
    }
    else if(lineNumber >= m_staffLines.size()){
        m_lowerLines.at( lineNumberToIndex(lineNumber) )->unsetCorrectState();
    }
    else{
        m_staffLines.at( lineNumberToIndex(lineNumber) )->unsetCorrectState();
    }

}
