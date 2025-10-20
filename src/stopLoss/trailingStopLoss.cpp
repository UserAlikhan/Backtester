#include "trailingStopLoss.h"
#include <iostream>

TrailingStopLoss::TrailingStopLoss(double percentage) : CloseOrder(percentage) {}

void TrailingStopLoss::setPrice(Trade* trade, double& currentPrice) {
    switch(trade->getTradeType()) {
        case LONG: {
            if (currentPrice >= trade->getEntryPrice() * (1 + percentage / 100)) {
                double newStop = currentPrice * (1 - percentage / 100);

                if (price == 0.0 || newStop > price) {
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


void TrailingStopLoss::checkExit(Trade* trade, double close) {    
    if (price > 0.0) {
        if (
            trade->getTradeType() == TradeType::LONG 
            && close <= price 
            && trade->getEntryPrice() < price
        ) {
            trade->closeTrade(price);
            std::cout << "TR SL HIT (LONG). CLOSE PRICE: " <<price << std::endl;
            return;
        } else if (
            trade->getTradeType() == TradeType::SHORT 
            && close >= price
            && trade->getEntryPrice() > price
        ) {
            trade->closeTrade(price);
            std::cout << "TR SL HIT (SHORT). CLOSE PRICE: " << price << std::endl;
            return;
        }
    }
}