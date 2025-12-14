#include "Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
//removes spaces/tabs/newlines from
//the beginning and the end of a string
static inline std::string trim(std::string s) {
    
    auto not_space = [](int ch) { return !std::isspace(ch); };

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
    return s;
}
//converts all characters in a string to lowercase letters
static inline std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
        { return static_cast<char>(std::tolower(c)); });
    return s;
}
//takes configure file path and extracts its values

Config loadConfig(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Cannot open config file: " + path);
    Config cfg;

    //"std::string::npos" its special value that said faliure
    std::string line;
    while (std::getline(in, line)) {
        //save only the string until the sign # or ;
        if (auto index = line.find('#');  index != std::string::npos) line = line.substr(0, index);
        if (auto index = line.find(';');  index != std::string::npos) line = line.substr(0, index);

        line = trim(line);
        if (line.empty()) continue;
        if (line.front() == '[' && line.back() == ']') continue; // [rules]

        auto equalSign = line.find('=');
        if (equalSign == std::string::npos) continue;

        std::string key = lower(trim(line.substr(0, equalSign)));
        std::string value = trim(line.substr(equalSign + 1));

        try {
            if (key == "risk_target_pct")       cfg.risk_target_pct = std::stod(value);
            else if (key == "min_rr")           cfg.min_rr = std::stod(value);
            else if (key == "max_slippage_bps") cfg.max_slippage_bps = std::stoi(value);
            else if (key == "max_vwap_dist")    cfg.max_vwap_dist = std::stod(value);
            else if (key == "threshold")       cfg.threshold = std::stod(value);
        }
        catch (...) {
            throw std::runtime_error("Bad value for key '" + key + "': " + value);
        }
    }


    if (std::isnan(cfg.risk_target_pct) || std::isnan(cfg.min_rr) ||
        std::isnan(cfg.max_vwap_dist) || std::isnan(cfg.threshold) ||
        cfg.max_slippage_bps == std::numeric_limits<int>::min()
        )
         {
        throw std::runtime_error("Config file contains empty or invalid numeric fields");
    }

    return cfg;
}
