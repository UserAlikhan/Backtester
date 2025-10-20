#include "fixedStopLoss.h"
#include <iostream>

FixedStopLoss::FixedStopLoss(double percentage) : StopLoss(percentage) {};

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

void FixedStopLoss::checkExit(Trade* trade, int index, std::vector<Candle*>& candles) {
    for (size_t i = index; i < candles.size(); i++) {
        // if price is bigger than stop loss close the long trade
        if (trade->getTradeType() == TradeType::LONG && price >= candles[i]->close) {
            trade->closeTrade(candles[i]->close);
            std::cout << "CLOSE PRICE " << candles[i]->close << std::endl;
            return;
        // if price is lower than stop loss close the short trade
        } else if (trade->getTradeType() == TradeType::SHORT && price <= candles[i]->close) {
            trade->closeTrade(candles[i]->close);
            std::cout << "CLOSE PRICE " << candles[i]->close << std::endl;
            return;
        // if we reached the end of the dataset and we do not have close price, 
        // close the trade using the last datapoint
        } else if (i == candles.size() - 1 && trade->getClosePrice() == 0.0) {
            trade->closeTrade(candles[i]->close);
        }
    }
}