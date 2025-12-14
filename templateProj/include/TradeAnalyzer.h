#pragma once
#include "../include/Trade.h"
#include "../include/Config.h"
#include "../include/TradeResult.h"
#include "../include/Decision.h"


class TradeAnalyzer
{
private:
	Config cfg_;

public:
	TradeAnalyzer(const Config& cfg) : cfg_(cfg) {}

	TradeResult analyze(const  Trade& trade);
};




