#include <QtTest>
#include "testrequest.h"
#include "testdeal.h"
#include "testuserdata.h"
#include "testrequestlistmodel.h"
#include "testdeallistmodel.h"


int main(int argc, char *argv[])
{
    QTest::qExec(new TestRequest, argc, argv);
    QTest::qExec(new TestDeal, argc, argv);
    QTest::qExec(new TestUserData, argc, argv);
    QTest::qExec(new TestRequestListModel, argc, argv);
    QTest::qExec(new TestDealListModel, argc, argv);
    return 0;
}
