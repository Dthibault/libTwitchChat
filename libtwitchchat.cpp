/**
  * @file libtwitchchat.cpp
  * @brief C++/Qt Library for Twitch Chat
  * @author Thibault "Palamecia" Dupuis
  * @version 0.2
  * @date 05/04/2016
  *
  * C++/Qt Library for Twitch Chat. This library is for bot use.
  *
  */

#include "libtwitchchat.h"

// Constructor for the LibTwitchChat class
LibTwitchChat::LibTwitchChat(QObject *parent) : QObject(parent)
{
    // Initialize all variables
    this->debugState = false;

    this->botName = QString();
    this->botToken = QString();

    this->connectedState = false;
    this->sock = nullptr;
}

// Destructor for the LibTwitchChat class
LibTwitchChat::~LibTwitchChat()
{

}

// Enable/Disable the debug mode
void LibTwitchChat::setDebug(bool state)
{
    // Set the debug state flag
    this->debugState = state;

    // DEBUG
    if(state)
    {
        qDebug() << "LibTwitchChat " + QString(LIBTWITCHCHAT_VERSION);
        qDebug() << "Qt Version " + QString(QT_VERSION_STR) << " - C++11";
        qDebug() << "DEBUG MODE ON\n\n";
    }
}

// Set a new name for the bot
void LibTwitchChat::setBotName(QString newName)
{
    // Set the new name
    this->botName = newName;

    // DEBUG
    if(this->debugState)
        qDebug() << "EVENT: New name for the bot: " << newName;
}


// Get the name of the bot
QString LibTwitchChat::getBotName()
{
    // Return the bot name
    return this->botName;
}

// Get the token of the bot
QString LibTwitchChat::getBotToken()
{
    // Return the bot token
    return this->botToken;
}

// Set a new token for the bot
void LibTwitchChat::setBotToken(QString newToken)
{
    // Set the new token
    this->botToken = newToken;

    // DEBUG
    if(this->debugState)
        qDebug() << "EVENT: New token for the bot: " << newToken;
}


//Connect the bot to the Twitch chat server
void LibTwitchChat::connect()
{
    // Check if the bot name is set
    if(this->botName.isEmpty())
    {
        emit error(LibTwitchChat::ErrorType::ERRBOTNAME);

        if(this->debugState)
            qDebug() << "ERR: Bot Name Error";

        return;
    }

    // Check if the bot token is set
    if(this->botToken.isEmpty())
    {
        emit error(LibTwitchChat::ErrorType::ERRBOTTOKEN);

        if(this->debugState)
            qDebug() << "ERR: Bot Token Error";

        return;
    }

    // Check if the bot token is good
    if(!this->botToken.contains("oauth:") && this->botToken.indexOf("oauth:") != 0)
    {
        emit error(LibTwitchChat::ErrorType::ERRBOTTOKEN);

        if(this->debugState)
            qDebug() << "ERR: Bot Token Error";

        return;
    }


    // Check if the socket is closed and not already connected
    if(this->sock != nullptr)
    {
        emit error(LibTwitchChat::ErrorType::ERRALREADYCONNECTED);

        if(this->debugState)
            qDebug() << "ERR: Already connected";

        return;
    }


    // Initialize the socket
    this->sock = new QTcpSocket(this);
    QObject::connect(this->sock, &QIODevice::readyRead, this, &LibTwitchChat::sockRead);
    QObject::connect(this->sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sockError(QAbstractSocket::SocketError)));
    QObject::connect(this->sock, &QAbstractSocket::connected, this, &LibTwitchChat::sockConnected);

    // Launch the connection to Twitch chat server
    this->sock->abort();
    this->sock->connectToHost(QString(LIBTWITCHCHAT_SERVER), QString(LIBTWITCHCHAT_PORT).toInt());
}


// Slot for reading socket data
void LibTwitchChat::sockRead()
{
    // Get Twitch chat server answer
    QString ans = this->sock->readAll();
    QStringList ansArray = ans.split("\r\n");

    // Remove the last answer if empty
    if(ansArray[ansArray.size() - 1].isEmpty())
        ansArray.removeLast();

    // Here, we are checking different things
    // Check for successfull connection
    // Check for a PING answer
    // If we found a PING, we'll need to send a "PONG" message
    // Check for ACK for capabilities
    for(int i = 0; i < ansArray.size(); i++)
    {
        // DEBUG
        if(this->debugState)
        {
            qDebug() << "REPLY: " << ansArray[i];
        }


        // Check if connection is successfull
        if(ansArray[i].indexOf(':') == 0)
        {
            // Find the username
            int pos = ansArray[i].indexOf(' ');
            QString tempAns = ansArray[i];
            tempAns.remove(pos, tempAns.size() - pos);

            // If the answer come from Twitch chat server
            if(tempAns == ":tmi.twitch.tv")
            {
                // Check if we got a right connection
                // 376 is the code for an ok connection
                if(ansArray[i].contains("376 " + this->botName.toLower() + " :>"))
                {
                    // The connection is OK
                    // Now, we'll need to send capabilities to the Twitch chat server
                    QTimer::singleShot(500, this, SLOT(sendCapabilitiesMembership()));

                    // DEBUG
                    if(this->debugState)
                        qDebug() << "INFO: CONNECTION IS OK | CAPABILITIES WE'LL BE SEND";

                    return;
                }

                // Check if the connection goes wrong
                QString tempCheck = ansArray[i];
                tempCheck.remove(":tmi.twitch.tv");

                if(tempCheck.contains("NOTICE") && tempCheck.indexOf("NOTICE") == 0)
                {
                    // Check for the Error logging in message
                    if(tempCheck.contains("Error loggin in"))
                    {
                        // Emit the error signal and close the connection
                        emit error(LibTwitchChat::ErrorType::ERRLOGIN);

                        this->disconnect();

                        return;
                    }
                }
            }
        }


        // Check if we got a PING command
        if(ansArray[i].contains("PING") && ansArray[i].indexOf("PING") == 0)
        {
            // Check if the PING command come from Twitch chat server
            QString tempAns = ansArray[i];
            tempAns.remove("PING ");

            // If the PING command is correct, send a PONG command
            if(tempAns == ":tmi.twitch.tv")
            {
                QString newAns = "PONG\r\n";
                this->sock->write(newAns.toUtf8(), newAns.size());

                // DEBUG
                if(this->debugState)
                {
                    qDebug() << "INFO: PING COMMAND DETECTED";
                    qDebug() << "SEND: PONG";
                }

                continue;
            }
        }


        // Check for ACK of capabilities
        if(ansArray[i].indexOf(':') == 0)
        {
            // Find the username
            int pos = ansArray[i].indexOf(' ');
            QString tempAns = ansArray[i];
            tempAns.remove(pos, tempAns.size() - pos);

            // If the answer come from Twitch chat server
            if(tempAns == ":tmi.twitch.tv")
            {
                // If we got the ACK for memberships
                // Call the commands capabilities
                if(ansArray[i].contains("CAP * ACK") && ansArray[i].contains(":twitch.tv/membership"))
                {
                    QTimer::singleShot(500, this, SLOT(sendCapabilitiesCommands()));

                    // DEBUG
                    if(this->debugState)
                        qDebug() << "INFO: ACK for Membership Capabilities";

                    continue;
                }

                // If we got the ACK for commands
                // Call the tags capabilities
                if(ansArray[i].contains("CAP * ACK") && ansArray[i].contains(":twitch.tv/commands"))
                {
                    QTimer::singleShot(500, this, SLOT(sendCapabilitiesTags()));

                    // DEBUG
                    if(this->debugState)
                        qDebug() << "INFO: ACK for Commands Capabilities";

                    continue;
                }


                // If we got the ACK for tags
                // Send a signal who said "We are connected"
                // At this moment, we can say "The connection to Twitch chat server is OK and ready to use"
                if(ansArray[i].contains("CAP * ACK") && ansArray[i].contains(":twitch.tv/tags"))
                {
                    this->connectedState = true;

                    emit connected();

                    // DEBUG
                    if(this->debugState)
                    {
                        qDebug() << "INFO: ACK for Tags Capabilities";
                        qDebug() << "INFO: CONNECTION IS OK AND READY TO USE";
                    }

                    continue;
                }
            }
        }


        // Emit the data who are received
        emit received(ansArray[i]);
    }
}


// Slot for reading socket error
void LibTwitchChat::sockError(QAbstractSocket::SocketError err)
{
    // DEBUG
    if(this->debugState)
        qDebug() << "ERROR: " << err;

    // Check for socket error
    switch(err)
    {
        // Twitch server close connection
        case QAbstractSocket::RemoteHostClosedError:
            emit error(LibTwitchChat::ErrorType::ERRSERVERCLOSED);
        break;

        // Twitch server not found
        case QAbstractSocket::HostNotFoundError:
            emit error(LibTwitchChat::ErrorType::ERRSERVERNOTFOUND);
        break;

        // Twitch server refused connection
        case QAbstractSocket::ConnectionRefusedError:
            emit error(LibTwitchChat::ErrorType::ERRSERVERREFUSED);
        break;

        // Twitch server refused connection
        case QAbstractSocket::SocketTimeoutError:
            emit error(LibTwitchChat::ErrorType::ERRSERVERTIMEOUT);
        break;

        default:
            emit error(LibTwitchChat::ErrorType::ERRSOCKUNKNOW);
        break;
    }

    // If an error is detected, close the actual connection
    this->disconnect();
}


//Slot when socket is connected to Twitch chat server
void LibTwitchChat::sockConnected()
{
    // DEBUG
    if(this->debugState)
        qDebug() << "EVENT: Connected to " << QString(LIBTWITCHCHAT_SERVER) << ":" << QString(LIBTWITCHCHAT_PORT);

    // Send the password to Twitch server
    QString ans = "PASS " + this->botToken + "\r\n";
    this->sock->write(ans.toUtf8(), ans.size());

    // DEBUG
    if(this->debugState)
        qDebug() << "SEND: " << ans;

    // Send the bot name to Twitch server
    ans = "NICK " + this->botName + "\r\n";
    this->sock->write(ans.toUtf8(), ans.size());

    // DEBUG
    if(this->debugState)
        qDebug() << "SEND: " << ans;
}


// Disconnect the bot from the Twitch chat server
void LibTwitchChat::disconnect()
{
    // If the socket is not disconnect, emit an error
    if(!this->connectedState)
    {
        emit error(LibTwitchChat::ErrorType::ERRNOTCONNECTED);

        // DEBUG
        if(this->debugState)
            qDebug() << "ERR: Not connected";

        return;
    }

    // Disconnect and reset the socket
    this->connectedState = false;
    this->sock->close();
    this->sock->abort();
    QObject::disconnect(this->sock, 0, 0, 0);
    this->sock->deleteLater();
    this->sock = nullptr;

}

// Send the membership capabilities to the Twitch chat server
void LibTwitchChat::sendCapabilitiesMembership()
{
    // Send the message to Twitch server
    QString ans = "CAP REQ :twitch.tv/membership\r\n";
    this->sock->write(ans.toUtf8(), ans.size());


    // DEBUG
    if(this->debugState)
    {
        qDebug() << "SEND: " << ans;
    }
}

// Send the commands capabilities to the Twitch chat server
void LibTwitchChat::sendCapabilitiesCommands()
{
    // Send the message to Twitch server
    QString ans = "CAP REQ :twitch.tv/commands\r\n";
    this->sock->write(ans.toUtf8(), ans.size());


    // DEBUG
    if(this->debugState)
    {
        qDebug() << "SEND: " << ans;
    }
}

// Send the tags capabilities to the Twitch chat server
void LibTwitchChat::sendCapabilitiesTags()
{
    // Send the message to Twitch server
    QString ans = "CAP REQ :twitch.tv/tags\r\n";
    this->sock->write(ans.toUtf8(), ans.size());


    // DEBUG
    if(this->debugState)
    {
        qDebug() << "SEND: " << ans;
    }
}

// Check if the bot is connected to Twitch chat server
bool LibTwitchChat::isConnected()
{
    // Return the state of the connection
    return this->connectedState;
}


// Send data string to Twitch chat server
void LibTwitchChat::send(QString data)
{
    // Copy the data and add '\r\n' to the end
    QString copy = data;
    copy.append("\r\n");

    // Send the data to Twitch chat server
    this->sock->write(copy.toUtf8(), copy.size());
}
