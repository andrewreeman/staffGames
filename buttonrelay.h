#ifndef BUTTONRELAY_H
#define BUTTONRELAY_H

#include <QObject>
#include <QString>
#include <QPushButton>

class ButtonRelay : public QObject
{
    Q_OBJECT
public:
    explicit ButtonRelay(QPushButton*, QString, QObject *parent = 0);

signals:
    void buttonClicked(QString);
public slots:
    void clicked();
private:
    QString m_userName;
    QPushButton* m_button;
};

#endif // BUTTONRELAY_H
