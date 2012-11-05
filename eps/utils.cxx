
#include "utils.h"

using namespace std;

/**
** This file includes some util-functions.
** 
** @author rt77789/xiaoe.
*/

void
print_vector(const vector<double> &v) {
	
	for(size_t i = 0; i < v.size(); ++i) {
		cout << v[i];
		if(i + 1 == v.size())
			cout << "\n";
		else
			cout << "\t";
	}
}

void
print_now() {
	time_t t = time(0); 
    char tmp[64]; 
    strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t)); 
    puts(tmp);
}

// Science number denoting -> double.
double
sci2double(string sci) {
	size_t i = sci.find('e');
	double res = 0;
	if(i != string::npos) {
		res = atof(sci.substr(0, i).c_str());
		res *= pow(10., atof(sci.substr(i+1).c_str()));
	}
	else {
		res = atof(sci.c_str());
	}
	
	return res;
}