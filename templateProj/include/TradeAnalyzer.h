#pragma once
#include "Trade.h"
#include "Config.h"
#include "TradeResult.h"
#include "Decision.h"


class TradeAnalyzer
{
private:
	Config cfg_;

public:
	TradeAnalyzer(const Config& cfg) : cfg_(cfg) {}

	TradeResult analyze(const  Trade& trade);
};





