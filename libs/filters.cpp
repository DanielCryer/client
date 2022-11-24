#include "filters.h"

Filters::Filters() : isBuy{true}, isOrderByDate{true}, isReverseSort{false} {}

Filters::Filters(const Filters &other) : isBuy{other.isBuy},
    isOrderByDate{other.isOrderByDate},
    isReverseSort{other.isReverseSort} {}

bool Filters::operator!=(const Filters &other) const
{
    return isBuy != other.isBuy ||
           isOrderByDate != other.isOrderByDate ||
           isReverseSort != other.isReverseSort;
}
