#include <iostream>
#include <stdio.h>
#include <string>
#include "../include/CSVReader.h"
#include "../include/PostrgeSQL.h"
#include "../include/TradeAnalyzer.h"
#include <thread>
#include "../include/Stats.h"
#include "../include/Latency.h"
#include "../include/TradeResult.h"
#include <vector>
#include <mutex>
#include "../include/Config.h"

static int next_id = 1;   
std::vector<TradeResult> producerQueue;
std::mutex queueMutex;
std::atomic<bool> csvTradeFinish{ false };
void importTradesFromCSVToDB(const std::string &csvPath,const std::string &configPath,const std::string& postgreConn);

int main(int argc, char* argv[]) {

  try {               //ask for 3 arguments 
      if (argc != 4) { 
          std::cerr << "Usage: ExecutionCoach <csvPath> <configPath> <postgreConn>\n";
          return 1;
      }
      importTradesFromCSVToDB(argv[1], argv[2], argv[3]);
  }
  catch (const std::exception& e) {
      std::cerr << "Fatal: " << e.what() << "\n";
      return 2;
  }
}


void importTradesFromCSVToDB(const std::string& csvPath, const std::string& configPath, const std::string& postgreConn) {

    //files loading
    Config cfg = loadConfig(configPath);
    CSVReader reader(csvPath);
    PostrgeSQL::getInstance()->connectToDB(postgreConn);

    Trade trade;
    TradeAnalyzer analyzer(cfg);

    //the thread that will write to the database
    std::thread writerThread([&]() {
        using namespace std::chrono_literals;

        while (!csvTradeFinish || !producerQueue.empty()) {
            //take the results from the producerQueue to the WriterQueue 
            std::vector<TradeResult> writerQueue;
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (!producerQueue.empty())
                    writerQueue.swap(producerQueue);
            }

                try 
                {
                    PostrgeSQL::getInstance()->saveToDB(writerQueue);
                }
                catch (const std::exception& e) {
                    Stats::db_errors++;
                    std::cerr << "DB write error: " << e.what() << std::endl;
                }
 
            //write in 10ms batch 
            if (writerQueue.empty())
                std::this_thread::sleep_for(10ms);
        }
        });
  

    int tradeId = 1;
    //hot path 
    while (true)
    {
     auto start = std::chrono::steady_clock::now();
     
     if(!reader.getNextTrade(trade)) break;
     trade.setId(next_id++);
        TradeResult res = analyzer.analyze(trade);

        while (true) 
        {
            { //producerQueue always get TradeResult
                std::lock_guard<std::mutex> lock(queueMutex);
                if (producerQueue.size() < 1000) 
                {   
                    producerQueue.push_back(res);
                    break;
                }
            }
            //backPressure mechanism
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        auto end = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        LatencyStats::getInstance().record(ms);
    }

    csvTradeFinish = true;
    if (writerThread.joinable()) writerThread.join();


    LatencyStats::getInstance().printSummary();
    Stats::printSummary();


}
	




