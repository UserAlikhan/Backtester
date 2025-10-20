#include "fixedStopLoss.h"
#include <iostream>

FixedStopLoss::FixedStopLoss(double percentage) : CloseOrder(percentage) {};

void FixedStopLoss::setPrice(Trade* trade, double& entryPrice) {
    switch(trade->getTradeType()) {
        case LONG: {
            price = entryPrice * (1 - percentage / 100.0);
            break;
        }
        case SHORT: {
            price = entryPrice * (1 + percentage / 100.0);
            break;
        }
    }

    std::cout << "stop-loss: " << price << " for entry: " << entryPrice << std::endl;
}

void FixedStopLoss::checkExit(Trade* trade, double close) {
    if (trade->getClosePrice() != 0.0) return;
    if (price > 0.0) {
        // if price is bigger than stop loss close the long trade
        if (trade->getTradeType() == TradeType::LONG && price >= close) {
            trade->closeTrade(price);
            std::cout << "CLOSE PRICE " << price << std::endl;
            return;
        // if price is lower than stop loss close the short trade
        } else if (trade->getTradeType() == TradeType::SHORT && price <= close) {
            trade->closeTrade(price);
            std::cout << "CLOSE PRICE " << price << std::endl;
            return;
        } 
    }
}