#include "backtester.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

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
    bool confirmation = false;

    if (indicatorTriggerStatus.size() == 1) return true;

    for (const auto& status : indicatorTriggerStatus) {
        if (status.second == true) {
            confirmation = true;
        } else {
            confirmation = false;
        }
    }
    
    return confirmation;
}

void ascendingSortIntersections(
    std::vector<std::pair<
        std::string, 
        std::vector<std::pair<TradeType, int>>
    >>& intersections
) {
    std::sort(intersections.begin(), intersections.end(),
        [](const auto& a, const auto& b) {
            return a.second.size() < b.second.size();
        }  
    );
}

void Backtester::run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators, double* balance) {
    // USER PROMPTS
    double maxStopLoss, trailingStopLoss, shareOfBalance;
    std::string trailingStopLossAnswer;

    std::cout << "Speficy maximum stop loss: " << std::endl;
    std::cin >> maxStopLoss;

    std::cout << "Do you need trailing stop loss? (Yes / No)" << std::endl;
    std::cin >> trailingStopLossAnswer;

    if (trailingStopLossAnswer == "Yes") {
        std::cout << "Specify trailing stop: " << std::endl;
        std::cin >> trailingStopLoss;
    }

    std::cout << "Enter a share of balance you want to use for trades (%): " << std::endl;
    std::cin >> shareOfBalance;

    // BACKTEST RUNNER
    std::unordered_map<std::string, bool> indicatorTriggerStatus;
    std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>> intersections;

    for (auto indicator: indicators) {
        indicator->calculate(candles);
        // fill hashmap with indicator name
        indicatorTriggerStatus[indicator->getName()] = false;

        if (IntersectionIndicator* inter = dynamic_cast<IntersectionIndicator*>(indicator)) {
            // save each indicator's intesections (trade entry points)
            intersections.push_back({indicator->getName(), inter->findIntersections()});
        }
    }

    // sort intersections
    ascendingSortIntersections(intersections);

    // iterate over intersections 
    for (const auto& pair : intersections) {
        const std::string& indicatorName = pair.first;
        const auto& indicatorSignals = pair.second;

        std::cout << "Indicator: " << indicatorName << std::endl;
        std::cout << "Num of trades: " << indicatorSignals.size() << std::endl;
    
        // iterate over MAIN trade's signals
        for (const auto& signal : indicatorSignals) {
            indicatorTriggerStatus[indicatorName] = true;

            TradeType type = signal.first;
            int index = signal.second;
            
            // run over OTHER indicators and check if there are trade signals at index
            for (const auto& otherPair: intersections) {
                const std::string& otherIndicatorName = otherPair.first;
                const auto& otherIndicatorSignals = otherPair.second;
                
                if (otherIndicatorName == indicatorName) continue;

                for (const auto& otherIndicatorSignal: otherIndicatorSignals) {
                    // checks price index to be the same and trade type to be the same
                    if (otherIndicatorSignal.second == index && otherIndicatorSignal.first == signal.first) {
                        indicatorTriggerStatus[otherPair.first] = true;
                        break;
                    }
                }

                // if there is no such intersection in other indicators, break and go to new trade signal
                if (indicatorTriggerStatus[otherIndicatorName] == false) {
                    // reset the trigger status
                    resetTriggerStatus(indicators, indicatorTriggerStatus);
                    break;
                }
            }

            // if there is trade signal in all indicators execute trade
            bool confirmation = checkTradeSignal(indicatorTriggerStatus);
            if (confirmation) {
                std::cout << "TRADE SIGNAL TYPE: " << type << " index: "<<  index << std::endl;

                // open a new trade
                std::cout << "Tradetype: " << signal.first << " entry price: " << candles[signal.second]->close << " share of balance: " << *balance * shareOfBalance / 100 << std::endl;
                Trade* trade = new Trade(signal.first, *balance * shareOfBalance / 100, candles[signal.second]->close);
                addTrade(trade);

                // set a new stop loss
                FixedStopLoss* fixedStopLoss = new FixedStopLoss(maxStopLoss);
                fixedStopLoss->setPrice(trade, candles[index]->close);
                addStopLoss(fixedStopLoss);

                std::cout << "DATA SIZE " << candles.size() << " INDEX " << index << std::endl;
                fixedStopLoss->checkExit(trade, index, candles);
                trade->calculatePL();

                std::cout << "TRADE AT INDEX " <<  index  << " WAS CLOSED. Profit: " << trade->getPL()  << "\n" << std::endl;
                trade->recalculateBalance(balance);
            }

            resetTriggerStatus(indicators, indicatorTriggerStatus);
        }

        // we do not need to run further. since, at this points we have all
        // the trade signals
        break;
    }

    std::cout << "Number of trades: " << trades.size() << std::endl;
    std::cout << "Balance: " << *balance << std::endl;
}