#pragma once
#include <string>
#include "../include/Trade.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "../include/CSVReader.h"
#include "../include/Stats.h"
#include"../include/Logger.h"
//REASON_CODES
const std::string NOVALUE = "00";
const std::string SUCCESS= "01";


CSVReader::	CSVReader(const std::string& filePath)
{
	file.open(filePath);
	(!file.is_open()) ?
		std::cout << "Cant open this file,Please enter Vaild Path!" << std::endl :
		std::cout << "file open succesfully!" << std::endl;
	std::string line;
	std::getline(file, line);

};
//return ifstream value of the current object for 
//pipeline for sequential data,whenever we call it
//we enter the next line in the csvfile
std::ifstream& CSVReader::getFile() { return file; };
bool CSVReader::getNextTrade(Trade& trade) {

	std::string line;
	std::ifstream& f = getFile();

	if (!std::getline(f, line))
		return false; 

	if (line.empty())
		return false;
	try {
		
		std::stringstream ss(line);
		std::string value;
		//ss its a stream that separate the current string 
		//by , 

		//after every getline() call,the ss pointer go forward
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoTimeValue: " + NOVALUE);
		}
		long long ns = std::stoll(value);
		trade.Ts_ns(std::chrono::nanoseconds(ns));

		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoTickerValue: " + NOVALUE);
		}
		trade.SetSymbol(value);

		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoSideOpition: " + NOVALUE);
		}
		trade.setSide(value);

		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoEquityValue: " + NOVALUE);
		}
		double doubleValue = std::stod(value);
		trade.setEquity(doubleValue);

		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoEntryValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setEntry(doubleValue);

		// exit
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoExitValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setExit(doubleValue);

		// stop
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoStopValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setStopLoss(doubleValue);

		// qty
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoQuantityValue: " + NOVALUE);
		}
		int intValue = std::stoi(value);
		trade.setQty(intValue);

		// fees
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoFeesValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setFee(doubleValue);

		// slippage_bps
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoSlippageValue: " + NOVALUE);
		}
		doubleValue = std::stoi(value);
		trade.setSlippage(doubleValue);

		// atr
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoAtrValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setAtr(doubleValue);

		// vwap_dist
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoVWapValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setVwapDistance(doubleValue);

		// rr_plan
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoRR_PlanValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setRiskRewardPlan(doubleValue);

		// rr_real
		std::getline(ss, value, ',');
		if (value.empty())
		{
			throw std::runtime_error("NoRR_RealValue: " + NOVALUE);
		}
		doubleValue = std::stod(value);
		trade.setRiskRewardReal(doubleValue);

		std::getline(ss, value);
		if (!value.empty() && value.back() == '\r') value.pop_back();
		doubleValue = std::stod(value);
		trade.setCaptureRatio(doubleValue);
		

	Stats::processed_lines++;
	Logger::getInstance().info( " parseTradeSuccesfully: " + SUCCESS);
	}
	catch (const std::exception& e) {
		Stats::parser_errors++;
		Logger::getInstance().error(e);
		this->getNextTrade(trade);
	}

	return true;
}