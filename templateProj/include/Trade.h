#pragma once
#include <string>
#include <chrono>
class Trade
{
private:
	
	int id;
	std::string side;
	std::string Symbol;
	const double risk;
	int64_t entry;
	int64_t exit;
	int64_t stopLoss;
	int64_t equity;
	int64_t fee;
	int64_t atr;
	int slippageBps;
	int qty;
	double captureRatio;
	double riskRewardReal;
	double riskRewardPlan;
	double vwapDistance;
	std::chrono::nanoseconds ts_ns;

public:

	Trade() :
		side(""),
		Symbol(""),
		id(),
		risk(getEntry() - getStopLoss()),
		entry(0.0),
		exit(0.0),
		stopLoss(0.0),
		equity(0.0),
		fee(0.0),
		slippageBps(0),
		qty(0),
		captureRatio(0.0),
		riskRewardReal(0.0),
		riskRewardPlan(0.0),
		vwapDistance(0.0),
		atr(0.0),
		ts_ns(std::chrono::nanoseconds(0))
	 {};

	std::string getSide() const { return side; }
	void setSide(std::string val) { side = val; }
	std::string GetSymbol() const { return Symbol; }
	void SetSymbol(std::string val) { Symbol = val; }
	const double getRisk() const { return risk; }
	std::chrono::nanoseconds Ts_ns() const { return ts_ns; }
	void Ts_ns(std::chrono::nanoseconds val) { ts_ns = val; }
	int getId() const { return id; }
	void setId(int _id) { id = _id; }
	int getQty() const { return qty; }
	void setQty(int val) { qty = val; }
	const int getSlippage() const { return slippageBps; }
	void setSlippage(double val) { slippageBps = val; }
	double getVwapDistance() const { return vwapDistance; }
	void setVwapDistance(double val) { vwapDistance = val; }
	double getRiskRewardPlan() const { return riskRewardPlan; }
	void setRiskRewardPlan(double val) { riskRewardPlan = val; }
	double getRiskRewardReal() const { return riskRewardReal; }
	void setRiskRewardReal(double val) { riskRewardReal = val; }
	double getCaptureRatio() const { return captureRatio; }
	void setCaptureRatio(double val) { captureRatio = val; }
	

	//from dollars to cents
	int64_t getEquity() const { return equity; }
	void setEquity(double val) { equity = static_cast<int64_t>(std::round(val * 100));}
	
	int64_t getAtr() const { return atr; }
	void setAtr(double val) { atr = static_cast<int64_t>(std::round(val * 100));}

	int64_t getEntry() const { return entry; }
	void setEntry(double val) { entry = static_cast<int64_t>(std::round(val * 100)); }
	
	int64_t getExit() const { return exit; }
	void setExit(double val) { exit = static_cast<int64_t>(std::round(val * 100)); }
	
	int64_t getStopLoss() const { return stopLoss; }
	void setStopLoss(double val) { stopLoss = static_cast<int64_t>(std::round(val * 100)); }
	
	int64_t getFee() const { return fee; }
	void setFee(double val) { fee = static_cast<int64_t>(std::round(val * 100));}
	




};

