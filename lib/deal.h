#ifndef DEAL_H
#define DEAL_H

#include <QString>

struct Deal
{
    explicit Deal();
    explicit Deal(const Deal& other);

    void setUTCDate(const QString& utfDate);

    int id, cost, value;
    QString date, name;
};

#endif // DEAL_H
