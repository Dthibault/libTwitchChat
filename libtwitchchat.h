/**
  * @file libtwitchchat.h
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
#include <QDebug>

#define LIBTWITCHCHAT_VERSION "0.0.1"

/**
 * @class libTwitchChat
 * @brief The libTwitchChat class
 *
 * This class is used for the whole library
 */
class libTwitchChat : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     *
     * Constructor for the libTwitchChat class
     *
     * @param parent
     */
    explicit libTwitchChat(QObject *parent = 0);

    /**
     * @brief Destructor
     *
     * Destructor for the libTwitchChat class
     *
     */
    virtual ~libTwitchChat();

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

private:

    bool debugState; /*!< Status of the debug mode */

    QString botName; /*!< Bot name */
    QString botToken; /*!< Bot token */
signals:

public slots:
};

#endif // LIBTWITCHCHAT_H
