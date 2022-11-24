#include "testrequest.h"
#include "../libs/request.h"

TestRequest::TestRequest() {}
TestRequest::~TestRequest() {}

void TestRequest::defaultValues()
{
    Request request{};

    QCOMPARE(request.id, -1);
    QCOMPARE(request.user_id, -1);
    QCOMPARE(request.cost, -1);
    QCOMPARE(request.value, -1);
    QCOMPARE(request.current_value, -1);
    QCOMPARE(request.status, "");
    QCOMPARE(request.date, "");
    QCOMPARE(request.direction, "");
}

void TestRequest::setUTCDate()
{
    Request request{};
    QDateTime date{QDateTime::currentDateTime()};

    request.setUTCDate(date.toUTC().toString(Qt::ISODateWithMs));
    QCOMPARE(request.date, date.toString("d MMM yyyy  hh:mm:ss"));
}
