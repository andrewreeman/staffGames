#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"

#include "exceptions.h"
#include <QMessageBox>
#include <QSettings>

#include "game_notefinding.h"
#include "game_linefinding.h"
#include "game_spacefinding.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_game(nullptr), m_title(nullptr),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    initUserNames();
    initTitle();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGame(int gameId)
{    
    try{
        removeWidget(m_title);        
        initGame(gameId);
    }
    catch(Except_StaffGames &e){
        m_game = nullptr;
        initTitle();
        QMessageBox::critical(this, "Error", QString() + "Error starting game.\n" + e.what());
    }
}

void MainWindow::stopGame()
{
    removeWidget(m_game);
    m_game = nullptr;
    initTitle(true);

}

void MainWindow::setUser(QString userName)
{
    QSettings settings;
    bool gameIsOwned;
    int totalBeats;

    settings.beginGroup(userSettingsKeys::users);
        settings.beginGroup(userName);
            totalBeats = settings.value(userSettingsKeys::totalBeats, QVariant(0)).toInt();
            m_user = UserSettings(userName, totalBeats);
            settings.beginGroup(userSettingsKeys::ownedGames);
                for(int i=0; i<gameIDs::numGames; ++i){
                    gameIsOwned = settings.value(QString::number(i), QVariant(false)).toBool();
                    if(gameIsOwned)
                        m_user.addOwnedGame(i);
                }
                settings.endGroup();
        settings.endGroup();
    settings.endGroup();

}

bool MainWindow::addUser(QString userName)
{

    auto addUserToLocalSettings = [&](){
        QSettings settings;
        settings.beginGroup(userSettingsKeys::users);
            settings.beginGroup(userName);
                settings.setValue(userSettingsKeys::totalBeats, 0);
                settings.beginGroup(userSettingsKeys::ownedGames);
                    settings.setValue(QString::number(gameIDs::noteFinderSpaces), true);
                settings.endGroup();
            settings.endGroup();
        settings.endGroup();
    };
    if(m_userNames.contains(userName)){
        return false;
    }
    m_userNames.push_back(userName);
    addUserToLocalSettings();    
    return true;
}

bool MainWindow::removeUser(QString userName)
{
    auto removeFromStoredUsers = [&](){
        QSettings settings;
        settings.beginGroup(userSettingsKeys::users);
            settings.beginGroup(userName);
                settings.remove("");
            settings.endGroup();
        settings.endGroup();
    };

    if( m_userNames.removeOne(userName) ){
        removeFromStoredUsers();
        return true;
    }
    else
        return false;
}

void MainWindow::removeWidget(QWidget *widget)
{
    ui->gameContainer->removeWidget(widget);
    widget->hide();
    widget->close();
}

void MainWindow::initGame(int gameId)
{
    try{
        m_game = m_gameFactory.createGame(gameId, this);
        m_game->setAttribute(Qt::WA_DeleteOnClose);
        ui->gameContainer->addWidget(m_game);
        connect(m_game, SIGNAL(stopGame()), this, SLOT(stopGame()));
        m_game->startGame();
    }
    catch(Except_MemoryAlloc &e){
        qCritical() << "Error: Could not allocate memory for game of gameID " << gameId << "\n";
        throw;
    }
    catch(Except_OutOfBounds &e){
        qCritical() << "Error: gameID " << gameId << " is not a valid gameID" << "\n";
        throw;
    }
    catch(Except_StaffGames &e){
        throw;
    }
    catch(...){
        throw;
    }
}

void MainWindow::initTitle(bool isUserSelected)
{
    m_title = new Title(this, isUserSelected);
    m_title->setAttribute(Qt::WA_DeleteOnClose);
    ui->gameContainer->addWidget(m_title);
    connect(m_title, SIGNAL(startGame(int)), this, SLOT(startGame(int)));    
}


void MainWindow::initUserNames()
{
    QSettings settings;    
    settings.beginGroup(userSettingsKeys::users);
        m_userNames = settings.childGroups();
    settings.endGroup();
}
