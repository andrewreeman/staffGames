#ifndef BUTTONRELAY_H
#define BUTTONRELAY_H

#include <QObject>
#include <QVariant>
#include <QPushButton>


class ButtonRelay : public QObject
{
    Q_OBJECT
public:
    explicit ButtonRelay(QPushButton*, QVariant, QObject *parent = 0);

signals:
    void buttonClicked(QVariant);
public slots:
    void clicked();
private:
    QPushButton* m_button;
    QVariant m_message;

};

#endif // BUTTONRELAY_H
