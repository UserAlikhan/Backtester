#pragma once
#include <iostream>
#include "intersectionEnum.h"

class IntersectionIndicator {
    public:
        virtual ~IntersectionIndicator() = default;
        virtual std::vector<std::pair<IntersectionEnum, int>> findIntersections() = 0;
};