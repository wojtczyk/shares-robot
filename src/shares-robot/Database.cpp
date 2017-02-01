/******************************************************************************
 * Database.cpp
 *
 * Created on: Jan 10, 2013
 * Copyright Martin Wojtczyk <wojtczyk@in.tum.de>
 ******************************************************************************/

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "Database.h"

using namespace std;
using namespace boost::gregorian;

Database::Database() :
	connected(false)
{
}

Database::~Database()
{
	if (connected)
	{
		close();
	}
}

bool Database::connect()
{
	if (connected)
	{
		return true;
	};

	const string host("localhost");
	const string user("xfi_user");
	const string pass("TODO:password");
	const string database("xfi");

	stringstream ss;
	ss.str("");
	ss << "dbname=" << database << " host=" << host << " port=5432 user=" << user << " password=" << pass;

	pgConn = PQconnectdb(ss.str().c_str());

	if (PQstatus(pgConn) != CONNECTION_OK)
	{
		printf("Connection Failed: %s\n", PQerrorMessage(pgConn));
		return false;
	}

	connected = true;

	cout << "Connected to DB: \"" << database << "\" on \"" << host << "\" as \"" << user << "\"" << endl;

	return true;
};

void Database::close()
{
	if (connected)
	{
		PQfinish(pgConn);
		connected = false;
	};
};

bool Database::isConnected()
{
	return connected;
};


std::string Database::dateISO2014(const date& d)
{
	stringstream ss;

	ss.str("");
	date_facet* date_output = new date_facet();
	date_output->format("%Y-%m-%d");
	ss.imbue(std::locale(std::cout.getloc(), date_output));
	ss << d;
	return ss.str();
};

int Database::getRanks(const boost::gregorian::date& d, const std::string& stocksets, Table* table)
{

	table->clear();
	stringstream qs; // query
	stringstream qsinit;
	PGresult* result;
	unsigned int num_fields;
	unsigned int i;
	unsigned int num_rows;

	qs.str("");
	qs << "SELECT xfiSymbol.ID AS ID, marketID, xfiSymbol, name, date, low, high, open, adjClose, volume, SMA130, RSI, RSI3, RSI10, RSI20, RSI30 "
		<< "FROM xfiSymbol, xfiIndexSet, xfiHistoricalData "
		<< "WHERE xfiIndexSet.indexID IN (" << stocksets << ") "
		<< "AND xfiIndexSet.symbolID = xfiSymbol.ID "
		<< "AND xfiSymbol.ID = xfiHistoricalData.symbolID "
		<< "AND xfiHistoricalData.date = '" << dateISO2014(d) << "' "
		<< "ORDER BY RSI10 DESC";

	qsinit.str("");
	qsinit << "BEGIN; DECLARE my_portal CURSOR FOR " << qs.str();

//	cout << qs.str().c_str() << endl;

	result = PQexec(pgConn, qsinit.str().c_str());


	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		printf("Error executing SQL!: %s\n", PQerrorMessage(pgConn));
		PQclear(result);
	}
	else
	{
		PQclear(result);
		result = PQexec(pgConn, "FETCH ALL in my_portal");

		if (PQresultStatus(result) != PGRES_TUPLES_OK)
		{
			printf("ERROR, Fetch All Failed: %s", PQerrorMessage(pgConn));
			PQclear(result);
		}
		else
		{
			num_fields = PQnfields(result);

			// Print out the rows
			for (i = 0; i < PQntuples(result); i++)
			{
				Row r;
				r.id =        PQgetisnull(result, i, 0) ? 0 : boost::lexical_cast<int>(string(PQgetvalue(result, i, 0)));
				r.marketId =  PQgetisnull(result, i, 1) ? 0 : boost::lexical_cast<int>(string(PQgetvalue(result, i, 1)));
				r.xfiSymbol = PQgetisnull(result, i, 2) ? string("") : string(PQgetvalue(result, i, 2));
				r.name =      PQgetisnull(result, i, 3) ? string("") : string(PQgetvalue(result, i, 3));
				r.d =         PQgetisnull(result, i, 4) ? from_string(string("2012-01-01")) : from_string(string(PQgetvalue(result, i, 4)));
				r.l =         PQgetisnull(result, i, 5) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 5)));
				r.h =         PQgetisnull(result, i, 6) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 6)));
				r.o =         PQgetisnull(result, i, 7) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 7)));
				r.c =         PQgetisnull(result, i, 8) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 8)));
				r.v =         PQgetisnull(result, i, 9) ? 0 : boost::lexical_cast<int>(string(PQgetvalue(result, i, 9)));
				r.t1 =        PQgetisnull(result, i, 10) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 10)));
				r.i1 =        PQgetisnull(result, i, 11) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 11)));
				r.i2 =        PQgetisnull(result, i, 12) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 12)));
				r.i3 =        PQgetisnull(result, i, 13) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 13)));
				r.i4 =        PQgetisnull(result, i, 14) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 14)));
				r.i5 =        PQgetisnull(result, i, 15) ? 0 : boost::lexical_cast<double>(string(PQgetvalue(result, i, 15)));

				table->push_back(r);
			}

			result = PQexec(pgConn, "END");
			PQclear(result);
			return table->size();
		}
	}

	return 0;
};
