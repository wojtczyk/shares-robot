/******************************************************************************
 * Robot.h
 *
 * Created on: Jan 9, 2013
 * Copyright Martin Wojtczyk <wojtczyk@in.tum.de>
 ******************************************************************************/

#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <map>
#include <vector>
#include <boost/date_time.hpp>
#include "Database.h"
#include "Stock.h"

typedef std::map<Stock, int, StockCompare> Depot;

class Robot
{
public:
	Robot();
	virtual ~Robot();

	std::vector<std::string>* invest(const double budget, const boost::gregorian::date& startDate, const std::string& stocksets);
	void printDepot();

private:
	static const int MAX_POSITIONS;
	static const float INVESTMENT_RATE;
	Database* db;
	double cash;
	double chunk;
	Depot depot;

	int buy(const Stock& stock, const int count, const double pricePerShare);
	int sell(const Stock& stock, const int count, const double pricePerShare);
	void tfStrategy(Table* table);
	void cashAll(Table* table);
};

#endif /* ROBOT_H */
