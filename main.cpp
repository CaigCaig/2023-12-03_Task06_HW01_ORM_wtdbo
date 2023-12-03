#include <iostream>
#include <Windows.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <string>
#pragma execution_character_set("utf-8")

using namespace std;
using namespace Wt::Dbo;
using namespace Wt::Dbo::backend;
namespace dbo = Wt::Dbo;

class publisher;
//class book;
class stock;
//class shop;
class sale;

class book
{
public:
	string title = "";
	dbo::ptr<publisher> id_publisher;
	dbo::weak_ptr<stock> stocks;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, title, "title");
		dbo::belongsTo(a, id_publisher);
		dbo::hasOne(a, stocks);
	}
};

class publisher
{
public:
	string name = "";
	dbo::weak_ptr<book> books;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		dbo::hasOne(a, books);
	}
};


class shop
{
public:
	string name = "";
	dbo::weak_ptr<stock> stocks;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		dbo::hasOne(a, stocks);
	}
};

class stock
{
public:
	int count = 0;
	dbo::ptr<book> id_book;
	dbo::ptr<shop> id_shop;
	dbo::weak_ptr<sale> sales;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, count, "count");
		dbo::belongsTo(a, id_book);
		dbo::belongsTo(a, id_shop);
		dbo::hasOne(a, sales);
	}
};

class sale
{
public:
	int count = 0;
	float price = 0.0;
	string date_sale = "";
	dbo::ptr<stock> id_stock;
	
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		dbo::belongsTo(a, id_stock);		
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	cout << "Hello!" << endl;

	try
	{
		string connectionString = "host = localhost"
			" port = 5432"
			" dbname = ORM_test"
			" user = user"
			" password = user";

		auto connection = make_unique<Postgres>(connectionString);

		Wt::Dbo::Session session;
		session.setConnection(move(connection));
		session.mapClass<publisher>("publisher");
		session.mapClass<book>("book");
		session.mapClass<stock>("stock");
		session.mapClass<shop>("shop");
		session.mapClass<sale>("sale");

		try	{session.createTables();} catch (...){}

		Wt::Dbo::Transaction t{ session };

		cout << "All ok!" << endl;
	}
	catch (const exception& e)
	{
		cout << "Error: " << e.what() << endl;
	}

	system("pause");
}