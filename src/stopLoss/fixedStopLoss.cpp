#include "fixedStopLoss.h"
#include <iostream>

FixedStopLoss::FixedStopLoss(double percentage) : StopLoss(percentage) {};

void FixedStopLoss::setPrice(Trade* trade, double& entryPrice) {
    switch(trade->getTradeType()) {
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

void FixedStopLoss::checkExit(Trade* trade, int index, std::vector<Candle*>& candles) {
    for (size_t i = index; i < candles.size(); i++) {
        // if price is bigger than stop loss close the long trade
        if (trade->getTradeType() == TradeType::LONG && price > candles[i]->close) {
            std::cout << "LONG !!! ENTRY PRICE: " << trade->getEntryPrice() 
                << " STOP LOSS: " << price << " CLOSED: " << candles[i]->close << std::endl;
            trade->closeTrade(candles[i]->close);
            return;
        // if price is lower than stop loss close the short trade
        } else if (trade->getTradeType() == TradeType::SHORT && price < candles[i]->close) {
            std::cout << "SHORT !!! ENTRY PRICE: " << trade->getEntryPrice() 
                << " STOP LOSS: " << price << " CLOSED: " << candles[i]->close << std::endl;
            trade->closeTrade(candles[i]->close);
            return;
        }    
    }
}