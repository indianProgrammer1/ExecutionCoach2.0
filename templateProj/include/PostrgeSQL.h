#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include "TradeAnalyzer.h"
class PostrgeSQL
{
private:
	static PostrgeSQL* instance;
    std::string connectionString;
    std::unique_ptr<pqxx::connection> conn_;

public:
    static PostrgeSQL* getInstance();
    void connectToDB(const std::string& postgreConn);
    void saveToDB(const std::vector<TradeResult>& batch);
};


