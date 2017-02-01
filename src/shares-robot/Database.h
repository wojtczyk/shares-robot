/******************************************************************************
 * Database.h
 *
 * Created on: Jan 10, 2013
 * Copyright Martin Wojtczyk <wojtczyk@in.tum.de>
 ******************************************************************************/

#ifndef DATABASE_H_
#define DATABASE_H_

#include <libpq-fe.h>
#include <vector>
#include <boost/date_time.hpp>

struct Row
{
	int id;
	int marketId;
	std::string xfiSymbol;
	std::string name;
	boost::gregorian::date d;
	double l;
	double h;
	double o;
	double c;
	int v;
	double t1;
	double i1;
	double i2;
	double i3;
	double i4;
	double i5;
};

typedef std::vector<Row> Table;

class Database
{
public:
	Database();
	virtual ~Database();

	bool connect();
	void close();
	bool isConnected();

	std::string dateISO2014(const boost::gregorian::date& d);
	int getRanks(const boost::gregorian::date& d, const std::string& stocksets, Table* table);

private:
	bool connected;
	PGconn* pgConn;
};

#endif /* DATABASE_H_ */
