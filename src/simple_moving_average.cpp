#include <simple_moving_average.h>
#include <iostream>
#include <vector>

SMA::SMA(const std::vector<Candle>& candleData, int period1, int period2) 
    : candleData(candleData), period1(period1), period2(period2) {}

std::vector<double> SMA::calculate() {
    std::vector<double> result;
    double balance = 5000.0;
    int maxLossPercentage = 25;

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
            double prevSma1 = sma1[sma1.size() - 2];
            double prevSma2 = sma2[sma2.size() - 2];

            // when we open a trade we have to track price movements, because we have a stop loss
            if (isInDeathCross && entryPrice != 0.0 && closePrice == 0.0) {
                double difference = candleData[i].close - entryPrice;
                // DEATH CROSS, we have loss when price goes up
                if (difference > 0.0) {
                    // if have loss case
                    double currentLoss = (std::abs(difference) / entryPrice) * 100;
                    // std::cout << "currentLoss: " << currentLoss << std::endl;
                    if (currentLoss >= maxLossPercentage) {
                        // close the trade
                        Trade trade(TradeType::SHORT, balance, balance * 0.35, entryPrice, candleData[i].close);
                        double newBalance = trade.result();

                        std::cout << "\nDeath Cross Trade WAS CLOSED BY STOP LOSS: %: " << currentLoss << " $: " << newBalance - balance << std::endl;
                        balance = newBalance;

                        entryPrice = 0.0;
                        closePrice = 0.0;
                        isInDeathCross = false;
                        isInGoldenCross = false;
                    }
                }
            // GOLDEN CROSS: LONG TRADE
            } else if (isInGoldenCross && entryPrice != 0.0 && closePrice == 0.0) {
                double difference = candleData[i].close - entryPrice;
                // GOLDEN CROSS, we have loss when price goes down
                if (difference < 0.0) {
                    // if have loss case
                    double currentLoss = (std::abs(difference) / entryPrice) * 100.0;
                    // std::cout << "currentLoss: " << currentLoss << std::endl;
                    if (currentLoss >= maxLossPercentage) {
                        // close the trade
                        Trade trade(TradeType::LONG, balance, balance * 0.35, entryPrice, candleData[i].close);
                        double newBalance = trade.result();

                        std::cout << "\nGolden Cross Trade WAS CLOSED BY STOP LOSS: %: " << currentLoss << " $: " << newBalance - balance << std::endl;
                        balance = newBalance;

                        entryPrice = 0.0;
                        closePrice = 0.0;
                        isInDeathCross = false;
                        isInGoldenCross = false;
                    }
                }
            }

            // Death cross
            if (prevSma1 > prevSma2 && sma1Value < sma2Value) {
                // if we don't have an open trade and it is death cross, open one
                if (!isInDeathCross) {
                    // if we have an open trade (golden cross / LONG) close it
                    if (entryPrice != 0.0) {
                        closePrice = candleData[i].close;

                        // execute the trade
                        Trade trade(TradeType::LONG, balance, balance * 0.35, entryPrice, closePrice);
                        double newBalance = trade.result();
                        
                        std::cout << "\nGolden Cross Trade result: " << newBalance - balance << std::endl;
                        balance = newBalance;
                        if (balance <= 0.0) {
                            break;
                        }

                        // clean the values
                        entryPrice = 0.0;
                        closePrice = 0.0;
                    }

                    std::cout << "DATE: " << candleData[i].timestamp 
                        <<  " 50 SMA was: " << prevSma1 
                        << " 200 SMA was: " << prevSma2
                        << " Now 50 SMA is: " << sma1Value
                        << " Now 200 SMA is: " << sma2Value
                        << " IT IS A DEATH CROSS "
                        << std::endl;

                    // execute new trade since it is a death cross
                    entryPrice = candleData[i].close;
                    isInDeathCross = true;
                    isInGoldenCross = false;
                }
            // Golden cross
            } else if (prevSma1 < prevSma2 && sma1Value > sma2Value) { 
                // if we don't have an open trade and it is golden cross, open one
                if (!isInGoldenCross) {
                    // if we have an open trade (Death Cross / SHORT) close it
                    if (entryPrice != 0.0) {
                        closePrice = candleData[i].close;

                        // close the trade
                        Trade trade(TradeType::SHORT, balance, balance * 0.35, entryPrice, closePrice);
                        double newBalance = trade.result();
                        
                        std::cout << "\nDeath Cross Trade result: " << newBalance - balance << std::endl;
                        balance = newBalance;
                        if (balance <= 0.0) {
                            break;
                        }

                        // clean the values
                        entryPrice = 0.0;
                        closePrice = 0.0;
                    }

                    std::cout << "DATE: " << candleData[i].timestamp 
                        << " 50 SMA was: " << prevSma1
                        << " 200 SMA was: " << prevSma2
                        << " Now 50 SMA is: " << sma1Value
                        << " Now 200 SMA is: " << sma2Value
                        << " IT IS A GOLDEN CROSS "
                        << std::endl;

                    // execute new trade since it is a death cross
                    entryPrice = candleData[i].close;
                    isInDeathCross = false;
                    isInGoldenCross = true;
                }
            }
        }
    }
    std::cout << "TRADE RESULTS: " << balance << std::endl;
    return result;
}