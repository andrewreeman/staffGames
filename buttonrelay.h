#ifndef BUTTONRELAY_H
#define BUTTONRELAY_H

#include <QObject>
#include <QString>
#include <QPushButton>

class ButtonRelay : public QObject
{
    Q_OBJECT
public:
    explicit ButtonRelay(QPushButton*, int, QObject *parent = 0);

signals:
    void buttonClicked(int);
public slots:
    void clicked();
private:
    int m_userIndex;
    QPushButton* m_button;
};

#endif // BUTTONRELAY_H
