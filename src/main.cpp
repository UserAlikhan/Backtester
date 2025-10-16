#include <string>
#include <iostream>
#include "csv_loader.cpp"

int main() {
    const std::string filePathBase = "data/";
    std::string filename, strategy;
    
    std::cout << "Enter the file name: " << std::endl;
    std::cin >> filename;

    std::cout << "Chose the strategy (SMA): " << std::endl;
    std::cin >> strategy;

    CSVLoader csvLoader(filePathBase + filename, ',');
    
    std::cout << "Loading data..." << std::endl;

    std::vector<Candle> candles = csvLoader.load(500);

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

    return 0;
}