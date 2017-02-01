/******************************************************************************
 * Stock.cpp
 *
 * Created on: Jan 9, 2013
 * Copyright Martin Wojtczyk <wojtczyk@in.tum.de>
 ******************************************************************************/

#include "Stock.h"

Stock::Stock() : id(0), symbol(""), name("")
{
}

Stock::Stock(const int id, const std::string& symbol, const std::string& name)
{
	this->id = id;
	this->symbol = symbol;
	this->name = name;
};

Stock::~Stock()
{
}

