#include "Thing.h"
#include <string>

using namespace std;

string Thing::GetName()
{
	return Name;
}
void Thing::SetName(string Name)
{
	this->Name = Name;
}
string Thing::GetAdress()
{
	return Adress;
}
void Thing::SetAdress(string Adress)
{
	this->Adress = Adress;
}
string Thing::GetType()
{
	return Type;
}
void Thing::SetType(string Type)
{
	this->Type = Type;
}
bool Thing::operator==(Thing& other)
{
	if (this->GetName() == other.GetName() && this->GetAdress() == other.GetAdress() && this->GetType() == other.GetType())
	{
		return true;
	}
	else
	{
		return false;
	}
}
