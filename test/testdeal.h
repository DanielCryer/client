#ifndef TESTDEAL_H
#define TESTDEAL_H

#include <QtTest>

class TestDeal : public QObject
{
    Q_OBJECT
public:
    TestDeal();

private slots:
    void defaultValues();
    void setUTCDate();
};

#endif // TESTDEAL_H
