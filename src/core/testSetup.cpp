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

void TestSetup::addIndicator() {
    std::string strategy;

    std::cout << "Chose the strategy (SMA): " << std::endl;
    std::cin >> strategy;

    if (strategy == "SMA") {
        double per1, per2;

        std::cout << "Chose period for first moving average: " << std::endl;
        std::cin >> per1;
        std::cout << "Chose period for second moving average: " << std::endl;
        std::cin >> per2;

        EMA* ema = new EMA("Simple moving average", per1, per2);
        indicators.push_back(ema);
        ema->calculate(candles);

        std::vector<double> data1 = ema->getDataPeriod1();
        std::vector<double> data2 = ema->getDataPeriod2();

        std::cout << "Indicator "<< ema->getName() << " was added successfully!" << std::endl;
    }
}