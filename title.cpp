#include "title.h"
#include "ui_title.h"
#include "mainwindow.h"
#include "staffGamesConstants.h"

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
//TODO do not need to load ALL user settings until user selected. This function should just return all user names
{
    QList<UserSettings> allUserSettings;
    QSettings settings;
    QStringList userNames;
    bool gameIsOwned;

    settings.beginGroup(userSettingsKeys::users);
        userNames = settings.childGroups();
        for(QString user : userNames){
            settings.beginGroup(user);
            allUserSettings.push_back( UserSettings(user, settings.value(userSettingsKeys::totalBeats).toInt()) );
                settings.beginGroup(userSettingsKeys::ownedGames);
                    for(int i=0; i<gameIDs::numGames; ++i){
                        gameIsOwned = settings.value(QString::number(i), QVariant(false)).toBool();
                        if(gameIsOwned)
                            allUserSettings.last().addOwnedGame(i);
                    }
                settings.endGroup();
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
    //TODO when selected user need a makeGameButton that creates buttons from owned games.

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

void Title::userButtonClicked(QString userName)
{    

    int userIndex = getUserIndex(userName);
    UserSettings user = m_allUsers.at(userIndex);
    ui->userName->setText( user.getName() );
    ui->userScore->setText( QString::number(user.getScore()) );
    ui->stackedWidget->setCurrentIndex(2);
    emit setUser(user);
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
    QString selectAName = "Please select a name to remove.";
    auto getAllUserNames = [&](){
        QStringList userNames;
        userNames << selectAName;
        for(UserSettings storedUser : m_allUsers){
            userNames << storedUser.getName();
        }
        return userNames;
    };
    bool userClickedOk;
    QString label = "Enter a user name to remove: ";    
    QString user = QInputDialog::getItem(this, "Remove A User", label, getAllUserNames(),
                                         0, false, &userClickedOk);

    if(userClickedOk && user != selectAName){
        QMessageBox msg;
        msg.setText("Are you sure you wish to remove this user?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        int userYes = msg.exec();
        if(userYes == QMessageBox::Yes)
            removeUser(user);
    }
}

bool Title::addUser(QString newUser)
{
    auto addUserToLocalSettings = [&](){
        QSettings settings;
        settings.beginGroup("users");
            settings.beginGroup( m_allUsers.last().getName() );
                settings.setValue( "totalBeats", m_allUsers.last().getScore() );
                settings.beginGroup(userSettingsKeys::ownedGames);
                    settings.setValue(QString::number(gameIDs::noteFinderSpaces), true);
                settings.endGroup();
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

void Title::on_playFullStave_clicked()
{
    emit startGame(gameIDs::noteFinderAll);
}

void Title::on_playLines_clicked()
{
    emit startGame(gameIDs::noteFinderLines);
}

void Title::on_playSpaces_clicked()
{
    emit startGame(gameIDs::noteFinderSpaces);
}
