#ifndef FILTERS_H
#define FILTERS_H

#include <QObject>

struct Filters
{
    Q_GADGET

    Q_PROPERTY(bool isBuy MEMBER isBuy)
    Q_PROPERTY(bool isOrderByDate MEMBER isOrderByDate)
    Q_PROPERTY(bool isReverseSort MEMBER isReverseSort)

public:
    Filters();
    Filters(const Filters& other);
    bool operator!=(const Filters& other) const;

    bool isBuy, isOrderByDate, isReverseSort;
};

Q_DECLARE_METATYPE(Filters)

#endif // FILTERS_H
