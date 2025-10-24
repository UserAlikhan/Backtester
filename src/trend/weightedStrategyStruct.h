#pragma once
#include <iostream>
#include "trendStrategy.h"

struct WeightedStrategy {
    TrendStrategy* strategy;
    double weight;
};