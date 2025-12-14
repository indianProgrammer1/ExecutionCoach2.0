#pragma warning(disable : 4996)
#include "PostrgeSQL.h"
#include "TradeResult.h"
PostrgeSQL* PostrgeSQL::instance = nullptr;
//singeltion pattern 
PostrgeSQL* PostrgeSQL::getInstance() {
	if (instance == nullptr)
		instance = new PostrgeSQL();
	return instance;
};

void PostrgeSQL::connectToDB(const std::string& postgreConn) {

	connectionString = postgreConn;
	//connection object for the database
	conn_ = std::make_unique<pqxx::connection>(connectionString);
	if (!conn_ || !conn_->is_open()) {
		throw std::runtime_error("Failed to open DB connection");
	}

	{
		// txn its object that do actions on his conn object,
		// txn do all its executions sucessfully,
		// or it not execute nothing 
		pqxx::work txn{ *conn_ };
		txn.exec(
			"CREATE TABLE IF NOT EXISTS trades ("
			"  id        INT PRIMARY KEY,"
			"  decision  TEXT NOT NULL,"
			"  score     DOUBLE PRECISION NOT NULL"
			");"
		);
		txn.commit();
	}
	conn_->prepare(
		"ins_trade",
		"INSERT INTO trades (id, decision, score) "
		"VALUES ($1,$2,$3) "
		"ON CONFLICT (id) DO UPDATE SET "
		"decision = EXCLUDED.decision, score = EXCLUDED.score"
	);
}


static inline std::string decision_to_string(decision d) {
	switch (d) {
	case OK:         return "OK";
	case WARN:       return "WARN";
	case ADJUST_QTY: return "ADJUST_QTY";
	case BLOCK:      return "BLOCK";
	default:         return "UNKNOWN";
	}
}
void PostrgeSQL::saveToDB(const std::vector<TradeResult>& batch)
{
	if (batch.empty()) return;
	if (!conn_ || !conn_->is_open()) {
		std::cerr << "DB connection not initialized.\n";
		return;
	}

	try {
		// txn its object that do actions on his conn object,
		// txn do all its executions sucessfully,
		// or it not execute nothing 
		pqxx::work txn{ *conn_ };
		for (const auto& r : batch) {

		txn.exec_prepared("ins_trade",
		r.id,
		decision_to_string(r.decision),
		r.score);

		}
		//only here we save all our execution on conn
		txn.commit();
	}
	catch (const std::exception& e) {
		std::cerr << "saveBatch error: " << e.what() << "\n";
	}
}
