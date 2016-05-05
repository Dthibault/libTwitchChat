/*
Copyright (C) 2016  Thibault Dupuis

This file is part of libTwitchChat.

libTwitchChat is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
  * @file libTwitchChat.h
  * @brief C++/Qt Library for Twitch Chat
  * @author Thibault "Palamecia" Dupuis
  * @version 0.2
  * @date 05/04/2016
  *
  * C++/Qt Library for Twitch Chat. This library is for bot use.
  *
  */

#ifndef LIBTWITCHCHAT_H
#define LIBTWITCHCHAT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QIODevice>
#include <QTimer>
#include <QDebug>

#define LIBTWITCHCHAT_VERSION "0.2"
#define LIBTWITCHCHAT_SERVER "irc.chat.twitch.tv"
#define LIBTWITCHCHAT_PORT "6667"

/**
 * @class LibTwitchChat
 * @brief The LibTwitchChat class
 *
 * This class is used for the whole library
 */
class LibTwitchChat : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     *
     * Constructor for the LibTwitchChat class
     *
     * @param parent
     */
    explicit LibTwitchChat(QObject *parent = 0);

    /**
     * @brief Destructor
     *
     * Destructor for the LibTwitchChat class
     *
     */
    virtual ~LibTwitchChat();

    /**
     * @brief Enable/Disable the debug mode
     * @param state True/False for enable/disable the debug mode
     */
    void setDebug(bool state);

    /**
     * @brief Set a new name for the bot
     * @param newName Name of the bot
     */
    void setBotName(QString newName);

    /**
     * @brief Get the name of the bot
     * @return The bot name
     */
    QString getBotName();

    /**
     * @brief Set a new token for the bot
     * @param newToken Token of the bot
     */
    void setBotToken(QString newToken);

    /**
     * @brief Get the token of the bot
     * @return The bot token
     */
    QString getBotToken();


    /**
     * @brief Connect the bot to the Twitch chat server
     */
    void connect();

    /**
     * @brief Disconnect the bot from the Twitch chat server
     */
    void disconnect();

    /**
     * @brief Check if the bot is connected to Twitch chat server
     * @return True if the bot is connected to Twitch chat server, False if not
     */
    bool isConnected();

    /**
     * @brief Send data string to Twitch chat server
     * @param data String who are send to Twitch chat server, \r\n is added at the end
     */
    void send(QString data);

    /**
     * @brief The ErrorType enum
     */
    enum ErrorType
    {
        ERRBOTNAME,
        ERRBOTTOKEN,
        ERRALREADYCONNECTED,
        ERRSERVERCLOSED,
        ERRSERVERNOTFOUND,
        ERRSERVERREFUSED,
        ERRSERVERTIMEOUT,
        ERRLOGIN,
        ERRNOTCONNECTED,
        ERRSOCKUNKNOW
    };

private:

    bool debugState; /*!< Status of the debug mode */

    bool connectedState; /*!< Status of the connection */

    QString botName; /*!< Bot name */
    QString botToken; /*!< Bot token */

    QTcpSocket *sock; /*!< TCP Socket for the library */

signals:

    /**
     * @brief Signal emitted when there is an error
     */
    void error(LibTwitchChat::ErrorType);

    /**
     * @brief Signal emitted when the connection to Twitch chat server is ok
     */
    void connected();

    /**
     * @brief Signal emitted when data are received from Twitch chat server;
     */
    void received(QString);

private slots:

    /**
     * @brief Slot for reading socket data
     */
    void sockRead();

    /**
     * @brief Slot for reading socket error
     * @param err Error from the socket
     */
    void sockError(QAbstractSocket::SocketError err);

    /**
     * @brief Slot when socket is connected to Twitch chat server
     */
    void sockConnected();

    /**
     * @brief Send the membership capabilities to the Twitch chat server
     */
    void sendCapabilitiesMembership();

    /**
     * @brief Send the commands capabilities to the Twitch chat server
     */
    void sendCapabilitiesCommands();

    /**
     * @brief Send the tags capabilities to the Twitch chat server
     */
    void sendCapabilitiesTags();
};

#endif // LibTwitchChat_H
