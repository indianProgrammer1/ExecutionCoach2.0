#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <iostream>

class Logger {
private:
    std::ofstream file;
    std::mutex mu;

    Logger();
    ~Logger();
      
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    void info(const std::string& msg);
    void error(const std::exception& msg);

};
