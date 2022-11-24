#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>

struct UserData
{
    Q_GADGET

    Q_PROPERTY(int id MEMBER id)
    Q_PROPERTY(QString email MEMBER email)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int usdBalance MEMBER usdBalance)
    Q_PROPERTY(int rubBalance MEMBER rubBalance)

public:
    UserData();
    UserData(const UserData& other);
    UserData(const QJsonObject& _userData, const QByteArray& _authHeader);
    ~UserData();

    int id;
    QString email, name;
    QByteArray authHeader;
    int usdBalance, rubBalance;

    static QByteArray createAuthHeader(const QString& email, const QString& password);
};

Q_DECLARE_METATYPE(UserData)

#endif // USERDATA_H
