#include "cppython.h"

using namespace CPPython;

std::vector<string> string::split(string ToFind, int MaxSplit)
{
	std::vector<string> found;
	string ToSplit = *this;
	for (long long i = 0; i < ToSplit.length(); i++)
	{
		if (MaxSplit >= 0 && found.size() >= MaxSplit)
			break;
		if (!strcmp(ToSplit.substr(i, ToFind.length()).c_str(), ToFind.c_str()))
		{
			found.push_back(ToSplit.substr(0, i));
			ToSplit = ToSplit.substr(i + 1);
			i = -1;
		}
	}

	if (!this->empty())
	{
		found.push_back(ToSplit);
	}

	return found;
}

std::vector<string> string::rsplit(string ToFind, int MaxSplit)
{
	std::vector<string> found;
	string ToSplit = *this;
	for (long long i = ToSplit.length() - 1; i >= 0; i--)
	{
		if (MaxSplit >= 0 && found.size() >= MaxSplit)
			break;
		if (!strcmp(ToSplit.substr(i, ToFind.length()).c_str(), ToFind.c_str()))
		{
			found.insert(found.begin(), ToSplit.substr(i + 1));
			ToSplit = ToSplit.substr(0, i);
			i = ToSplit.length();
		}
	}
	if (!ToSplit.empty())
	{
		found.insert(found.begin(), ToSplit);
	}

	return found;
}

string string::lower(string s)
{
	string new_string("");
	for (int i = 0; i < s.length(); i++)
	{
		new_string += std::tolower(s[i]);
	}
	return new_string;
}

string string::upper(string s)
{
	string new_string("");
	for (int i = 0; i < s.length(); i++)
	{
		new_string += std::toupper(s[i]);
	}
	return new_string;
}