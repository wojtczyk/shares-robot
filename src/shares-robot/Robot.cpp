/******************************************************************************
 * Robot.cpp
 *
 * Created on: Jan 9, 2013
 * Copyright Martin Wojtczyk <wojtczyk@in.tum.de>
 ******************************************************************************/

#include <iostream>
#include <math.h>
#include "Database.h"
#include "Robot.h"

using namespace std;
using namespace boost::gregorian;

const int Robot::MAX_POSITIONS = 15;
const float Robot::INVESTMENT_RATE = 0.075;

Robot::Robot()
{
	db = new Database();
	db->connect();
}

Robot::~Robot()
{
	delete db;
}

std::vector<std::string>* Robot::invest(const double budget,  const boost::gregorian::date& startDate, const std::string& stocksets)
{
	cash = budget;
	chunk = INVESTMENT_RATE * cash;
	Table oldTable;
	Table table;

	day_iterator itday(startDate);
	date today = day_clock::local_day();

	// traverse the dates since startDate for investment
	while (itday <= today)
	{
		cout << db->dateISO2014(*itday) << endl;

		if (table.size() > 0)
		{
			oldTable = table;
		}
		db->getRanks(*itday, stocksets, &table);
		if (table.size() > 0)
		{
			tfStrategy(&table);
		};

		++itday;
	}

	cashAll(&oldTable);
	return NULL;
};

void Robot::printDepot()
{
	for (Depot::iterator it = depot.begin(); it != depot.end(); it++)
	{
		cout << it->second << "x " << it->first.symbol << endl;
	}
	cout << "cash: " << cash << endl;
};

void Robot::cashAll(Table* table)
{
	cout << "cashAll" << endl;
	Table::iterator itt = table->begin();
	Stock stock;

	while (itt != table->end())
	{
		stock.id = itt->id;
		stock.symbol = itt->xfiSymbol;
		stock.name = itt->name;

		if (depot.find(stock) != depot.end())
		{
			int sold = sell(stock, depot[stock], itt->c);
			cout << "Sold " << sold << " " << stock.symbol << " for " << itt->c << " :: cash = " << cash << endl;
		}

		++itt;
	}
};

/**
 * @param stock
 * @param count
 * @pricePerShare
 * @return
 *
 * buy will correct the number of stocks it can buy if it exceeds the cash.
 */
int Robot::buy(const Stock& stock, const int count, const double pricePerShare)
{
	// adjust count if over budget
	const int maxBuy = (int) floor(cash / pricePerShare);
  int toBuy = count;
	if (maxBuy < count)
	{
		toBuy = maxBuy;
	}

	std::map<Stock, int, StockCompare>::iterator dit = depot.find(stock);

	if (dit == depot.end())
	{
		depot[stock] = toBuy;
	}
	else
	{
		depot[stock] += toBuy;
	}
	cash -= toBuy * pricePerShare;

	return count;
};

/**
 * @param stock
 * @param count
 * @param pricePerShare
 * @return Number of stocks sold. If there were fewer stocks in the depot than
 * count, only the stocks from the depot are sold
 */
int Robot::sell(const Stock& stock, const int count, const double pricePerShare)
{
	std::map<Stock, int, StockCompare>::iterator dit = depot.find(stock);

	if (dit != depot.end())
	{
		if (depot[stock] > count)
		{
			cash += count * pricePerShare;
			depot[stock] -= count;
			return count;
		}
		else // equal or less than count: sell all and delete record
		{
			int inDepot = depot[stock];
			cash += inDepot * pricePerShare;
			depot.erase(stock);
			return inDepot;
		}
	}

	return 0;
};

void Robot::tfStrategy(Table* table)
{
	// go through table and sell items, that fell into the lower 50% or have
	// an indicator of < 1.0
	Table::iterator itmed = table->begin() + table->size() / 2;
	Table::iterator itt = table->begin();
	Stock stock;

	while (itt != table->end())
	{
		stock.id = itt->id;
		stock.symbol = itt->xfiSymbol;
		stock.name = itt->name;

		// if stock found in the lower end, sell
		if ((itt->i3 < 1.0) || (itt >= itmed))
		{
			if (depot.find(stock) != depot.end())
			{
				int sold = sell(stock, depot[stock], itt->c);
				cout << "Sold " << sold << " " << stock.symbol << " for " << itt->c << " :: cash = " << cash << endl;
			}
		}

		++itt;
	}

	Table::iterator ittop10pc = table->begin() + (table->size() / 3);
	itt = table->begin();

	// buy the top 33%, invest INVESTMENT_RATE, don't buy more than MAX_POSITIONS
	while ((itt <= ittop10pc) && (depot.size() < MAX_POSITIONS))
	{
		stock.id = itt->id;
		stock.symbol = itt->xfiSymbol;
		stock.name = itt->name;

		if ((itt->i3 > 1.0) && (depot.find(stock) == depot.end()))
		{
			int num = chunk / itt->c;
			buy(stock, num, itt->c);
			cout << "Bought " << depot[stock] << " " << stock.symbol << " for " << itt->c << " :: cash = " << cash << endl;
		}

		++itt;
	}
}
