#pragma once
#include <iostream>
#include "tradeTypeEnum.h"

class Trade {
    private:
        TradeType tradeType;
        double transactionAmount;
        double entryPrice;
        double closePrice;
        double PL;
        double PLpercentage;
    
    public:
        // constructor
        Trade(TradeType tradeType, double transactionAmount, double entryPrice);

        TradeType getTradeType() { return tradeType; }
        double getEntryPrice() { return entryPrice; }
        double getClosePrice() { return closePrice; }
        double getPL() { return PL; }

        double calculateLiquidationPrice(double *currentBalance);
        void closeTrade(double closePrice);
        void calculatePL(double* currentBalance);
};

// class Trade {
//     private:
//         TradeType tradeType;
//         double currentBalance;
//         double transactionAmount;
//         double entryPrice;
//         double closePrice;
//         double maxLossPercentage;
//         double trailingStopPercentage;
//         double trailingStopInitialPrice = 0.0;
//         double tranilingStopLastPrice = 0.0;
    
//     public:
//         Trade(
//             TradeType tradeType, double currentBalance, double transactionAmount, 
//             double entryPrice, double closePrice, double trailingStopPercentage
//         );

//         double getEntryPrice() {
//             return this->entryPrice;
//         }

//         TradeType getTradeType() {
//             return this->tradeType;
//         }

//         double getTrailingStopPercentage() {
//             return this->trailingStopPercentage;
//         }

//         double getTrailingStopInitialPrice() {
//             return this->trailingStopInitialPrice;
//         };

//         void setTrailingStopInitialPrice(double stopLossPrice) {
//             this->trailingStopInitialPrice = stopLossPrice;
//         };

//         double getTranilingStopLastPrice() {
//             return this->tranilingStopLastPrice;
//         }

//         void setTranilingStopLastPrice(double stopLossPrice) {
//             this->tranilingStopLastPrice = stopLossPrice;
//         }

//         // this method calculates new balance based closed trade
//         double result();

//         // this method closes trades
//         static void closeTrade(
//             double& entryPrice, double closePrice, TradeType tradeType, 
//             double& balance, std::vector<TradeResult>& tradeResults
//         );
        
//         // this method tracks current loss and compare its with allowed max loss
//         // if it is the same or bigger, it closes the trade
//         static void trackStopLossSMA(
//             bool& isInDeathCross, bool& isInGoldenCross, TradeType tradeType,
//             double& entryPrice, double& closePrice, double close, 
//             double& balance, std::vector<TradeResult>& tradeResults,
//             int maxLossPercentage
//         );

//         static void trackTrailingStop(
//             bool& isInDeathCross, bool&isInGoldenCross, Trade& trade, double& entryPrice, 
//             double& closePrice, double currentPrice, TradeType tradeType, 
//             double& balance, std::vector<TradeResult>& tradeResults
//         );
// };