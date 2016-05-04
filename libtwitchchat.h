#ifndef LIBTWITCHCHAT_H
#define LIBTWITCHCHAT_H

#include <QObject>
#include <QString>
#include <QDebug>

#define LIBTWITCHCHAT_VERSION "0.0.1"

class libTwitchChat : public QObject
{
    Q_OBJECT
public:
    explicit libTwitchChat(QObject *parent = 0);
    virtual ~libTwitchChat();

    void setDebug(bool state);

    void setBotName(QString newName);
    void setBotToken(QString newToken);

private:

    bool debugState;

    QString botName;
    QString botToken;
signals:

public slots:
};

#endif // LIBTWITCHCHAT_H
