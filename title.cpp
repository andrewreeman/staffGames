#include "title.h"
#include "ui_title.h"
#include "mainwindow.h"

#include <QDebug>
#include <QSettings>
#include <QInputDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

Title::Title(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Title)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

Title::~Title()
{        
    delete ui;
}

void Title::on_titleToLogin_clicked()
{    
    ui->stackedWidget->setCurrentIndex(1);
    m_allUsers = getAllUserSettings();
    makeAllUserButtons();    
    addMenu();
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(removeMenu()));
}

QList<UserSettings> Title::getAllUserSettings()
{
    QList<UserSettings> allUserSettings;
    QSettings settings;
    QStringList userNames;
    settings.beginGroup("users");
        userNames = settings.childGroups();
        for(QString user : userNames){
            settings.beginGroup(user);
            allUserSettings.push_back( UserSettings(user, settings.value("totalBeats").toInt()) );
            settings.endGroup();
        }
    settings.endGroup();
    return allUserSettings;
}

void Title::makeAllUserButtons()
{
    for(int i=0; i<m_allUsers.size(); ++i){                
        makeUserButton(i);        
    }
}

void Title::makeUserButton(int userIndex)
{
    QLayout* scrollLayout = ui->scrollAreaWidgetContents->layout();
    UserSettings user = m_allUsers.at(userIndex);
    QString name = user.getName();
    QPushButton* button = new QPushButton(name);
    ButtonRelay* buttonRelay = new ButtonRelay(button,name, this);

    m_userPushButtons.append(button);
    m_userButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QString)), this, SLOT(userButtonClicked(QString)));
}

void Title::userButtonClicked(QString user)
{    
    int userIndex = getUserIndex(user);
    m_user = m_allUsers.at(userIndex);
    ui->userName->setText( m_user.getName() );
    ui->userScore->setText( QString::number(m_user.getScore()) );
    ui->stackedWidget->setCurrentIndex(2);    
}

void Title::writeSettings()
{
    QSettings settings;
    settings.beginGroup("users");
        settings.beginGroup( m_user.getName() );
            settings.setValue("totalBeats", m_user.getScore() );
        settings.endGroup();
    settings.endGroup();
    settings.sync();
}

void Title::addMenu()
{
    QMenuBar* menuBar = ((MainWindow*)parent())->menuBar();
    QMenu* menu = new QMenu("Tools");
    QAction* action = menu->addAction("Remove user");
    menuBar->addMenu(menu);
    connect(action, SIGNAL(triggered()), this, SLOT(removeUser_clicked()));
}

void Title::removeMenu()
{
    QMenuBar* menuBar = ((MainWindow*)parent())->menuBar();
    menuBar->clear();
    disconnect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(removeMenu()));
}

void Title::removeUser_clicked()
{
    bool userClickedOk;
    QString label = "Enter a user name to remove: ";

    QString user = QInputDialog::getText(this, "Remove User", label,
                                            QLineEdit::Normal, "", &userClickedOk);
    if(!user.isEmpty() && userClickedOk){
        if(removeUser(user) == false){

        }

    }
}

bool Title::addUser(QString newUser)
{
    auto addUserToLocalSettings = [&](){
        QSettings settings;
        settings.beginGroup("users");
            settings.beginGroup( m_allUsers.last().getName() );
                settings.setValue( "totalBeats", m_allUsers.last().getScore() );
            settings.endGroup();
        settings.endGroup();
    };    
    if(isUserExist(newUser)){
        return false;
    }
    m_allUsers.push_back(UserSettings(newUser, 0));
    addUserToLocalSettings();
    makeUserButton(m_allUsers.size()-1);
    return true;
}

bool Title::removeUser(QString user)
{    
    auto removeFromStoredUsers = [&](){
        QSettings settings;
        settings.beginGroup("users");
            settings.beginGroup(user);
                settings.remove("");
            settings.endGroup();
        settings.endGroup();
    };
    //TODO drop down list of users. And are you sure.
    if(isUserExist(user)){
        int userIndex = getUserIndex(user);
        m_allUsers.removeAt(userIndex);
        delete m_userButtonRelays.takeAt(userIndex);
        delete m_userPushButtons.takeAt(userIndex);
        removeFromStoredUsers();    
        return true;
    }
    else
        return false;    
}

bool Title::isUserExist(QString user)
{
        for(UserSettings storedUser : m_allUsers){
            if(storedUser.getName() == user)
                return true;
        }
        return false;
}

int Title::getUserIndex(QString user)
{
    for(int i=0; i<m_allUsers.size(); ++i){
       UserSettings storedUser = m_allUsers.at(i);
       if(storedUser.getName() == user)
           return i;
    }
    return -1;
}

void Title::on_startGame_clicked()
{
    emit startGame();
}

void Title::on_AddUser_clicked()
{

    bool userClickedOk;
    QString label = "Please enter a name for the new user: ";
    QString newUser = QInputDialog::getText(this, "New User", label,
                                            QLineEdit::Normal, "", &userClickedOk);
    if(userClickedOk && !newUser.isEmpty()){
        if(!addUser(newUser)){
            QMessageBox msg;
            msg.setText("This name is taken! Please add a unique user name.");
            msg.exec();
        }
    }
}
