#ifndef LINEMANAGER_H
#define LINEMANAGER_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QList>
#include <QPen>

#include "staffline.h"
#include "ledgerline.h"


//WARNING line manager should not be a QGraphicsLineItem!
class lineManager : public QGraphicsLineItem
{    
public:    
    lineManager();

    virtual QRectF boundingRect()const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){}
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    StaffLine* getLine(int lineNumber);
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
    bool isOdd(int);

    int m_selectedLine;      
    QList<StaffLine*> m_staffLines;
    QList<LedgerLine*> m_upperLines;
    QList<LedgerLine*> m_lowerLines;
    QBrush m_blackLineBrush;
    QBrush m_whiteLineBrush;
};

#endif // LINEMANAGER_H
