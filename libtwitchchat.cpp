#include "libtwitchchat.h"

libTwitchChat::libTwitchChat(QObject *parent) : QObject(parent)
{
    this->debugState = false;

    this->botName = QString();
    this->botToken = QString();
}

libTwitchChat::~libTwitchChat()
{

}

void libTwitchChat::setDebug(bool state)
{
    this->debugState = state;
}

void libTwitchChat::setBotName(QString newName)
{
    this->botName = newName;

    if(this->debugState)
        qDebug() << "New name for the bot: " << newName;
}

void libTwitchChat::setBotToken(QString newToken)
{
    this->botToken = newToken;

    if(this->debugState)
        qDebug() << "New token for the bot: " << newToken;
}
