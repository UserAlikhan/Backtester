#include "backtester.h"
#include <iostream>
#include <unordered_map>

// constructor
Backtester::Backtester() {};

// destructor
Backtester::~Backtester() {
    for (auto t : trades) {
        delete t;
    }

    for (auto sl: stopLosses) {
        delete sl;
    }
}

void Backtester::addTrade(Trade* trade) {
    trades.push_back(trade);
}

void Backtester::addStopLoss(StopLoss* stopLoss) {
    stopLosses.push_back(stopLoss);
}

void resetTriggerStatus(
    std::vector<Indicator*>& indicators, 
    std::unordered_map<std::string, bool>& indicatorTriggerStatus
) {
    for (auto& indicator : indicators) {
        indicatorTriggerStatus[indicator->getName()] = false;
    }
}

bool checkTradeSignal(std::unordered_map<std::string, bool>& indicatorTriggerStatus) {
    bool confirmation = true;

    if (indicatorTriggerStatus.size() == 1) return confirmation;

    for (const auto& status : indicatorTriggerStatus) {
        if (status.second != true) {
            confirmation = false;
            break;
        }
    }
    
    return confirmation;
}

void Backtester::run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators) {
    // USER PROMPTS
    double maxStopLoss, trailingStopLoss;
    std::string trailingStopLossAnswer;

    std::cout << "Speficy maximum stop loss: " << std::endl;
    std::cin >> maxStopLoss;

    std::cout << "Do you need trailing stop loss? (Yes / No)" << std::endl;
    std::cin >> trailingStopLossAnswer;

    if (trailingStopLossAnswer == "Yes") {
        std::cout << "Specify trailing stop: " << std::endl;
        std::cin >> trailingStopLossAnswer;
    }

    // BACKTEST RUNNER
    std::unordered_map<std::string, bool> indicatorTriggerStatus;
    std::unordered_map<std::string, std::vector<std::pair<IntersectionEnum, int>>> intersections;

    for (auto indicator: indicators) {
        indicator->calculate(candles);
        // fill hashmap with indicator name
        indicatorTriggerStatus[indicator->getName()] = false;

        if (IntersectionIndicator* inter = dynamic_cast<IntersectionIndicator*>(indicator)) {
            // save each indicator's intesections (trade entry points)
            intersections[indicator->getName()] = inter->findIntersections();
        }
    }

    // iterate over intersections 
    for (const auto& pair : intersections) {
        const std::string& indicatorName = pair.first;
        const auto& indicatorSignals = pair.second;

        std::cout << "Indicator: " << indicatorName << std::endl;
        std::cout << "Num of trades: " << indicatorSignals.size() << std::endl;
    
        // iterate over first trade's signals
        for (const auto& signal : indicatorSignals) {
            indicatorTriggerStatus[indicatorName] = true;

            IntersectionEnum type = signal.first;
            int index = signal.second;
            
            // run over other indicators and check if there are trade signals at index
            for (const auto& otherPair: intersections) {
                const std::string& otherIndicatorName = otherPair.first;
                const auto& otherIndicatorSignals = otherPair.second;
                
                if (otherIndicatorName == indicatorName) continue;

                for (const auto& otherIndicatorSignal: otherIndicatorSignals) {
                    if (otherIndicatorSignal.second == index) {
                        indicatorTriggerStatus[otherPair.first] = true;
                    }
                }

                // if there is no such intersection in other indicators, break and go to new trade signal
                if (indicatorTriggerStatus[otherIndicatorName] == false) {
                    // reset the trigger status
                    resetTriggerStatus(indicators, indicatorTriggerStatus);
                    break;
                }
            }
        }

        // if there is trade signal in all indicators execute trade
        bool confirmation = checkTradeSignal(indicatorTriggerStatus);
        if (confirmation) {
            // Trade* trade;
            // addTrade(trade);
        }
    }
}