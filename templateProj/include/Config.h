#pragma once
#include <string>

struct Config {
    double risk_target_pct;
    double min_rr ;
    int    max_slippage_bps ;
    double max_vwap_dist ;
    double threshold ; 
};

Config loadConfig(const std::string& path);
