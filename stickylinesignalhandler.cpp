#include "stickylinesignalhandler.h"
#include <QDebug>


StickyLineSignalHandler::StickyLineSignalHandler(QObject *parent) :
    QObject(parent), m_line(nullptr)
{    
    m_lineColourTimer = new QTimer(this);
    m_lineColourTimer->setInterval(1000);
    m_lineColourTimer->setSingleShot(true);
    connect(m_lineColourTimer, SIGNAL(timeout()), SLOT(resetLineColour()));

}

void StickyLineSignalHandler::setLine(StickyLine* line)
{
    //qDebug() << "SETLINE address for line " << line->data(objectPropertyKeys::name).toString() << ": " << line;
    //m_line = nullptr;
    m_line = line;
}

void StickyLineSignalHandler::test()
{
    qDebug() << "test" << m_lineName;
}

void StickyLineSignalHandler::userResult(bool isCorrect)
{

    //qDebug() << "SETLINE address for line : " << m_line;
    //m_line->data(objectPropertyKeys::type);


    //m_line->setColour(colours::correct);




    m_lineColourTimer->start();

    if(isCorrect){
        m_line->setHighlight(colours::correct);
        m_lineColourTimer->start();
    }
    else{
        m_line->setHighlight(colours::incorrect);
        m_lineColourTimer->start();
    }
}

void StickyLineSignalHandler::resetLineColour()
{
    QString type = m_line->data(objectPropertyKeys::type).toString();

    qDebug() << m_line->data(objectPropertyKeys::name).toString();
    if(type == objectPropertyTypes::lineType){
        //m_line->setColour(Qt::black);
        m_line->setHighlight(colours::highlighted);
    }
    else{
        qDebug() << "Colour reset";
        m_line->setHighlight(colours::highlighted);
    }

}
