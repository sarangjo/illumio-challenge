#include <iostream>

#include "NatProcessor.h"

void usage() {
	std::cout << "usage: ./nat [nat_file] [flow_file] [output_file]" << std::endl;
}
int main(int argc, char **argv) {
	if (argc != 4) {
		usage();
		return 1;
	}

	NatProcessor proc;

	proc.ReadNatFile(string(argv[1]));
	proc.ReadFlowFile(string(argv[2]), string(argv[3]));
}