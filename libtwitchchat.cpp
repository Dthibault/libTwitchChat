/**
  * @file libtwitchchat.cpp
  * @brief C++/Qt Library for Twitch Chat
  * @author Thibault "Palamecia" Dupuis
  * @version 0.0.1
  * @date 05/04/2016
  *
  * C++/Qt Library for Twitch Chat. This library is for bot use.
  *
  */

#include "libtwitchchat.h"

// Constructor for the LibTwitchChat class
LibTwitchChat::LibTwitchChat(QObject *parent) : QObject(parent)
{
    this->debugState = false;

    this->botName = QString();
    this->botToken = QString();
    this->errorString = QString();

    this->sock = nullptr;
}

// Destructor for the LibTwitchChat class
LibTwitchChat::~LibTwitchChat()
{

}

// Enable/Disable the debug mode
void LibTwitchChat::setDebug(bool state)
{
    this->debugState = state;
}

// Set a new name for the bot
void LibTwitchChat::setBotName(QString newName)
{
    this->botName = newName;

    if(this->debugState)
        qDebug() << "EVENT: New name for the bot: " << newName;
}


// Get the name of the bot
QString LibTwitchChat::getBotName()
{
    return this->botName;
}

// Get the token of the bot
QString LibTwitchChat::getBotToken()
{
    return this->botToken;
}

// Set a new token for the bot
void LibTwitchChat::setBotToken(QString newToken)
{
    this->botToken = newToken;

    if(this->debugState)
        qDebug() << "EVENT: New token for the bot: " << newToken;
}

// Get the last error who occured
QString LibTwitchChat::lastError()
{
    return this->errorString;
}

//Connect the bot to the Twitch chat server
bool LibTwitchChat::connect()
{
    if(this->botName.isEmpty())
    {
        this->errorString = "Connect(): Bot name is not set";
        return false;
    }

    if(this->botToken.isEmpty())
    {
        this->errorString = "Connect(): Bot token is not set";
        return false;
    }

    if(!this->botToken.contains("oauth:"))
    {
        this->errorString = "Connect(): Bot token is not a Twitch token";
        return false;
    }

    if(this->sock != nullptr)
    {
        this->errorString = "Connect(): Already connected";
        return false;
    }


    this->sock = new QTcpSocket(this);
    QObject::connect(this->sock, &QIODevice::readyRead, this, &LibTwitchChat::sockRead);
    QObject::connect(this->sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sockError(QAbstractSocket::SocketError)));
    QObject::connect(this->sock, &QAbstractSocket::connected, this, &LibTwitchChat::sockConnected);

    this->sock->abort();
    this->sock->connectToHost(QString(LIBTWITCHCHAT_SERVER), QString(LIBTWITCHCHAT_PORT).toInt());


    return true;
}


// Slot for reading socket data
void LibTwitchChat::sockRead()
{
    qDebug() << this->sock->readAll();
}


// Slot for reading socket error
void LibTwitchChat::sockError(QAbstractSocket::SocketError err)
{
    qDebug() << this->sock->state();
    qDebug() << "Err! : " << err;
}


//Slot when socket is connected to Twitch chat server
void LibTwitchChat::sockConnected()
{
    if(this->debugState)
        qDebug() << "EVENT: Connected to " << QString(LIBTWITCHCHAT_SERVER) << ":" << QString(LIBTWITCHCHAT_PORT);

    QString ans = "PASS " + this->botToken + "\r\n";
    this->sock->write(ans.toUtf8(), ans.size());

    if(this->debugState)
        qDebug() << "SEND: " << ans;

    ans = "NICK " + this->botName + "\r\n";
    this->sock->write(ans.toUtf8(), ans.size());

    if(this->debugState)
        qDebug() << "SEND: " << ans;
}
