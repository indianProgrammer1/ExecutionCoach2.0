#include "TradeAnalyzer.h"
#include "TradeResult.h"
#include "Stats.h"
#include "Trade.h"
#include <algorithm>
#include <cmath>
#include <string>
double scoreTrade(const Trade& t);

TradeResult TradeAnalyzer::analyze(const  Trade& trade) {
	TradeResult result;

	decision decision = OK;
	//configuration requirements 

	if (trade.getRiskRewardPlan() < cfg_.min_rr)
		decision = std::max(decision, BLOCK);

	if ((trade.getRisk()* trade.getQty()/ trade.getEquity()) > cfg_.risk_target_pct)
		decision = std::max(decision, BLOCK);

	if (trade.getRisk() == cfg_.risk_target_pct)
		decision = std::max(decision, WARN);

	if (trade.getSlippage() == cfg_.max_slippage_bps)
		decision = std::max(decision, WARN);

	if (trade.getSlippage() > cfg_.max_slippage_bps)
		decision = std::max(decision, ADJUST_QTY);

	if (abs(trade.getVwapDistance()) >= cfg_.max_vwap_dist)
		decision = std::max(decision, WARN);

	if (trade.getCaptureRatio() < cfg_.threshold)
		decision = std::max(decision, WARN);


	switch (decision) {
	case OK:         Stats::ok_count++; break;
	case WARN:       Stats::warn_count++; break;
	case ADJUST_QTY: Stats::adjust_count++; break;
	case BLOCK:      Stats::block_count++; break;
	}

	result.decision = decision;
	result.score = scoreTrade(trade);
	result.id = trade.getId();
	return result;
	

}
double scoreTrade(const Trade& t)
{ //from cents to dollars
	const double entry = static_cast<double>(t.getEntry()) / 100.0;
	const double exit = static_cast<double>(t.getExit()) / 100.0;
	const double stop = static_cast<double>(t.getStopLoss()) / 100.0;
	const double equity = static_cast<double>(t.getEquity()) / 100.0;
	const double fee = static_cast<double>(t.getFee()) / 100.0;
	const double atr = static_cast<double>(t.getAtr()) / 100.0; 

	const double slipBps = static_cast<double>(t.getSlippage());
	const int qty = t.getQty();
	
	//std::max its to not divide by 0 
	//the more we close to 0.6 atr capture,the more score we get
    double atrPercentage = std::fabs(exit - entry) / std::max(1e-6, atr);
	double differnce = std::fabs(atrPercentage - 0.6);
	double scoreAtr = std::max(0.0, 100.0 - std::min(1.0, differnce / 0.6) * 100.0);

	// the closer the real RR to the planned RR, the higher the score 
    double riskReward = t.getRiskRewardReal() / std::max(1e-4, t.getRiskRewardPlan());
	double scoreReward = 100.0 * std::clamp(riskReward, 0.0, 1.0);

	// the smaller the risk percentage of equity, the higher the score
	 double riskMoney = std::fabs(entry - stop) * static_cast<double>(qty); 
	 double riskPercentage = riskMoney / std::max(1e-6, equity);
	 double scoreRisk = 100.0 * std::clamp((0.05 - riskPercentage) / 0.05, 0.0, 1.0);

	// the lower the total cost slippage + fee impact,  the higher the score
	double slipPerTrade = (slipBps/10000.0 * entry *qty + fee) / std::max(1e-6, riskMoney);
	double scoreSlip = 100.0 * std::clamp((0.01 - slipPerTrade) / 0.01, 0.0, 1.0);

	// the closer the trade execution price is to VWAP, the higher the score
	double vwapDist = std::abs(t.getVwapDistance());
	double scoreVwap = 100.0 * std::clamp((0.02 - vwapDist) / 0.02, 0.0, 1.0);

	//giving diffrent weight base on its impact 
	return scoreAtr * 0.2 +
		   scoreReward * 0.3 +
		   scoreRisk * 0.3 +
	       scoreSlip * 0.15 +
		   scoreVwap * 0.25;

}


