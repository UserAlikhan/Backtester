#include "trade.h"
#include <iostream>

Trade::Trade(
    TradeType tradeType, double currentBalance,
    double transactionAmount, double entryPrice, double closePrice
) : tradeType(tradeType), currentBalance(currentBalance), 
transactionAmount(transactionAmount), entryPrice(entryPrice), 
closePrice(closePrice) {}

double Trade::result() {
    if (transactionAmount < 0) {
        std::cout << "Transaction amount must be greater than 0.0" << std::endl;
        return 0.0;
    }

    double quantity = transactionAmount / entryPrice;
    double result = 0.0;

    switch (tradeType) {
        case LONG: {
            result = (closePrice - entryPrice) * quantity;
            break;
        }
        case SHORT: {
            result = (entryPrice - closePrice) * quantity;
            break;
        }
    }

    double newBalance = currentBalance + result;
            
    if (newBalance < 0) {
        newBalance = 0.0;
    }
    
    return newBalance;
}