#include "testuserdata.h"
#include "../libs/userdata.h"

TestUserData::TestUserData() {}

void TestUserData::createAuthHeader_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<QString>("password");
    QTest::addColumn<QByteArray>("headerValue");

    QTest::newRow("test_case_1") << "user@email.com" << "Password" << QByteArray{"Basic dXNlckBlbWFpbC5jb206UGFzc3dvcmQ="};
    QTest::newRow("test_case_2") << "AcSwFss@email.com" << "Test_Pa*sS" << QByteArray{"Basic QWNTd0Zzc0BlbWFpbC5jb206VGVzdF9QYSpzUw=="};
}

void TestUserData::createAuthHeader()
{
    QFETCH(QString, email);
    QFETCH(QString, password);
    QFETCH(QByteArray, headerValue);

    QCOMPARE(UserData::createAuthHeader(email, password), headerValue);
}
