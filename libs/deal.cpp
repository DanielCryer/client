#include "deal.h"
#include <QDateTime>
#include <QTimeZone>

Deal::Deal()
{
    id = -1;
    date = "";
    value = -1;
    cost = -1;
    name = "";
}

Deal::Deal(const Deal &other)
{
    id = other.id;
    date = other.date;
    value = other.value;
    cost = other.cost;
    name = other.name;
}

void Deal::setUTCDate(const QString &utfDate)
{
    auto time = QDateTime::fromString(utfDate, Qt::ISODateWithMs);
    time.setTimeZone(QTimeZone{0});
    date = time.toLocalTime().toString("d MMM yyyy  hh:mm:ss");
}
