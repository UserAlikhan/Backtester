#pragma once
#include <vector>
#include <string>
#include "candle.h"
#include "csvLoader.h"
#include "indicator.h"
#include "ema.h"
#include "backtester.h"

class TestSetup {
    private:
        std::vector<Candle*> candles;
        std::vector<Indicator*> indicators;
        Backtester* backtester;
    
    public:
        TestSetup();
        ~TestSetup();

        void setCandles();
        
        void addIndicator();

        void initializeBacktester();
};