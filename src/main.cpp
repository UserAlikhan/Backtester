#include <string>
#include <iostream>
#include <vector>
#include <simple_moving_average.h>
#include <testSetup.h>

int main() {
    
    TestSetup testSetup;
    testSetup.setCandles();

    testSetup.addIndicator();

    // if (strategy == "SMA") {
    //     std::string trStopAnswer;
    //     double maxStopLoss, trStopPercentage;

    //     std::cout << "\nCalculating SMA...\n" << std::endl;

    //     std::cout << "Specify Max Stop loss: " << std::endl;
    //     std::cin >> maxStopLoss;

    //     std::cout << "Do you need a trailing stop?: Yes/No" << std::endl;
    //     std::cin >> trStopAnswer;

    //     if (trStopAnswer == "Yes") {
    //         std::cout << "Enter trailing stop percentage (10.0): " << std::endl;
    //         std::cin >> trStopPercentage;
    //     }

    //     SMA sma(candles, 50, 200);

    //     sma.calculate(maxStopLoss, trStopPercentage);
    // }

    return 0;
}