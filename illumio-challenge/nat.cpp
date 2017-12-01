#include "NatProcessor.h";

const string NAT_FILE("NAT");
const string FLOW_FILE("FLOW");
const string OUTPUT_FILE("OUTPUT");

int main(int argc, char **argv) {
	NatProcessor proc;

	proc.ReadNatFile(NAT_FILE);
	proc.ReadFlowFile(FLOW_FILE, OUTPUT_FILE);
}