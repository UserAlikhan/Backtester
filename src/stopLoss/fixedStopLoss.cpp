#include "fixedStopLoss.h"
#include <iostream>

FixedStopLoss::FixedStopLoss(double percentage) : StopLoss(percentage) {};

void FixedStopLoss::setPrice(Trade& trade, double entryPrice) {
    switch(trade.getTradeType()) {
        case LONG: {
            price = entryPrice * (1 - percentage / 100);
            break;
        }
        case SHORT: {
            price = entryPrice * (1 + percentage / 100);
            break;
        }
    }
}

bool FixedStopLoss::checkExit(Trade& trade, double currentPrice) {
    switch (trade.getTradeType()) {
        case LONG: {
            if (price < currentPrice) {
                // TODO: close trade
            }
            break;
        }
        case SHORT: {
            if (price > currentPrice) {
                // close trade
            }
            break;
        }
    }
}