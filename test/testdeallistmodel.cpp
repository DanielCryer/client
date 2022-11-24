#include "testdeallistmodel.h"
#include "../libs/deallistmodel.h"
#include <QJsonObject>

TestDealListModel::TestDealListModel()
{
    auto validReq = QJsonObject({{"deal_id", 1},
                               {"value", 27},
                               {"cost", 34},
                               {"name", "tod"},
                               {"date", "2022-10-16 13:24:12"}});

    auto invalidReq = QJsonObject({{"request_id", 1},
                                 {"user_id", 1},
                                 {"status", "open"},
                                 {"direction", "buy"},
                                 {"place", "y"}});

    _validArray = QJsonArray{validReq, validReq};
    _invalidArray = QJsonArray{validReq, invalidReq};
}

void TestDealListModel::setData_data()
{
    QTest::addColumn<QJsonArray>("jsonArray");
    QTest::addColumn<int>("count");

    QTest::addRow("validArr") << _validArray << 2;
    QTest::addRow("invalidArr") << _invalidArray << 0;
}

void TestDealListModel::setData()
{
    QFETCH(QJsonArray, jsonArray);
    QFETCH(int, count);

    DealListModel model{};
    model.setData(jsonArray);
    QCOMPARE(model.rowCount(), count);
}
