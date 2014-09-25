#include "title.h"
#include "ui_title.h"

#include <QDebug>
#include <QSettings>
#include <QInputDialog>

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
    QPushButton* button = new QPushButton(user.getName());
    ButtonRelay* buttonRelay = new ButtonRelay(button,userIndex, this);

    scrollLayout->addWidget(button);
    connect(buttonRelay, SIGNAL(buttonClicked(int)), this, SLOT(userButtonClicked(int)));
}

void Title::userButtonClicked(int userIndex)
{
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

bool Title::addUser(QString newUser)
{
    auto checkUserExists = [&](){
        for(UserSettings user : m_allUsers){
            if(user.getName() == newUser)
                return false;
        }
        return true;
    };
    auto addUserToLocalSettings = [&](){
        QSettings settings;
        settings.beginGroup("users");
            settings.beginGroup( m_allUsers.last().getName() );
                settings.setValue( "totalBeats", m_allUsers.last().getScore() );
            settings.endGroup();
        settings.endGroup();
    };

    if(checkUserExists() == false)
        return false;
    m_allUsers.push_back(UserSettings(newUser, 0));
    addUserToLocalSettings();
    makeUserButton(m_allUsers.size()-1);
    return true;
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
    if(userClickedOk && !newUser.isEmpty())
        addUser(newUser);

     // TODO remove user admin

}
