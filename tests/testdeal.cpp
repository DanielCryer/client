#include "testdeal.h"
#include "../lib/deal.h"

TestDeal::TestDeal() {}

void TestDeal::defaultValues()
{
    Deal deal{};

    QCOMPARE(deal.id, -1);
    QCOMPARE(deal.cost, -1);
    QCOMPARE(deal.value, -1);
    QCOMPARE(deal.date, "");
    QCOMPARE(deal.name, "");
}

void TestDeal::setUTCDate()
{
    Deal deal{};
    QDateTime date{QDateTime::currentDateTime()};

    deal.setUTCDate(date.toUTC().toString(Qt::ISODateWithMs));
    QCOMPARE(deal.date, date.toString("d MMM yyyy  hh:mm:ss"));
}
