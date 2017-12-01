#include "NatProcessor.h"

#include <string>

NatProcessor::NatProcessor()
{
}

NatProcessor::~NatProcessor()
{
}

// Utility function to split a string on `delim`
int split(string s, char delim, string &left, string &right) {
	size_t delim_index = s.find(delim);
	if (delim_index == string::npos) {
		return -1;
	}
	left = s.substr(0, delim_index);
	right = s.substr(delim_index + 1);
}

// Handles processing for a single line from the NAT input file.
// Processes the in->out mappings by placing them in the appropriate unordered_map.
void NatProcessor::handleNatLine(string line) {
	// Split into incoming and outgoing translation
	string in, out;
	split(line, ',', in, out);

	// Split into ip and port to make checking for wildcards easier
	string ip, port;
	split(in, ':', ip, port);

	// IP is wildcard, port specified
	if (!ip.compare("*")) {
		port_specified.emplace(port, out);
	}
	// port is wildcard, IP specified
	else if (!port.compare("*")) {
		ip_specified.emplace(ip, out);
	}
	// no wildcards, insert into standard table
	else {
		both_specified.emplace(in, out);
	}
}

// Utility function to output a single mapping based on our through the unordered_maps
// `in` is the string from the FLOW file, `found` is the iterator used to find the mapping
void NatProcessor::outputLine(string in, unordered_map<string, string>::const_iterator found) {
	this->output << in << " -> " << (*found).second << endl;
}

// Handles processing for a single line from the FLOW input file.
// Checks the line against our three maps to see which has been registered in the initial NAT stage.
void NatProcessor::handleFlowLine(string line) {
	unordered_map<string, string>::const_iterator found;

	// First check exact matches
	if ((found = both_specified.find(line)) != both_specified.end()) {
		outputLine(line, found);
		return;
	}

	// If not, check possible wildcard matches
	string ip, port;
	split(line, ':', ip, port);

	if ((found = ip_specified.find(ip)) != ip_specified.end()) {
		outputLine(line, found);
		return;
	}

	if ((found = port_specified.find(port)) != port_specified.end()) {
		outputLine(line, found);
		return;
	}

	this->output << "No nat match for " << line << endl;
}

// Reads a particular file, calling `handler` on each line
void NatProcessor::readFile(string filename, void(NatProcessor::*handler)(string)) {
	string line;
	ifstream inputfile(filename);

	if (inputfile.is_open()) {
		while (getline(inputfile, line)) {
			// This clever trick to dereference a member function pointer was borrowed from
			// https://stackoverflow.com/questions/23686456/calling-pointer-to-member-function-c
			(this->*handler)(line);
		}
		inputfile.close();
	}

	inputfile.close();
}

// Given a filename that represents the NAT file, loads the mapping information into the NAT processor.
// If this function is called multiple times, only the NAT mapping information from the latest call
// is stored.
void NatProcessor::ReadNatFile(string filename)
{
	// Clear old mappings
	this->both_specified.clear();
	this->ip_specified.clear();
	this->port_specified.clear();

	// Read the given file
	readFile(filename, &NatProcessor::handleNatLine);
}

// Given a filename that represents the FLOW file, interprets and outputs the NAT mappings to the given
// output file.
void NatProcessor::ReadFlowFile(string filename, string outputfile)
{
	this->output.open(outputfile);
	readFile(filename, &NatProcessor::handleFlowLine);
}
