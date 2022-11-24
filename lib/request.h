#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>

struct Request
{
    Q_GADGET

    Q_PROPERTY(int id MEMBER id)
    Q_PROPERTY(int user_id MEMBER user_id)
    Q_PROPERTY(int cost MEMBER cost)
    Q_PROPERTY(int value MEMBER value)
    Q_PROPERTY(int current_value MEMBER current_value)
    Q_PROPERTY(QString status MEMBER status)
    Q_PROPERTY(QString date MEMBER date)
    Q_PROPERTY(QString direction MEMBER direction)

public:
    explicit Request();
    explicit Request(const Request& other);

    void setUTCDate(const QString& utfDate);

    int id, user_id, cost,
        value, current_value;
    QString status, date, direction;
};

Q_DECLARE_METATYPE(Request)

#endif // REQUEST_H
