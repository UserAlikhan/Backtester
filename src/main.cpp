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
        std::cout << "\nCalculating SMA...\n" << std::endl;

        SMA sma(candles, 50, 200);

        sma.calculate();
    }

    return 0;
}