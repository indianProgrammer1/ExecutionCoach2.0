#include "../include/Logger.h"
Logger::Logger() {
	//append to app.log file 
	//log messages

	file.open("Logger.log", std::ios::app);
	if (!file.is_open()) {
		std::cerr << "Failed to open app.log" << std::endl;
	}
}
Logger::~Logger() {
	file.close();
}

void Logger::info(const std::string& msg) {
	std::lock_guard<std::mutex> lock(mu);
	file << "[INFO]: " << msg << std::endl;
}

void Logger::error(const std::exception& msg) {
	std::lock_guard<std::mutex> lock(mu);
	file << "[ERROR]: " << msg.what() << std::endl;
}