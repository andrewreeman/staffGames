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
    QWidget(parent),
    ui(new Ui::Title)
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
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::users);
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

    QLayout* scrollLayout = ui->userNameContainerContents->layout();
    UserSettings user = m_allUsers.at(userIndex);
    QString name = user.getName();
    QPushButton* button = new QPushButton(name);
    ButtonRelay* buttonRelay = new ButtonRelay(button,name, this);

    m_userPushButtons.append(button);
    m_userButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QVariant)), this, SLOT(userButtonClicked(QVariant)));
}

void Title::makeAllGameButtons()
{
    QList<int> userGames = m_user.getOwnedGames();
    for(int game : userGames)
        makeGameButton(m_gameProperties.value(game));
}

void Title::makeGameButton(GameProperties* gameProps)
{
    QLayout* scrollLayout = ui->gameContainerContents->layout();
    QPushButton* button = new QPushButton(gameProps->getGameTitle());
    ButtonRelay* buttonRelay = new ButtonRelay(button, gameProps->getGameId(), this);

    m_gamePushButtons.append(button);
    m_gameButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QVariant)), this, SLOT(gameButtonClicked(QVariant)));

}

void Title::makeAllShopButtons()
{
    for(GameProperties* game : m_gameProperties){
        makeShopButton(game);
    }
}

void Title::makeShopButton(GameProperties *gameProps)
{

    QList<int> userOwnedGames = m_user.getOwnedGames();
    int gameID = gameProps->getGameId();

    if(userOwnedGames.contains(gameID) ) return;

    QLayout* scrollLayout = ui->shopContainerContents->layout();
    QString buttonTitle = gameProps->getGameTitle() + ": " + QString::number( gameProps->getPrice() );
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
    int gameID = gameProps->getGameId();
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

void Title::userButtonClicked(QVariant userName)
{    

    int userIndex = getUserIndex(userName.toString());
    m_user = m_allUsers.at(userIndex);

    ui->userName->setText( m_user.getName() );
    ui->userScore->setText( QString::number(m_user.getScore()) );
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
    //TODO button to go back to user select. If new user selected then remove all game and shop buttons ... just user "takeAt to remove from layout from index"
    makeAllGameButtons();
    makeAllShopButtons();
    emit setUser(m_user);
}

void Title::gameButtonClicked(QVariant gameID)
{
    emit startGame(gameID.toInt());

}

void Title::shopButtonClicked(QVariant gameID)
{
    GameProperties* game = m_gameProperties.value(gameID.toInt());
    int gamePrice = game->getPrice();

    if(m_user.getScore() >= gamePrice){
        m_user.addOwnedGame(gameID.toInt());
        m_user.addScore(-gamePrice);
        m_user.write();
        ui->userScore->setText( QString::number(m_user.getScore()) );
        makeGameButton(game);
        removeShopButton(game);
        ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
    }
    else{
        QMessageBox::information(this, "Transaction information", "You have unsufficient beats!");
    }

//TODO change all gets to property name
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


void Title::on_shopButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::shop);    
}

void Title::on_backToUserGames_clicked()
{
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
}
