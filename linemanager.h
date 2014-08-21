#ifndef LINEMANAGER_H
#define LINEMANAGER_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QList>
#include <QPen>

#include "staffline.h"
#include "ledgerline.h"


class lineManager : public QGraphicsLineItem
{    
public:    
    lineManager();

    virtual QRectF boundingRect()const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
        Q_UNUSED(painter);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    void setCorrectState(int lineNumber, bool isCorrect);
    void unsetCorrectState(int lineNumber);
private:    
    void setPens();
    void createStaff();
        QList<QRectF> createLineTemplates();
        void offsetRects(QList<QRectF> &recs, int offset);
    void createLedgerLines();
        void createUpperLedgers();
        void createLowerLedgers();

    void circleCollision(QGraphicsItem* circle);
        QList<int> getCollidedLineNumbers(QList<QGraphicsItem*>* collidedItems);
        void selectLine(int newSelectedLine);        
        void setSelectLine(int selectedLine, bool selectOn);
        void unsetSelectLine(int selectedLine);
        void updateUpperLedgers(QPointF circleCentre);
        void updateLowerLedgers(QPointF circleCentre);
    int lineNumberToIndex(int lineNumber);
        bool isOdd(int);    

    int m_numBlackLedgerLines;
    int m_numBlackStaffLines;
    int m_selectedLine;      
    QList<StaffLine*> m_staffLines;
    QList<LedgerLine*> m_upperLines;
    QList<LedgerLine*> m_lowerLines;
    QBrush m_blackLineBrush;
    QBrush m_whiteLineBrush;
};

#endif // LINEMANAGER_H