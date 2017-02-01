/******************************************************************************
 * Stock.h
 *
 * Created on: Jan 9, 2013
 * Copyright Martin Wojtczyk <wojtczyk@in.tum.de>
 ******************************************************************************/

#ifndef STOCK_H
#define STOCK_H

#include <string>

struct Stock
{
public:
	Stock();
	Stock(const int id, const std::string& symbol, const std::string& name);
	virtual ~Stock();

	int id;
	std::string symbol;
	std::string name;
};

struct StockCompare
{
   bool operator() (const Stock& lhs, const Stock& rhs) const
   {
       return lhs.id < rhs.id;
   }
};

#endif /* STOCK_H */
