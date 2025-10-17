#include "csv_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

CSVLoader::CSVLoader(const std::string& file, char delim)
    : filename(file), delimiter(delim) {}

std::vector<Candle> CSVLoader::load() {
    std::vector<Candle> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: could not open the file " << filename << std::endl;
        return data;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (isHeader) { 
            isHeader = false; 
            continue; 
        }

        std::stringstream ss(line);
        std::string value;
        Candle candle;

        std::getline(ss, candle.timestamp, delimiter);

        std::getline(ss, value, delimiter); 
        candle.open = std::stod(value);

        std::getline(ss, value, delimiter); 
        candle.high = std::stod(value);

        std::getline(ss, value, delimiter); 
        candle.low = std::stod(value);

        std::getline(ss, value, delimiter); 
        candle.close = std::stod(value);

        std::getline(ss, value, delimiter); 
        candle.adjClose = std::stod(value);

        std::getline(ss, value, delimiter); 
        candle.volume = std::stod(value);

        data.push_back(candle);
    }

    return data;
}