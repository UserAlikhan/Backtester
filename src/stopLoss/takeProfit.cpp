#include "takeProfit.h"
#include <iostream>

TakeProfit::TakeProfit(double percentage) : CloseOrder(percentage) {};

void TakeProfit::setPrice(Trade* trade, double& entryPrice) {
    switch (trade->getTradeType()) {
        case LONG: {
            price = entryPrice * (1 + percentage / 100);
            break;
        }
        case SHORT: {
            price = entryPrice * (1 - percentage / 100);
            break;
        }
    }
}

void TakeProfit::checkExit(Trade* trade, double close) {
    if (trade->getClosePrice() != 0.0) return;
    
    // if price is bigger or the same as take profit close the profitable long trade
    if (trade->getTradeType() == TradeType::LONG && price <= close) {
        trade->closeTrade(price);
        std::cout << "TAKE PROFIT PRICE " << price << std::endl;
        return;
    // if price is lower or the same as take profit close the profitable short trade
    } else if (trade->getTradeType() == TradeType::SHORT && price >= close) {
        trade->closeTrade(price);
        std::cout << "TAKE PROFIT PRICE " << price << std::endl;
        return;
    } 
}