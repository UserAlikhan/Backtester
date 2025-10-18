#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "candle.h"

class Indicator {
    protected:
        std::string name;
    public:
        // constructor
        Indicator(std::string name) : name(name) {}

        // virtual destructor
        virtual ~Indicator() {}

        // accessor
        std::string getName() const { return name; }

        // abstract method. calculates data points for indicator
        virtual void calculate(std::vector<Candle*>& candles) = 0;
};