#include <iostream>
#include <testSetup.h>

int main() {
    
    TestSetup testSetup;
    testSetup.setCandles();

    testSetup.setBalance();

    testSetup.addIndicator();

    testSetup.identifyTrend();

    testSetup.initializeBacktester();

    return 0;
}