#include <simple_moving_average.h>
#include <iostream>
#include <vector>

SMA::SMA(const std::vector<Candle>& candleData, int period1, int period2) 
    : candleData(candleData), period1(period1), period2(period2) {}

std::vector<double> SMA::calculate(double maxStopLossPercentage, double trailingStopPercentage = 0.0) {
    std::vector<double> result;
    std::vector<TradeResult> tradeResults;
    double balance = 5000.0;

    if (candleData.size() < std::max(period1, period2)) {
        return result;
    }

    double sumPeriod1 = 0.0, sumPeriod2 = 0.0;
 
    // initialize sums
    for (int i = 0; i < period1; ++i) {
        sumPeriod1 += candleData[i].close;
    }

    for (int i = 0; i < period2; ++i) {
        sumPeriod2 += candleData[i].close;
    }

    std::vector<double> sma1, sma2;
    bool isInDeathCross = false, isInGoldenCross = false;
    double entryPrice = 0.0, closePrice = 0.0;

    // move forward and calculate SMA
    for (int i = std::max(period1, period2); i < candleData.size(); ++i) {
        sumPeriod1 += candleData[i].close - candleData[i - period1].close;
        sumPeriod2 += candleData[i].close - candleData[i - period2].close;
        
        double sma1Value = sumPeriod1 / period1;
        double sma2Value = sumPeriod2 / period2;

        sma1.push_back(sma1Value);
        sma2.push_back(sma2Value);

        // Check if lines crossed each other
        if (sma1.size() > 1 && sma2.size() > 1) {
            // when we open a trade we have to track price movements, because we have a stop loss
            TradeType tradeType;
            if (isInDeathCross) tradeType = TradeType::SHORT;
            else if (isInGoldenCross) tradeType = TradeType::LONG;
            
            // if we have an open trade and trailing stop specified
            // if (trailingStopPercentage != 0.0) {
            //     std::cout << "TR STOP" << std::endl;
            //     Trade trade(tradeType, balance, balance * 0.35, entryPrice, 0.0, trailingStopPercentage);
            //     trade.trackTrailingStop(isInDeathCross, isInGoldenCross, trade, entryPrice, closePrice, 
            //         candleData[i].close, tradeType, balance, tradeResults);
            // }

            Trade::trackStopLossSMA(isInDeathCross, isInGoldenCross, 
                tradeType, entryPrice, closePrice, candleData[i].close, balance,
                tradeResults, maxStopLossPercentage
            );

            double prevSma1 = sma1[sma1.size() - 2];
            double prevSma2 = sma2[sma2.size() - 2];
            
            // Death cross
            if (prevSma1 > prevSma2 && sma1Value < sma2Value) {
                // if we don't have an open death cross trade, open one
                if (!isInDeathCross) {
                    // if we have an open trade (golden cross / LONG) close it
                    Trade::closeTrade(
                        entryPrice, candleData[i].close, TradeType::LONG, 
                        balance, tradeResults
                    );

                    // open new Death cross trade
                    std::cout << "DATE: " << candleData[i].timestamp 
                        <<  " 50 SMA was: " << prevSma1 
                        << " 200 SMA was: " << prevSma2
                        << " Now 50 SMA is: " << sma1Value
                        << " Now 200 SMA is: " << sma2Value
                        << " IT IS A DEATH CROSS "
                        << std::endl;

                    entryPrice = candleData[i].close;
                    isInDeathCross = true;
                    isInGoldenCross = false;
                }
            // Golden cross
            } else if (prevSma1 < prevSma2 && sma1Value > sma2Value) { 
                // if we don't have an open trade and it is golden cross, open one
                if (!isInGoldenCross) {
                    // if we have an open trade (Death Cross / SHORT) close it
                    Trade::closeTrade(
                        entryPrice, candleData[i].close, TradeType::SHORT, 
                        balance, tradeResults
                    );

                    // open new golden cross trade
                    std::cout << "DATE: " << candleData[i].timestamp 
                        << " 50 SMA was: " << prevSma1
                        << " 200 SMA was: " << prevSma2
                        << " Now 50 SMA is: " << sma1Value
                        << " Now 200 SMA is: " << sma2Value
                        << " IT IS A GOLDEN CROSS "
                        << std::endl;

                    entryPrice = candleData[i].close;
                    isInDeathCross = false;
                    isInGoldenCross = true;
                }
            }
        }
    }

    std::cout << "Result: " << balance << std::endl;

    // calculate and print statistics
    Statistic stat = Statistic::calculate(tradeResults);
    stat.print();

    return result;
}