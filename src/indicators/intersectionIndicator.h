#pragma once
#include <iostream>
#include "intersectionEnum.h"
#include "tradeTypeEnum.h"

class IntersectionIndicator {
    public:
        virtual ~IntersectionIndicator() = default;
        virtual std::vector<std::pair<TradeType, int>> findIntersections() = 0;
};