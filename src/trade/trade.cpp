#include "trade.h"
#include <iostream>

Trade::Trade(
    TradeType tradeType, double transactionAmount, double entryPrice
) : tradeType(tradeType), transactionAmount(transactionAmount),
entryPrice(entryPrice) {};

void Trade::closeTrade(double closePrice) {
    this->closePrice = closePrice;
}

double Trade::calculateLiquidationPrice(double* currentBalance) {
    double liquidationPrice;

    switch(tradeType) {
        case LONG: {
            liquidationPrice = entryPrice - (*currentBalance / transactionAmount);
        }
        case SHORT: {
            liquidationPrice = entryPrice + (*currentBalance / transactionAmount);
        }
    }

    return liquidationPrice;
}

void Trade::calculatePL(double* currentBalance) {
    switch(tradeType) {
        case LONG: {
            PL = transactionAmount * (closePrice - entryPrice);
            PLpercentage = ((closePrice - entryPrice) / entryPrice) * 100;

            if (PL <= -(*currentBalance)) {
                // check liquidation price if loss exceeds the balance
                double liquidationPrice = calculateLiquidationPrice(currentBalance);
                this->closePrice = liquidationPrice;
                PL = transactionAmount * (closePrice - entryPrice);
                PLpercentage = -100;
                *currentBalance = 0;
                std::cout << "LONG trade liquidated at: " << liquidationPrice << std::endl;
                return;
            }
            
            *currentBalance += PL;
            break;
        }
        case SHORT: {
            PL = transactionAmount * (entryPrice - closePrice);
            PLpercentage = ((entryPrice - closePrice) / entryPrice) * 100;

            // check liquidation price if loss exceeds the balance
            if (PL < -(*currentBalance)) {
                double liquidationPrice = calculateLiquidationPrice(currentBalance);
                this->closePrice = liquidationPrice;
                PL = transactionAmount * (entryPrice - closePrice);
                PLpercentage = -100;
                *currentBalance = 0;
                std::cout << "SHORT trade liquidated at: " << liquidationPrice << std::endl;
                return;
            }

            *currentBalance += PL;
            break;
        }
    }
}