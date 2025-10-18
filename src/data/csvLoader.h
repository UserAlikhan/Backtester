#pragma once
#include <vector>
#include <string>
#include <candle.h>

class CSVLoader {
    private:        
        std::string filename;
        char delimiter;
    
    public:
        CSVLoader(const std::string& file, char delim = ',');
        std::vector<Candle*> load();
};