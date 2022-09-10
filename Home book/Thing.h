#pragma once
#include <string>

using namespace std;

class Thing
{
public:

	string GetName();
	void SetName(string Name);
	string GetAdress();
	void SetAdress(string Adress);
	string GetType();
	void SetType(string Type);
	bool operator == (Thing& other);

private:
	string Name;
	string Adress;
	string Type;
};

