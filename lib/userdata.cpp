#include "userdata.h"
#include <QJsonObject>

UserData::UserData()
{
    id = -1;
    email = "";
    name = "";
    usdBalance = -1;
    rubBalance = -1;
    authHeader = "";
}

UserData::UserData(const UserData &other)
{
    id = other.id;
    email = other.email;
    name = other.name;
    usdBalance = other.usdBalance;
    rubBalance = other.rubBalance;
    authHeader = other.authHeader;
}

UserData::UserData(const QJsonObject& _userData, const QByteArray& _authHeader)
{
    id = _userData["user_id"].toInt();
    email = _userData["email"].toString();
    name = _userData["name"].toString();
    usdBalance = _userData["USD_balance"].toInt();
    rubBalance = _userData["RUB_balance"].toInt();
    authHeader = _authHeader;
}

UserData::~UserData() {}

QByteArray UserData::createAuthHeader(const QString &email, const QString &password)
{
    return "Basic " + QString{email + ":" + password}.toUtf8().toBase64();
}
