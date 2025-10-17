#include <string>
#include <iostream>
#include <vector>
#include <csv_loader.h>
#include <simple_moving_average.h>

int main() {
    const std::string filePathBase = "data/";
    std::string filename, strategy;
    
    std::cout << "Enter the file name: " << std::endl;
    std::cin >> filename;

    std::cout << "Chose the strategy (SMA): " << std::endl;
    std::cin >> strategy;

    CSVLoader csvLoader(filePathBase + filename, ',');
    
    std::cout << "Loading data..." << std::endl;

    std::vector<Candle> candles = csvLoader.load();

    for (int i = 0; i < candles.size(); i++) {
        Candle candle = candles[i];
        std::cout << candle.timestamp
            << " Open: " << candle.open
            << " High: " << candle.high
            << " Low: " << candle.low
            << " Close: " << candle.close
            << " Adjusted Close: " << candle.adjClose
            << " Volume: " << candle.volume
            << std::endl;
    }

    if (strategy == "SMA") {
        std::cout << "Calculating SMA..." << std::endl;

        SMA sma50(candles, 50);

        std::vector<double> sma50Calc = sma50.calculate();

        for (int i = 0; i < sma50Calc.size(); i++) {
            std::cout << "Index: " << i << " value: " << sma50Calc[i] << std::endl;
        }
    }

    return 0;
}