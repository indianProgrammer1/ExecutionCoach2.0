#pragma once
#include <string>

struct Config {
    double risk_target_pct = std::numeric_limits<double>::quiet_NaN();
    double min_rr = std::numeric_limits<double>::quiet_NaN();
    int    max_slippage_bps = std::numeric_limits<int>::min();
    double max_vwap_dist = std::numeric_limits<double>::quiet_NaN();
    double threshold = std::numeric_limits<double>::quiet_NaN();
};

Config loadConfig(const std::string& path);

