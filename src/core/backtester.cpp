#include "backtester.h"
#include <iostream>

// constructor
Backtester::Backtester() {};

// destructor
Backtester::~Backtester() {
    for (auto t : trades) {
        delete t;
    }

    for (auto sl: stopLosses) {
        delete sl;
    }
}

void Backtester::addTrade() {

}

void Backtester::addStopLoss() {

}

void Backtester::run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators) {
    for (auto indicator: indicators) {
        if (IntersectionIndicator* inter = dynamic_cast<IntersectionIndicator*>(indicator)) {
            indicator->calculate(candles);
            std::vector<std::pair<IntersectionEnum, int>> intersections = inter->findIntersections();

            for (auto intersection : intersections) {
                // execute trade
                std::cout << "Executing trade: " << intersection.second << std::endl;
            }

            std::cout << "Num of trades: " << intersections.size() << std::endl;
        }
    }
}