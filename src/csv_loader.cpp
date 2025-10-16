#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "candle.cpp"

class CSVLoader {
    private:
        std::string filename;
        char delimiter;
    
    public:
        CSVLoader(const std::string& file, char delim = ','): filename(file), delimiter(delim) {}

        std::vector<Candle> load(int numOfRows) {
            std::vector<Candle> data;
            std::ifstream file(filename);

            if(!file.is_open()) {
                std::cerr << "Error: could not open the file." << std::endl;
                return data;
            }

            std::string line;
            bool isHeader = true;
            int counter = 0;

            while(std::getline(file, line)) {
                if (isHeader) {
                    isHeader = false;
                    continue;
                }

                if (counter >= numOfRows) {
                    break;
                }

                std::stringstream ss(line);
                std::string value;
                Candle candle;

                std::getline(ss, value, delimiter);
                candle.timestamp = value;

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
                counter++;
            }

            file.close();
            return data;
        }
};