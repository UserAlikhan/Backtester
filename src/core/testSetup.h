#pragma once
#include <vector>
#include <string>
#include "candle.h"
#include "csvLoader.h"
#include "indicator.h"
#include "ema.h"
#include "sma.h"
#include "rsi.h"
#include "backtester.h"
#include "oscilatorStrategyTypesEnum.h"

class TestSetup {
    private:
        std::vector<Candle*> candles;
        std::vector<Indicator*> indicators;
        Backtester* backtester;
        double balance = 5000.0;
    
    public:
        TestSetup();
        ~TestSetup();

        void setCandles();

        double getBalance() { return balance; }
        
        void setBalance();
        
        void addIndicator();

        void initializeBacktester();
};