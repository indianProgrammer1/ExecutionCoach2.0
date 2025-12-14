#pragma once
#include <string>
#include <chrono>
class Trade
{
private:
	
	int id{ 0 };
	std::string side;
	std::string Symbol;
	int64_t entry{ 0 };
	int64_t exit{ 0 };
	int64_t stopLoss{ 0 };
	int64_t equity{ 0 };
	int64_t fee{ 0 };
	int64_t atr{ 0 };
	double slippageBps{ 0 };
	int qty{ 0 };
	double captureRatio{ 0.0 };
	double riskRewardReal{ 0.0 };
	double riskRewardPlan{ 0.0 };
	double vwapDistance{ 0.0 };
	std::chrono::nanoseconds ts_ns{ 0 };

public:

	Trade() = default;


	std::string getSide() const { return side; }
	void setSide(std::string val) { side = val; }
	std::string getSymbol() const { return Symbol; }
	void setSymbol(std::string val) { Symbol = val; }
	std::chrono::nanoseconds Ts_ns() const { return ts_ns; }
	void Ts_ns(std::chrono::nanoseconds val) { ts_ns = val; }
	int getId() const { return id; }
	void setId(int _id) { id = _id; }
	int getQty() const { return qty; }
	void setQty(int val) { qty = val; }
	const double getSlippage() const { return slippageBps; }
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
	int64_t getRisk() const {
		return (std::llabs(getEntry() - getStopLoss()));
	 }
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



