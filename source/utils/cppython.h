#pragma once
#include <iostream>
#include <vector>
#include <string>

//using namespace std;

namespace CPPython
{
	class string : public std::string
	{
	private:
		std::string _char_to_charptr(char c) { char C[1] = { c }; return std::string(C); }

	public:
		// Constructors
		string(const string& s) : std::string(s) {};
		string(const std::string& s) : std::string(s) {};
		string(char* c) : std::string(c) {};
		string(char c) : std::string(_char_to_charptr(c)) {}

		// Python functions
		std::vector<string> split(string ToFind = " ", int MaxSplit = -1);
		std::vector<string> rsplit(string ToFind = " ", int MaxSplit = -1);
		char index(int i) { return i >= 0 ? std::string::operator[](i) : std::string::operator[](this->length() - 1); }
		string lower(string s);
		string upper(string s);

		// Convertors
		operator std::string() { return *this; }

		// Operators
		char operator[](int i) { return index(i); }
		
	};

}
