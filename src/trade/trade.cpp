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

// Trade::Trade(
//     TradeType tradeType, double currentBalance,
//     double transactionAmount, double entryPrice, double closePrice,
//     double trailingStopPercentage
// ) : tradeType(tradeType), currentBalance(currentBalance), 
// transactionAmount(transactionAmount), entryPrice(entryPrice), 
// closePrice(closePrice), trailingStopPercentage(trailingStopPercentage) {}

// double Trade::result() {
//     if (transactionAmount < 0) {
//         std::cout << "Transaction amount must be greater than 0.0" << std::endl;
//         return 0.0;
//     }

//     double quantity = transactionAmount / entryPrice;
//     double result = 0.0;

//     switch (tradeType) {
//         case LONG: {
//             result = (closePrice - entryPrice) * quantity;
//             break;
//         }
//         case SHORT: {
//             result = (entryPrice - closePrice) * quantity;
//             break;
//         }
//     }

//     double newBalance = currentBalance + result;
            
//     if (newBalance < 0) {
//         newBalance = 0.0;
//     }
    
//     return newBalance;
// }

// void Trade::trackStopLossSMA(
//     bool& isInDeathCross, bool& isInGoldenCross, TradeType tradeType,
//     double& entryPrice, double& closePrice, double close, 
//     double& balance, std::vector<TradeResult>& tradeResults,
//     int maxLossPercentage
// ) {
//     if ((isInDeathCross || isInGoldenCross) && entryPrice != 0.0 && closePrice == 0.0) {
//         double difference = close - entryPrice;
//         // DEATH CROSS, we have loss when price goes up
//         if ((tradeType == SHORT && difference > 0.0) || (tradeType == LONG && difference < 0.0)) {
//             // if have loss case
//             double currentLoss = (std::abs(difference) / entryPrice) * 100;
//             // std::cout << "currentLoss: " << currentLoss << std::endl;
//             if (currentLoss >= maxLossPercentage) {
//                 // close the trade
//                 Trade trade(tradeType, balance, balance * 0.35, entryPrice, close, 0.0);
//                 double newBalance = trade.result();
//                 // save trade result (gain / loss)
//                 tradeResults.push_back({newBalance - balance});

//                 if (tradeType == SHORT) {
//                     std::cout << "\nDeath Cross Trade WAS CLOSED BY STOP LOSS. %: " << currentLoss << " $: " << newBalance - balance << std::endl;
//                 } else if (tradeType == LONG) {
//                     std::cout << "\nGolden Cross Trade WAS CLOSED BY STOP LOSS. %: " << currentLoss << " $: " << newBalance - balance << std::endl;
//                 }

//                 // balance cannot be less than 0
//                 balance = (newBalance > 0) ? newBalance : 0.0;

//                 entryPrice = 0.0;
//                 closePrice = 0.0;
//                 isInDeathCross = false;
//                 isInGoldenCross = false;
//             }
//         }
//     }
// }

// void Trade::closeTrade(
//     double& entryPrice, double closePrice, TradeType tradeType, 
//     double& balance, std::vector<TradeResult>& tradeResults
// ) {
//     if (entryPrice != 0.0) {
//         // close the trade
//         Trade trade(tradeType, balance, balance * 0.35, entryPrice, closePrice, 0.0);
//         double newBalance = trade.result();
//         // save trade result
//         tradeResults.push_back({newBalance - balance});
        
//         if (tradeType == LONG) {
//             std::cout << "\nGolden Cross Trade result: " << newBalance - balance << std::endl;
//         } else if (tradeType == SHORT) {
//             std::cout << "\nDeath Cross Trade result: " << newBalance - balance << std::endl;
//         }
        
//         // balance cannot be less that 0
//         balance = (newBalance > 0) ? newBalance : 0.0;

//         // clean the values
//         entryPrice = 0.0;
//         closePrice = 0.0;
//     }
// }

// void Trade::trackTrailingStop(
//     bool& isInDeathCross, bool&isInGoldenCross, Trade& trade, double& entryPrice, 
//     double& closePrice, double currentPrice, TradeType tradeType, 
//     double& balance, std::vector<TradeResult>& tradeResults
// ) {
//     std::cout << "Trade object address: " << &trade << std::endl;

//     std::cout << "INSIDE TR STOP" << std::endl;

//     if (entryPrice == 0.0) {
//         return;
//     }

//     switch(tradeType) {
//         case LONG: {
//             if (trade.getTrailingStopInitialPrice() == 0.0) {
//                 double stopLossPrice = entryPrice * (1 - trade.getTrailingStopPercentage() / 100);
//                 trade.setTrailingStopInitialPrice(stopLossPrice);
//                 trade.setTranilingStopLastPrice(stopLossPrice);
//             }

//             // check if price went up
//             double difference = currentPrice - entryPrice;

//             // if price moved up, change trailing stop loss position
//             if (difference > 0.0) {
//                 double newStopLoss = currentPrice * (1 - trade.getTrailingStopPercentage() / 100);
//                 std::cout << "LONG old stop loss: " << trade.getTranilingStopLastPrice() << "newStopLoss: " << newStopLoss << std::endl;

//                 // if new stop loss is less than trailing stop loss, then close the trade
//                 if (newStopLoss <= trade.getTranilingStopLastPrice() + 1000) {
//                     trade.closeTrade(entryPrice, newStopLoss, TradeType::LONG, balance, tradeResults);
//                     std::cout << "LONG Trade was closed using trailing stop. NewStopLoss: " << newStopLoss
//                         << " Last Stop loss: " << trade.getTranilingStopLastPrice()
//                         << " Entry Price: " << entryPrice
//                         << std::endl;

//                     // reset variables
//                     entryPrice = 0.0;
//                     isInDeathCross = false;
//                     isInGoldenCross = false;

//                 } else {
//                     trade.setTranilingStopLastPrice(newStopLoss);
//                 }
//             }
//             break;
//         }
//         case SHORT: {
//             if (trade.getTrailingStopInitialPrice() == 0.0) {
//                 double stopLossPrice = entryPrice * (1 + trade.getTrailingStopPercentage() / 100);
//                 trade.setTrailingStopInitialPrice(stopLossPrice);
//                 trade.setTranilingStopLastPrice(stopLossPrice);
//             }
            
//             // check if price went down
//             double difference = entryPrice - currentPrice;

//             // move trailing stop loss if price moved up
//             if (difference > 0.0) {
//                 double newStopLoss = currentPrice * (1 + trade.getTrailingStopPercentage() / 100);

//                 // if new stop loss 
//                 if (newStopLoss >= trade.getTranilingStopLastPrice()) {
//                     trade.closeTrade(entryPrice, newStopLoss, TradeType::SHORT, balance, tradeResults);
//                     std::cout << "SHORT Trade was closed using trailing stop. NewStopLoss: " << newStopLoss
//                         << " Last Stop loss: " << trade.getTranilingStopLastPrice()
//                         << " Entry Price: " << entryPrice
//                         << std::endl;
//                     // reset variables
//                     entryPrice = 0.0;
//                     isInDeathCross = false;
//                     isInGoldenCross = false;

//                 } else {
//                     trade.setTranilingStopLastPrice(newStopLoss);
//                 }
//             }
//             break;
//         }
//     }
// }