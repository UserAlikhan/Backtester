#include <statistic.h>
#include <iostream>

Statistic::Statistic(
    int totalNumberOfTrades, int numOfProfitableTrades, 
    int numOfLossTrades, int gainAmount
) : totalNumberOfTrades(totalNumberOfTrades), 
numOfProfitableTrades(numOfProfitableTrades),
numOfLossTrades(numOfLossTrades),
gainAmount(gainAmount) {}

Statistic Statistic::calculate(std::vector<TradeResult>& tradeResults) {
    int totalNumOfTrades = tradeResults.size();
    int numOfProfitableTrades = 0, numOfLossTrades = 0, gainAmount = 0;

    for (int i = 0; i < totalNumOfTrades; i++) {
        gainAmount += tradeResults[i].yield;
        if (tradeResults[i].yield > 0) {
            numOfProfitableTrades++;
        } else {
            numOfLossTrades++;
        }
    }

    return {
        totalNumOfTrades, numOfProfitableTrades, 
        numOfLossTrades, gainAmount
    };
}

void Statistic::print() {
    std::cout << "Statistics: " << std::endl;
    std::cout << "totalNumOfTrades: " << totalNumberOfTrades 
        << "\nnumOfProfitableTrades: "  << numOfProfitableTrades
        << "\nnumOfLossTrades: "  << numOfLossTrades
        << "\ngainAmount: "  << gainAmount
        << std::endl;
}