#include "trailingStopLoss.h"
#include <iostream>

TrailingStopLoss::TrailingStopLoss(double percentage) : StopLoss(percentage) {}

void TrailingStopLoss::setPrice(Trade* trade, double& currentPrice) {
    switch(trade->getTradeType()) {
        case LONG: {
            if (currentPrice >= trade->getEntryPrice() * (1 + percentage / 100)) {
                double newStop = currentPrice * (1 - percentage / 100);

                if (newStop > price) {
                    price = newStop;
                }
            }

            break;
        }
        case SHORT: {
            if (currentPrice <= trade->getEntryPrice() * (1 - percentage / 100)) {
                double newStop = currentPrice * (1 + percentage / 100);

                if (price == 0.0 || newStop < price) {
                    price = newStop;
                }
            }

            break;
        }
    }
}


void TrailingStopLoss::checkExit(Trade* trade, int& index, std::vector<Candle*>& candles) {

}