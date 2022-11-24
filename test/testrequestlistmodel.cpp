#include "testrequestlistmodel.h"
#include "../libs/requestlistmodel.h"
#include <QJsonObject>

TestRequestListModel::TestRequestListModel()
{
    auto validReq = QJsonObject({{"request_id", 1},
                               {"user_id", 1},
                               {"status", "open"},
                               {"direction", "buy"},
                               {"value", 27},
                               {"current_value", 20},
                               {"cost", 34},
                               {"open_date", "2022-10-16 13:24:12"}});

    auto invalidReq = QJsonObject({{"request_id", 1},
                                 {"user_id", 1},
                                 {"status", "open"},
                                 {"direction", "buy"},
                                 {"place", "y"}});

    _validArray = QJsonArray{validReq, validReq};
    _invalidArray = QJsonArray{validReq, invalidReq};
}

void TestRequestListModel::setData_data()
{
    QTest::addColumn<QJsonArray>("jsonArray");
    QTest::addColumn<int>("count");

    QTest::addRow("validArr") << _validArray << 2;
    QTest::addRow("invalidArr") << _invalidArray << 0;
}

void TestRequestListModel::setData()
{
    QFETCH(QJsonArray, jsonArray);
    QFETCH(int, count);

    RequestListModel model{};
    model.setData(jsonArray);
    QCOMPARE(model.rowCount(), count);
}

void TestRequestListModel::addData_data()
{
    QTest::addColumn<QJsonArray>("jsonArray");
    QTest::addColumn<int>("count");

    QTest::addRow("validArr") << _validArray << 4;
    QTest::addRow("invalidArr") << _invalidArray << 0;
}

void TestRequestListModel::addData()
{
    QFETCH(QJsonArray, jsonArray);
    QFETCH(int, count);

    RequestListModel model{};
    model.setData(jsonArray);
    model.addData(jsonArray);
    QCOMPARE(model.rowCount(), count);
}
