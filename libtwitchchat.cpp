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

// Constructor for the libTwitchChat class
libTwitchChat::libTwitchChat(QObject *parent) : QObject(parent)
{
    this->debugState = false;

    this->botName = QString();
    this->botToken = QString();
}

// Destructor for the libTwitchChat class
libTwitchChat::~libTwitchChat()
{

}

// Enable/Disable the debug mode
void libTwitchChat::setDebug(bool state)
{
    this->debugState = state;
}

// Set a new name for the bot
void libTwitchChat::setBotName(QString newName)
{
    this->botName = newName;

    if(this->debugState)
        qDebug() << "New name for the bot: " << newName;
}


// Get the name of the bot
QString libTwitchChat::getBotName()
{
    return this->botName;
}

// Get the token of the bot
QString libTwitchChat::getBotToken()
{
    return this->botToken;
}

// Set a new token for the bot
void libTwitchChat::setBotToken(QString newToken)
{
    this->botToken = newToken;

    if(this->debugState)
        qDebug() << "New token for the bot: " << newToken;
}
