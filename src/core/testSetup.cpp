#include "testSetup.h"
#include <iostream>
#include <string>

// constructor
TestSetup::TestSetup() {}

// destructor, frees the memory
TestSetup::~TestSetup() {
    for (auto c : candles) {
        delete c;
    }

    for (auto ind : indicators) {
        delete ind;
    }

    delete backtester;

    std::cout << "TestSetup memory was cleaned." << std::endl;
}

void TestSetup::setCandles() {
    const std::string filePathBase = "data/";
    std::string filename;
    
    std::cout << "Enter the file name: " << std::endl;
    std::cin >> filename;

    CSVLoader csvLoader(filePathBase + filename, ',');
    std::vector<Candle*> candlesData = csvLoader.load();

    candles = candlesData;

    std::cout << "Data was uploaded successfully" << std::endl;
}

void TestSetup::setBalance() {
    double balance;

    std::cout << "Enter you initial balace: " << std::endl;
    std::cin >> balance;

    this->balance = balance;
}

void TestSetup::addIndicator() {
    std::string strategy;

    while (true) {
        std::cout << "Type indicator names to select (type stop after you are done): " << std::endl;
        std::cin >> strategy;

        if (strategy == "stop") break;

        if (strategy == "SMA") {
            double per1, per2;

            std::cout << "Chose period for first moving average: " << std::endl;
            std::cin >> per1;
            std::cout << "Chose period for second moving average: " << std::endl;
            std::cin >> per2;

            SMA* ema = new SMA("Simple moving average", per1, per2);
            indicators.push_back(ema);

            std::cout << "Indicator "<< ema->getName() << " was added successfully!" << std::endl;
        } else if (strategy == "EMA") {
            double per1, per2;

            std::cout << "Chose period for first moving average: " << std::endl;
            std::cin >> per1;
            std::cout << "Chose period for second moving average: " << std::endl;
            std::cin >> per2;

            EMA* ema = new EMA("Exponential moving average", per1, per2);
            indicators.push_back(ema);

            std::cout << "Indicator "<< ema->getName() << " was added successfully!" << std::endl;
        } else if (strategy == "RSI") {
            double period;
            std::string rsiStrategy;

            std::cout << "Chose period for RSI: " << std::endl;
            std::cin >> period;

            std::cout << "Chose strategy for RSI: " << std::endl;
            std::cin >> rsiStrategy;
            
            RSI* rsi = nullptr;
            
            if (rsiStrategy == "extreme" || rsiStrategy == "extreme values") {
                rsi = new RSI("Relative Strength Index", OscilatorStrategyTypes::EXTREME_VALUES, period);
            } else if (rsiStrategy == "trend" || rsiStrategy == "trend confirmation") {
                rsi = new RSI("Relative Strength Index", OscilatorStrategyTypes::TREND_CONFIRMATION, period);
            } else if (rsiStrategy == "divergence") {
                rsi = new RSI("Relative Strength Index", OscilatorStrategyTypes::DIVERGENCE, period);
            }

            if (!rsi) {
                std::cout << "❌ Invalid RSI strategy entered!" << std::endl;
                return;
            }

            indicators.push_back(rsi);

            std::cout << "Indicator "<< rsi->getName() << " was added successfully!" << std::endl;
        }
    }
}

void TestSetup::initializeBacktester() {
    std::cout << "Running backtest..." << std::endl;

    std::cout << "Your indicators: ";
    for (auto indc : indicators) {
        std::cout << indc->getName() << " ";
    }
    std::cout << std::endl;

    Backtester* backtester = new Backtester();
    backtester->run(candles, indicators, balance);

    std::cout << "Final balance: " << balance;
}