#include "trade.h"
#include <iostream>

Trade::Trade(
    TradeType tradeType, double currentBalance,
    double transactionAmount, double entryPrice, double closePrice,
    double maxLossPercentage
) : tradeType(tradeType), currentBalance(currentBalance), 
transactionAmount(transactionAmount), entryPrice(entryPrice), 
closePrice(closePrice), maxLossPercentage(maxLossPercentage) {}

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

void Trade::trackStopLoss(
    bool& isInDeathCross, bool& isInGoldenCross, TradeType tradeType,
    double& entryPrice, double& closePrice, double close, 
    double& balance, std::vector<TradeResult>& tradeResults,
    int maxLossPercentage
) {
    if ((isInDeathCross || isInGoldenCross) && entryPrice != 0.0 && closePrice == 0.0) {
        double difference = close - entryPrice;
        // DEATH CROSS, we have loss when price goes up
        if ((tradeType == SHORT && difference > 0.0) || (tradeType == LONG && difference < 0.0)) {
            // if have loss case
            double currentLoss = (std::abs(difference) / entryPrice) * 100;
            // std::cout << "currentLoss: " << currentLoss << std::endl;
            if (currentLoss >= maxLossPercentage) {
                // close the trade
                Trade trade(tradeType, balance, balance * 0.35, entryPrice, close, maxLossPercentage);
                double newBalance = trade.result();
                // save trade result (gain / loss)
                tradeResults.push_back({newBalance - balance});

                if (tradeType == SHORT) {
                    std::cout << "\nDeath Cross Trade WAS CLOSED BY STOP LOSS. %: " << currentLoss << " $: " << newBalance - balance << std::endl;
                } else if (tradeType == LONG) {
                    std::cout << "\nGolden Cross Trade WAS CLOSED BY STOP LOSS. %: " << currentLoss << " $: " << newBalance - balance << std::endl;
                }

                // balance cannot be less than 0
                balance = (newBalance > 0) ? newBalance : 0.0;

                entryPrice = 0.0;
                closePrice = 0.0;
                isInDeathCross = false;
                isInGoldenCross = false;
            }
        }
    }
}

void Trade::closeTrade(
    double& entryPrice, double closePrice, TradeType tradeType, 
    double& balance, double maxLossPercentage, 
    std::vector<TradeResult>& tradeResults
) {
    if (entryPrice != 0.0) {
        // close the trade
        Trade trade(tradeType, balance, balance * 0.35, entryPrice, closePrice, maxLossPercentage);
        double newBalance = trade.result();
        // save trade result
        tradeResults.push_back({newBalance - balance});
        
        if (tradeType == LONG) {
            std::cout << "\nGolden Cross Trade result: " << newBalance - balance << std::endl;
        } else if (tradeType == SHORT) {
            std::cout << "\nDeath Cross Trade result: " << newBalance - balance << std::endl;
        }
        
        // balance cannot be less that 0
        balance = (newBalance > 0) ? newBalance : 0.0;

        // clean the values
        entryPrice = 0.0;
        closePrice = 0.0;
    }
}