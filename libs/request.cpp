#include "request.h"
#include <QDateTime>
#include <QTimeZone>

Request::Request()
{
    id = -1;
    status = "";
    date = "";
    user_id = -1;
    current_value = -1;
    value = -1;
    direction = "";
    cost = -1;
}

Request::Request(const Request &other)
{

    id = other.id;
    status = other.status;
    date = other.date;
    user_id = other.user_id;
    current_value = other.current_value;
    value = other.value;
    direction = other.direction;
    cost = other.cost;
}

void Request::setUTCDate(const QString &utfDate)
{
    auto time = QDateTime::fromString(utfDate, Qt::ISODateWithMs);
    time.setTimeZone(QTimeZone{0});
    date = time.toLocalTime().toString("d MMM yyyy  hh:mm:ss");
}
