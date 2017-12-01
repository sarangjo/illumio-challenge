#pragma once

#include <unordered_map>
#include <fstream>

using namespace std;

class NatProcessor
{
public:
	NatProcessor();
	~NatProcessor();
	void ReadNatFile(string filename);
	void ReadFlowFile(string filename, string outputfile);
private:
	unordered_map<string, string> both_specified, ip_specified, port_specified;
	ofstream output;

	void handleNatLine(string);
	void handleFlowLine(string);
	void outputLine(string, unordered_map<string, string>::const_iterator);
	void readFile(string, void(NatProcessor::*handler)(string));
};
