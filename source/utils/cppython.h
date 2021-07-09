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
		string(char c) : std::string(_char_to_charptr(c)) {};

		// Python functions
		std::vector<string> split(string ToFind = " ", int MaxSplit = -1); // Need a split function that is "OR" instead of "AND".
		std::vector<string> rsplit(string ToFind = " ", int MaxSplit = -1);
		string strip(string ToStrip = "\n\t\r ");
		char index(int i) { return i >= 0 ? std::string::operator[](i) : std::string::operator[](this->length() - 1); }
		string lower();
		string upper();
		string join(string*, uint64_t size);
		string join(std::vector<string> vec) { return this->join(&vec[0], vec.size()); }
		bool startswith(string Start);
		bool endswith(string End);

		// Convertors
		operator std::string() { return *this; }
		operator const char* () { return this->c_str(); }

		// Operators
		char operator[](int i) { return index(i); }
		
	};

}
