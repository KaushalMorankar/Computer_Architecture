#ifndef TEST_H
#define TEST_H

#include <string>
using namespace std;

void processFile(const string &filename);
string addHex(const string& hex1, const string& hex2);
string hex_binary(const string& hex);
string decimalToHex(long long int decimalValue);
extern vector<string> validRegisters;
extern vector<pair<string,string>> memory;
string binary_hex(string a);
long long int hex_dec(const string& hexStr);
long long int hexToDecimal(const string& hex);
extern int d;
extern vector<pair<string,int>> labels;
string labeloutput(int n);
long long int subtractHex(const string& hex1, const string& hex2);
extern vector<string> tokensiwant;
extern int counter;
string hex_binary(const string& hex);

#endif
