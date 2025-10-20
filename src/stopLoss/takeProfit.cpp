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

void TakeProfit::checkExit(
    Trade* trade, double close, CloseOrder* takePr, 
    std::vector<Trade*> trades, std::vector<CloseOrder*> takeProfits
) {
    // if price is bigger or the same as take profit close the profitable long trade
    if (trade->getTradeType() == TradeType::LONG && price <= close) {
        trade->closeTrade(price);
        // push into arrays
        trades.push_back(trade);
        takeProfits.push_back(takePr);
        std::cout << "TAKE PROFIT PRICE " << price << std::endl;
        return;
    // if price is lower or the same as take profit close the profitable short trade
    } else if (trade->getTradeType() == TradeType::SHORT && price >= close) {
        trade->closeTrade(price);
        // push into arrays
        trades.push_back(trade);
        takeProfits.push_back(takePr);
        std::cout << "TAKE PROFIT PRICE " << price << std::endl;
        return;
    } 
}