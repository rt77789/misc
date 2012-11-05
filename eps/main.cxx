#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>

#include "exindex.h"
#include "utils.h"

using namespace std;

ExternalIndex ei;

void
build() {
	print_now();
	ei.buildIndex("test_out", "filt3.wavelet.eps");
	print_now();
}

void
test() {
	print_now();
	ei.loadIndex("test_out");
	
	ifstream in("filt3.wavelet.eps");
	assert(in.is_open() == true);
	
	string line;
	
	while(getline(in, line)) {
		istringstream ss(line);
		vector<double> signal;
		string sig;
		while(ss >> sig) {
			signal.push_back(sci2double(sig));
		}
		/*
		for(size_t i = 0; i < DIM_NUM; ++i)
			signal.push_back(sin(i * 0.77 * acos(-1.)));
		*/
		print_vector(signal);
		print_now();
		vector<double> ssig = ei.find(signal);
		print_now();
		print_vector(ssig);
		getchar();
	}
}

int
main() {
	printf("%x\n", 358504562);  //155e5872
	printf("%x\n", 130);  //155e5872
	build();
	//test();

	return 0;
}
