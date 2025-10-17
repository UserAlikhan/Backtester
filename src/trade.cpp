#include "trade.h"
#include <iostream>

Trade::Trade(
    TradeType tradeType, double transactionAmount, 
    double entryPrice, double closePrice
) : tradeType(tradeType), transactionAmount(transactionAmount), 
entryPrice(entryPrice), closePrice(closePrice) {}

double Trade::result() {
    switch (tradeType) {
        case LONG: {
            double result = (closePrice - entryPrice) * transactionAmount;
            return result;
        }
        case SHORT: {
            double result = (entryPrice - closePrice) * transactionAmount;
            return result;
        }
    }
    
    return 0.0;
}