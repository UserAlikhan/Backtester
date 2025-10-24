#pragma once
#include <iostream>

/*
Ways to detect trend:
 - Moving averages (if price below SMA, SMA becomes resistance, otherwise support)
 - Check swings order, HH for a decent amount of time -> uptrend, otherwise LL -> downtrend 
 - recongnize price levels where asset previously met resistance or support ( > 5)
 - rsi is above 50 for most of the time -> uptrend, below 50 -> downtrend
 - large volumes
 - rsi divergence may show trend reversal
 - swing chart (Kagi charts, Gann-based swing charts)
*/

// assess trend using multiple factors and sum up these factors. Each factor should have a weight
// based on its priority. If final sum is more than a threshold for approving a trend and
// ADX (Average Directional Index) supports the result, then trend is confirmed

// Trend detection is really important, since will be used in Swing / Trend Trading type strategies
class TrendDetection {
    private:
        int threshold;
};