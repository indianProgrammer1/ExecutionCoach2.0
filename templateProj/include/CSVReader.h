#include "Trade.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>



class CSVReader {
private:
	//  pipeline for sequential data
	std::ifstream file;
public:
	CSVReader(const std::string& filePath);
	bool getNextTrade(Trade& trade);
	std::ifstream& getFile();
};

