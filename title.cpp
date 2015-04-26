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
#include <QFile>

Title::Title(QWidget *parent, bool isUserSelected) :
    QWidget(parent), m_mainWindow( (MainWindow*)parent),
    ui(new Ui::Title)
{
    auto constructGamePropertiesList = [&](){
        m_gameProperties.insert(gameIDs::noteFinderSpaces, new GameProperties_SpaceFinding());
        m_gameProperties.insert(gameIDs::noteFinderAll, new GameProperties_NoteFindingAll());
        m_gameProperties.insert(gameIDs::noteFinderLines, new GameProperties_LineFinding());
    };

    ui->setupUi(this);
    constructGamePropertiesList();

    if(isUserSelected){
        userButtonClicked(m_mainWindow->user()->name());
    }
    else{
       ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::title);
    }
    loadStyleSheet();
}

Title::~Title()
{
    qDeleteAll(m_gameProperties);
    delete ui;
}

void Title::on_titleToLogin_clicked()
{     
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::users);
    makeAllUserButtons();    
    addMenu();
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(removeMenu()));
}

void Title::loadStyleSheet(){
    QFile style(":/stylesheet/global.css");

    style.open(QFile::ReadOnly);
    setStyleSheet(style.readAll());
}

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

    button->setProperty(propertyKeys::userName.toLatin1(), QVariant(name));
    buttonRelay->setProperty(propertyKeys::userName.toLatin1(), QVariant(name));


    m_userPushButtons.append(button);
    m_userButtonRelays.append(buttonRelay);
    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(QVariant)), this, SLOT(userButtonClicked(QVariant)));
}

void Title::removeAllUserButtons()
{
    QLayout* userLayout = ui->userNameContainerContents->layout();

    for(QPushButton* button : m_userPushButtons)
        userLayout->removeWidget(button);

    qDeleteAll(m_userPushButtons);
    m_userPushButtons.clear();
    qDeleteAll(m_userButtonRelays);
    m_userButtonRelays.clear();
}


void Title::userButtonClicked(QVariant userName)
{
    removeAllGameButtons();
    removeAllShopButtons();

    m_mainWindow->setUser(userName.toString());
    ui->userName->setText(userName.toString());
    ui->userScore->setText( QString::number( m_mainWindow->user()->score() ) );
    ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::userHome);
    makeAllGameButtons();
    makeAllShopButtons();
    removeAllUserButtons();
}


void Title::makeAllGameButtons()
{       
    for(int game : m_mainWindow->user()->ownedGames())
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

    QList<int> userOwnedGames = m_mainWindow->user()->ownedGames();
    int gameID = gameProps->gameId();

    if(userOwnedGames.contains(gameID) ) return;

    QLayout* scrollLayout = ui->shopContainerContents->layout();
    QString buttonTitle = gameProps->gameTitle() + ": " + QString::number( gameProps->price() );
    QPushButton* button = new QPushButton(buttonTitle);
    ButtonRelay* buttonRelay = new ButtonRelay(button, gameID, this);

    button->setProperty(propertyKeys::gameID.toLatin1(), QVariant(gameID));
    buttonRelay->setProperty(propertyKeys::gameID.toLatin1(), QVariant(gameID));

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

    for(int i=0; i<m_shopPushButtons.size(); ++i){
        QPushButton* button = m_shopPushButtons.at(i);
        otherGameID = button->property(propertyKeys::gameID.toLatin1()).toInt();
        if(otherGameID == gameID){
            shopLayout->removeWidget(button);
            delete button;
            m_shopPushButtons.removeAt(i);
            break;
        }
    }

    for(int i=0; i<m_shopButtonRelays.size(); ++i){
        ButtonRelay* button = m_shopButtonRelays.at(i);
        otherGameID = button->property(propertyKeys::gameID.toLatin1()).toInt();
        if(otherGameID == gameID)
            delete button;
            m_shopButtonRelays.removeAt(i);
            break;
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

void Title::gameButtonClicked(QVariant gameID)
{
    emit startGame(gameID.toInt());

}

void Title::shopButtonClicked(QVariant gameID)
{

    GameProperties* game = m_gameProperties.value(gameID.toInt());
    UserSettings* user = m_mainWindow->user();
    int gamePrice = game->price();

    if(user->score() >= gamePrice){
        user->addOwnedGame(gameID.toInt());
        user->addScore(-gamePrice);
        user->write();
        ui->userScore->setText( QString::number(user->score()) );
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
    QString selectAName = "Please select a name to remove.";
    auto selectionList = [&](){
        QStringList userNames;
        userNames << selectAName;
        userNames << m_mainWindow->getAllUserNames();
        return userNames;
    };    
    bool userClickedOk;
    QString label = "Enter a user name to remove: ";    
    QString user = QInputDialog::getItem(this, "Remove A User", label, selectionList(),
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
    newUser = newUser.simplified();
    if(m_mainWindow->addUser(newUser)){
        makeUserButton(newUser);
        return true;
    }
    return false;
}

bool Title::removeUser(QString userName)
{
    if(m_mainWindow->removeUser(userName)){

        for(int i=0; i<m_userButtonRelays.size(); ++i){
            ButtonRelay* buttonRelay = m_userButtonRelays.at(i);
            QString buttonUser = buttonRelay->property(propertyKeys::userName.toLatin1()).toString();
            if(buttonUser == userName)
                delete m_userButtonRelays.takeAt(i);
                break;
        }

        for(int i=0; i<m_userPushButtons.size(); ++i){
            QPushButton* button = m_userPushButtons.at(i);
            QString buttonUser = button->property(propertyKeys::userName.toLatin1()).toString();
            if(buttonUser == userName)
                delete m_userPushButtons.takeAt(i);
                break;
        }
        return true;
    }
    return false;
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
    //on_titleToLogin_clicked();
}

void Title::on_backToUserList_clicked()
{
    on_titleToLogin_clicked();
    /*ui->stackedWidget->setCurrentIndex(titleStackedWidgetIndices::users);
    addMenu();
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(removeMenu()));*/
}
