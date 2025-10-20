#include "backtester.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

const double minTradeAmount = 2.0;

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

void Backtester::addStopLoss(CloseOrder* stopLoss) {
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

void Backtester::getUserInputs(double& maxStopLoss, double& trailingStopLoss, double& takeProfit, double& shareOfBalance) {
    std::string trailingStopLossAnswer;

    std::cout << "Specify maximum stop loss: ";
    std::cin >> maxStopLoss;

    std::cout << "Specify take profit: ";
    std::cin >> takeProfit;

    std::cout << "Do you need trailing stop loss? (Yes / No): ";
    std::cin >> trailingStopLossAnswer;

    if (trailingStopLossAnswer == "Yes") {
        std::cout << "Specify trailing stop: ";
        std::cin >> trailingStopLoss;
    }

    std::cout << "Enter a share of balance you want to use for trades (%): ";
    std::cin >> shareOfBalance;
}

void Backtester::prepareIndicators(
    std::vector<Indicator*>& indicators,
    std::vector<Candle*>& candles,
    std::unordered_map<std::string, bool>& indicatorTriggerStatus,
    std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>>& intersections
) {
    for (auto indicator: indicators) {
        indicator->calculate(candles);
        // fill hashmap with indicator name
        indicatorTriggerStatus[indicator->getName()] = false;

        if (IntersectionIndicator* inter = dynamic_cast<IntersectionIndicator*>(indicator)) {
            // save each indicator's intesections (trade entry points)
            intersections.push_back({indicator->getName(), inter->findIntersections()});
        }
    }
}

bool Backtester::confirmSignalAcrossIndicators(
    std::vector<Indicator*>& indicators,
    const std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>>& intersections,
    std::unordered_map<std::string, bool>& indicatorTriggerStatus,
    const std::string& mainIndicator,
    TradeType& type,
    int& index
) {
    // run over indicators and check if there are trade signal matches with LEADING INDICATOR
    for (const auto& otherPair: intersections) {
        const std::string& otherIndicatorName = otherPair.first;
        const auto& otherIndicatorSignals = otherPair.second;
       
        // skip main indicator   
        if (otherIndicatorName == mainIndicator) continue;

        for (const auto& otherIndicatorSignal: otherIndicatorSignals) {
            // checks price index to be the same and trade type to be the same
            if (otherIndicatorSignal.second == index && otherIndicatorSignal.first == type) {
                indicatorTriggerStatus[otherPair.first] = true;
                break;
            }
        }

        // if there are no such intersection in other indicators, break and go to new trade signal
        if (indicatorTriggerStatus[otherIndicatorName] == false) {
            // reset the trigger status
            resetTriggerStatus(indicators, indicatorTriggerStatus);
            break;
        }
    }

    // check hash map, so all indicators have the same trade signal
    return checkTradeSignal(indicatorTriggerStatus);
}

void Backtester::executeTrade(
    std::vector<Candle*>& candles,
    TradeType& type,
    int& index,
    double* balance,
    double& shareOfBalance,
    double& maxStopLoss,
    double& takeProfit,
    double& trailingStopLoss
) {
    // open a new trade
    Trade* trade = new Trade(type, *balance * shareOfBalance / 100, candles[index]->close);
    addTrade(trade);
    std::cout << type << " index: " << index << " trade was opened." << " Entry price: " 
        << trade->getEntryPrice()
        << ". Share of balance: " << *balance * shareOfBalance / 100 
        << std::endl;

    checkAllCloseOrders(candles, trade, index, maxStopLoss, takeProfit, trailingStopLoss);
    trade->calculatePL(balance);

    if (*balance > 0) {
        std::cout << type << " Trade was closed at " << trade->getClosePrice() << ". Profit: " << trade->getPL() << std::endl;
    } else {
        std::cout << type << " Trade liquidated at " << trade->getClosePrice() << ". Profit: " << trade->getPL() << std::endl;
    }
    std::cout << "Current Balance: " << *balance << "\n" << std::endl;
}

void Backtester::processTradeSignals(
    std::vector<Candle*>& candles,
    std::vector<Indicator*>& indicators,
    const std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>>& intersections,
    std::unordered_map<std::string, bool>& indicatorTriggerStatus,
    double* balance,
    double& shareOfBalance,
    double& maxStopLoss,
    double& takeProfit,
    double& trailingStopLoss
) {
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

            // confirm intersection with other indicators
            bool confirmed = confirmSignalAcrossIndicators(
                indicators, intersections, indicatorTriggerStatus, 
                indicatorName, type, index
            );

            // if balance gets to 0 no further trades available
            if (*balance <= minTradeAmount) break;
            else if (confirmed) {
                executeTrade(
                    candles, type, index, balance, 
                    shareOfBalance, maxStopLoss,
                    takeProfit, trailingStopLoss
                );
            }

            resetTriggerStatus(indicators, indicatorTriggerStatus);
        }

        // we do not need to run further. since, at this points we have all
        // the trade signals
        break;
    }
}

void Backtester::checkAllCloseOrders(
    std::vector<Candle*>& candles,
    Trade* trade,
    int& index,
    double& maxStopLoss,
    double& takeProfit,
    double& trailingStopLoss
) { 
    FixedStopLoss* fixedSl = nullptr;
    TrailingStopLoss* tsl = nullptr;
    TakeProfit* tkP = nullptr;

    if (maxStopLoss > 0.0) {
        fixedSl = new FixedStopLoss(maxStopLoss);
        fixedSl->setPrice(trade, candles[index]->close);
    }

    if (takeProfit > 0.0) {
        tkP = new TakeProfit(takeProfit);
        tkP->setPrice(trade, candles[index]->close);
    }

    if (trailingStopLoss > 0.0) {
        tsl = new TrailingStopLoss(trailingStopLoss);
        tsl->setPrice(trade, candles[index]->close);
    }
    // TODO: take profit case

    for (size_t i = index; i < candles.size(); i++) {
        if (trade->getClosePrice() != 0.0) break;
        double close = candles[i]->close;
        tsl->setPrice(trade, close);

        // stop loss case
        fixedSl->checkExit(trade, close);

        // take profit case
        tkP->checkExit(trade, close);

        // trailing stop loss
        tsl->checkExit(trade, close);

        // if we reached the end of the dataset and we do not have close price, 
        // close the trade using the last datapoint
        if (i == candles.size() - 1 && trade->getClosePrice() == 0.0) {
            trade->closeTrade(candles[i]->close);
            std::cout << "End of the dataset. Closed by market (no stop hit): " << candles[i]->close << std::endl;
            return;
        }
    }

    delete fixedSl;
    delete tsl;
}

void Backtester::run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators, double* balance) {
    if (*balance <= minTradeAmount) {
        std::cout << "Balance is less than minimum trade amount. You cannot make trades" << std::endl;
        return;
    }

    // 1. Get all user inputs
    double maxStopLoss, trailingStopLoss, takeProfit, shareOfBalance;
    getUserInputs(maxStopLoss, trailingStopLoss, takeProfit, shareOfBalance);

    // 2. Prepare indicators and intersection signals
    std::unordered_map<std::string, bool> indicatorTriggerStatus;
    std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>> intersections;

    prepareIndicators(indicators, candles, indicatorTriggerStatus, intersections);

    // 3. Sort intersections in ascending order based on number of intersections
    ascendingSortIntersections(intersections);

    // 4. Process signals and run trades
    processTradeSignals(
        candles, indicators, intersections, indicatorTriggerStatus,
        balance, shareOfBalance, maxStopLoss, takeProfit, trailingStopLoss
    );

    std::cout << "Number of trades: " << trades.size() << std::endl;
}