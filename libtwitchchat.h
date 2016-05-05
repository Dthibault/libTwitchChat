/**
  * @file libTwitchChat.h
  * @brief C++/Qt Library for Twitch Chat
  * @author Thibault "Palamecia" Dupuis
  * @version 0.0.1
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
#include <QDebug>

#define LIBTWITCHCHAT_VERSION "0.0.1"
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
     * @brief Get the last error who occured
     * @return The last error
     */
    QString lastError();

    /**
     * @brief Connect the bot to the Twitch chat server
     * @return True if the connection is successfull, False if there is a problem
     */
    bool connect();

private:

    bool debugState; /*!< Status of the debug mode */
    QString errorString; /*!< String of the last error */

    QString botName; /*!< Bot name */
    QString botToken; /*!< Bot token */

    QTcpSocket *sock; /*!< TCP Socket for the library */
signals:

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

};

#endif // LibTwitchChat_H
