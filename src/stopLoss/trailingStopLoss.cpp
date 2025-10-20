#include "trailingStopLoss.h"
#include <iostream>

TrailingStopLoss::TrailingStopLoss(double percentage) : StopLoss(percentage) {}

void TrailingStopLoss::setPrice(Trade* trade, double& orderPrice) {
    switch(trade->getTradeType()) {
        case LONG: {
            // if they are equal, it means we just opened the trade
            if (trade->getEntryPrice() == orderPrice) {
                price = trade->getEntryPrice() * (1 - percentage / 100);
            // move trailing stop loss if price rised (LONG CASE)
            } else if (trade->getEntryPrice() < orderPrice) {
                price = orderPrice * (1 - percentage / 100);
            }

            break;
        }
        case SHORT: {
            if (trade->getEntryPrice() == orderPrice) {
                price = orderPrice * (1 + percentage / 100);
            // move trailing price is price declined (SHORT CASE)
            } else if (trade->getEntryPrice() > orderPrice) {
                price = orderPrice * (1 + percentage / 100);
            }

            break;
        }
    }
}


void TrailingStopLoss::checkExit(Trade* trade, int& index, std::vector<Candle*>& candles) {

}