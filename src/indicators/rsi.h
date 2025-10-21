// RSI is a momentum oscilator used primarily to identify overbought and oversold conditions
// Typically an RSI below 30 indicates oversold and above 70 indicates overbought conditions

// RSI tends to stay between 40 and 70, with overbought conditions above 70, indicating
// strong momentum rather than an imminent reversal. Traders often looking for dip-buying
// opportunities when RSI pulls back to around 50 rather than waiting for it to drop below 30
// - something that rarely happens in strong uptrends.
#pragma once
#include <iostream>
#include "indicator.h"

class RSI : public Indicator {

};