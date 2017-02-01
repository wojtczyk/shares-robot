/******************************************************************************
 * main.cpp
 *
 * Created on: Jan 9, 2013
 * Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
 *
 * Description
 ******************************************************************************/

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include "config.h"
#include "Robot.h"

using namespace std;
using namespace boost;
using namespace boost::gregorian;

bool getIntList(const std::string& src, std::vector<int>& res)
{
	using boost::lexical_cast;
	using boost::bad_lexical_cast;
	bool success = true;
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sepa(",");
	tokenizer tokens(src, sepa);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		try
		{
			res.push_back(lexical_cast<int>(*tok_iter));
		}
		catch (bad_lexical_cast &)
		{
			success = false;
		}
	}
	return success;
}

int main(int argc, char** argv)
{
	cout << PACKAGE_STRING << endl << "by " << PACKAGE_BUGREPORT << endl << endl;

	if (argc != 4)
	{
		cout << "Syntax: " << PACKAGE_NAME << " budget date stocksets" << endl;
		return 1;
	}
	else
	{
		string sBudget(argv[1]);
		string sDate(argv[2]);
		string sStocksets(argv[3]);

		// parse and validate the budget input
		double dBudget;
		try
		{
			dBudget = lexical_cast<double>(sBudget);
		}
		catch (std::exception& e)
		{
			cerr << "Error: 1st parameter is not a valid budget." << endl;
			cerr << e.what() << endl;
			return -1;
		}

		// parse and validate the date input
		date dDate;
		try
		{
			dDate = from_string(sDate);
		}
		catch (std::exception& e)
		{
			cerr << "Error: 2nd parameter is not a valid date in form YYYY-MM-DD." << endl;
			cerr << e.what() << endl;
			return -1;
		}

		// parse and validate the index list input
		vector<int> indexList;
		if (!getIntList(sStocksets, indexList))
		{
			cout << "Error: 3rd parameter is not an index list int[,int...]" << endl;
			return -1;
		}

		cout << PACKAGE_NAME << " simulated investment based on historical data: " << endl;
		cout << "Budget: " << dBudget << endl;
		cout << "Start Date: " << dDate << endl;
		cout << "Stock Sets: " << sStocksets << endl;

		Robot* robot = new Robot();
		robot->invest(dBudget, dDate, sStocksets);
		robot->printDepot();
		delete robot;
	}

	return 0;
};
