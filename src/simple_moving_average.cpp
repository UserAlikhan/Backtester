#include <simple_moving_average.h>
#include <iostream>
#include <vector>

SMA::SMA(const std::vector<Candle>& candleData, int period1, int period2) 
    : candleData(candleData), period1(period1), period2(period2) {}

std::vector<double> SMA::calculate() {
    std::vector<double> result;
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
        if (sma1.size() > 1 && sma2.size() > 2) {
            double prevSma1 = sma1[sma1.size() - 2];
            double prevSma2 = sma2[sma2.size() - 2];

            // Death cross
            if (prevSma1 > prevSma2 && sma1Value < sma2Value) {
                if (!isInDeathCross) {
                    std::cout << "DATE: " << candleData[i].timestamp 
                        <<  " 50 SMA was: " << prevSma1 
                        << " 200 SMA was: " << prevSma2
                        << " Now 50 SMA is: " << sma1Value
                        << " Now 200 SMA is: " << sma2Value
                        << " IT IS A DEATH CROSS "
                        << std::endl;

                    // if we have an open trade (golden cross / LONG) close it
                    if (entryPrice != 0.0) {
                        closePrice = candleData[i].close;

                        // execute the trade
                        Trade trade(TradeType::LONG, 500, entryPrice, closePrice);
                        double result = trade.result();
                        balance += result;
                        std::cout << "Death Cross Trade result: " << result << std::endl;

                        // clean the values
                        entryPrice = 0.0;
                        closePrice = 0.0;
                    }

                    // execute new trade since it is a death cross
                    entryPrice = candleData[i].close;
                    isInDeathCross = true;
                    isInGoldenCross = false;
                }
            // Golden cross
            } else if (prevSma1 < prevSma2 && sma1Value > sma2Value) {
                if (!isInGoldenCross) {
                    std::cout << "DATE: " << candleData[i].timestamp 
                        << " 50 SMA was: " << prevSma1
                        << " 200 SMA was: " << prevSma2
                        << " Now 50 SMA is: " << sma1Value
                        << " Now 200 SMA is: " << sma2Value
                        << " IT IS A GOLDEN CROSS "
                        << std::endl;

                    // if we have an open trade (Death Cross / SHORT) close it
                    if (entryPrice != 0.0) {
                        closePrice = candleData[i].close;

                        // execute the trade
                        Trade trade(TradeType::SHORT, 500, entryPrice, closePrice);
                        double result = trade.result();
                        balance += result;
                        std::cout << "Golden Cross Trade result: " << result << std::endl;

                        // clean the values
                        entryPrice = 0.0;
                        closePrice = 0.0;
                    }

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