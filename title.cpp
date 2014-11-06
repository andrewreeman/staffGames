#include "title.h"
#include "ui_title.h"
#include "mainwindow.h"
#include "staffGamesConstants.h"
#include "gameproperties.h"

#include <QDebug>
#include <QSettings>
#include <QInputDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

Title::Title(QWidget *parent) :
    QWidget(parent), m_mainWindow( (MainWindow*)parent),
    ui(new Ui::Title), m_isFirstTimeUserSelected(true)
{
    auto constructGamePropertiesList = [&](){
        m_gameProperties.insert(gameIDs::noteFinderSpaces, new GameProperties_SpaceFinding());
        m_gameProperties.insert(gameIDs::noteFinderAll, new GameProperties_NoteFindingAll());
        m_gameProperties.insert(gameIDs::noteFinderLines, new GameProperties_LineFinding());
    };

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::title);
    constructGamePropertiesList();
}

Title::~Title()
{
    qDeleteAll(m_gameProperties);
    delete ui;
}

void Title::on_titleToLogin_clicked()
{    


    QStringList test = m_mainWindow->getAllUserNames();
    qDebug() << test;

    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::users);
    makeAllUserButtons();    
    addMenu();
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(removeMenu()));
}
/*
QList<UserSettings> Title::getAllUserSettings()
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
}*/

void Title::makeAllUserButtons()
{
    QStringList allUsers = m_mainWindow->getAllUserNames();

    for(QString user : allUsers){
        makeUserButton(user);
    }
}

void Title::makeUserButton(QString name)
{

    QLayout* scrollLayout = ui->userNameContainerContents->layout();

    QPushButton* button = new QPushButton(name);
    ButtonRelay* buttonRelay = new ButtonRelay(button,name, this);

    button->setProperty(objectPropertyKeys::userName.toLatin1(), QVariant(name));
    buttonRelay->setProperty(objectPropertyKeys::userName.toLatin1(), QVariant(name));


    m_userPushButtons.append(button);
    m_userButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QVariant)), this, SLOT(userButtonClicked(QVariant)));
}

void Title::makeAllGameButtons()
{
    QList<int> userGames = m_mainWindow->getUserOwnedGames();
    for(int game : userGames)
        makeGameButton(m_gameProperties.value(game));
}

void Title::makeGameButton(GameProperties* gameProps)
{
    QLayout* scrollLayout = ui->gameContainerContents->layout();
    QPushButton* button = new QPushButton(gameProps->gameTitle());
    ButtonRelay* buttonRelay = new ButtonRelay(button, gameProps->gameId(), this);

    m_gamePushButtons.append(button);
    m_gameButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QVariant)), this, SLOT(gameButtonClicked(QVariant)));

}

void Title::removeAllGameButtons()
{
    QLayout* gameLayout = ui->gameContainerContents->layout();

    for(QPushButton* button : m_gamePushButtons)
        gameLayout->removeWidget(button);

    qDeleteAll(m_gamePushButtons);
    m_gamePushButtons.clear();
    qDeleteAll(m_gameButtonRelays);
    m_gameButtonRelays.clear();

}

void Title::makeAllShopButtons()
{
    for(GameProperties* game : m_gameProperties){
        makeShopButton(game);
    }
}

void Title::makeShopButton(GameProperties *gameProps)
{

    QList<int> userOwnedGames = m_mainWindow->getUserOwnedGames();
    int gameID = gameProps->gameId();

    if(userOwnedGames.contains(gameID) ) return;

    QLayout* scrollLayout = ui->shopContainerContents->layout();
    QString buttonTitle = gameProps->gameTitle() + ": " + QString::number( gameProps->price() );
    QPushButton* button = new QPushButton(buttonTitle);
    ButtonRelay* buttonRelay = new ButtonRelay(button, gameID, this);

    button->setProperty(objectPropertyKeys::gameID.toLatin1(), QVariant(gameID));
    buttonRelay->setProperty(objectPropertyKeys::gameID.toLatin1(), QVariant(gameID));

    m_shopPushButtons.append(button);
    m_shopButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QVariant)), this, SLOT(shopButtonClicked(QVariant)));


}

void Title::removeShopButton(GameProperties *gameProps)
{
    int gameID = gameProps->gameId();
    QLayout* shopLayout = ui->shopContainerContents->layout();
    int otherGameID;

    for(QPushButton* button : m_shopPushButtons){
        otherGameID = button->property(objectPropertyKeys::gameID.toLatin1()).toInt();
        if(otherGameID == gameID){
            shopLayout->removeWidget(button);
            delete button;
        }
    }

    for(ButtonRelay* button : m_shopButtonRelays){
        otherGameID = button->property(objectPropertyKeys::gameID.toLatin1()).toInt();
        if(otherGameID == gameID)
            delete button;
    }

}

void Title::removeAllShopButtons()
{
    QLayout* gameLayout = ui->gameContainerContents->layout();

    for(QPushButton* button : m_shopPushButtons)
        gameLayout->removeWidget(button);

    qDeleteAll(m_shopPushButtons);
    m_shopPushButtons.clear();
    qDeleteAll(m_shopButtonRelays);
    m_shopButtonRelays.clear();
}

void Title::userButtonClicked(QVariant userName)
{    

    int newUserIndex = userIndex(userName.toString());

    //TODO remove all this now?
    if(!m_isFirstTimeUserSelected){
        int currentUserIndex = userIndex(m_mainWindow->getUserName());
        if(newUserIndex == currentUserIndex){
            ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
            return;
        }
        else{
            removeAllGameButtons();
            removeAllShopButtons();
        }
    }

    //m_user = m_allUsers.at(newUserIndex);
    m_mainWindow->setUser(userName.toString()); //TODO throw catch . perhaps signal when done
    ui->userName->setText( m_mainWindow->getUserName() );
    ui->userScore->setText( QString::number( m_mainWindow->getUserScore() ) );
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
    makeAllGameButtons();
    makeAllShopButtons();
    m_isFirstTimeUserSelected = false;

    //emit setUser(m_user);
}

void Title::gameButtonClicked(QVariant gameID)
{
    emit startGame(gameID.toInt());

}

void Title::shopButtonClicked(QVariant gameID)
{

    GameProperties* game = m_gameProperties.value(gameID.toInt());
    int gamePrice = game->price();

    if(m_mainWindow->getUserScore() >= gamePrice){
        m_mainWindow->addUserOwnedGame(gameID.toInt());
        m_mainWindow->addUserScore(-gamePrice);
        m_mainWindow->writeUserSettings();
        ui->userScore->setText( QString::number(m_mainWindow->getUserScore()) );
        makeGameButton(game);
        removeShopButton(game);
        ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
    }
    else{
        QMessageBox::information(this, "Transaction information", "You have unsufficient beats!");
    }
}
void Title::addMenu()
{
    QMenuBar* menuBar = m_mainWindow->menuBar();
    QMenu* menu = new QMenu("Tools");
    QAction* action = menu->addAction("Remove user");
    menuBar->addMenu(menu);
    connect(action, SIGNAL(triggered()), this, SLOT(removeUser_clicked()));
}

void Title::removeMenu()
{
    QMenuBar* menuBar = m_mainWindow->menuBar();
    menuBar->clear();
    disconnect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(removeMenu()));
}

void Title::removeUser_clicked()
{
    /* TODO put this in mainwindow?   not this function but remove user... fill this one later
    QString selectAName = "Please select a name to remove.";
    auto getAllUserNames = [&](){
        QStringList userNames;
        userNames << selectAName;
        for(UserSettings storedUser : m_allUsers){
            userNames << storedUser.name();
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
    } */
}

bool Title::addUser(QString newUser)
{
    //TODO put this in mainWindow or userhandler?
    /*
    auto addUserToLocalSettings = [&](){
        QSettings settings;
        settings.beginGroup("users");
            settings.beginGroup( m_allUsers.last().name() );
                settings.setValue( "totalBeats", m_allUsers.last().score() );
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
    return true; */
    return true;
}

bool Title::removeUser(QString user)
{    
    //TODO put this in main!
    /*
    auto removeFromStoredUsers = [&](){
        QSettings settings;
        settings.beginGroup(userSettingsKeys::users);
            settings.beginGroup(user);
                settings.remove("");
            settings.endGroup();
        settings.endGroup();
    };

    if(isUserExist(user)){
        int thisUserIndex = userIndex(user);
        m_allUsers.removeAt(thisUserIndex);
        delete m_userButtonRelays.takeAt(thisUserIndex);
        delete m_userPushButtons.takeAt(thisUserIndex);
        removeFromStoredUsers();    
        return true;
    }
    else
        return false;    */ return true;
}

bool Title::isUserExist(QString user)
{
    //TODO also in main or user handler
    /*
        for(UserSettings storedUser : m_allUsers){
            if(storedUser.name() == user)
                return true;
        }
        return false;
        */ return true;
}

int Title::userIndex(QString user)
{
    //TODO getAllUsers should be in MainWindow. Only gives names. Then can select user from name
    //TODO remove user also in main and add user. with getAllUsers function...buttons update...errr brain mush!
    //TODO unneeded?
    /*
    for(int i=0; i<m_allUsers.size(); ++i){
       UserSettings storedUser = m_allUsers.at(i);
       if(storedUser.name() == user)
           return i;
    }
    return -1;
    */
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


void Title::on_shopButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::shop);    
}

void Title::on_backToUserGames_clicked()
{
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
}

void Title::on_backToUserList_clicked()
{
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::users);
}
