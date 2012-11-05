#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <set>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;
/*
const int INF = 100000000;
const int DIM = 1000;
vector<vector<double> > vin;

pair<double, int>
cross_correlation(const vector<double> &sa, const vector<double> &sb) {
	double ma = 0, mb = 0;

	if(sa.size() != sb.size())
		return make_pair<double,int>(-1, -INF);

	int len = sa.size();

	for(int i = 0; i < len; ++i) {
		ma += sa[i];
		mb += sb[i];
	}
	ma /= len;	
	mb /= len;

	double res = -1;
	int offset = -INF;

	double deta = 0;
	double detb = 0;
	for(int i = 0; i < len; ++i) {
		deta += (sa[i] - ma) * (sa[i] - ma);
		detb += (sb[i] - mb) * (sb[i] - mb);
	}

	deta = sqrt(deta * detb);
	// cout << "-len " << -(int)len << endl;
	for(int d = -len; d < len; ++d) {
		// cout << "test" << endl;
		double num = 0;
		for(int i = 0; i < len; ++i) {
			num += (sa[i] - ma) * (sb[((i + d) % len + len) %len] - mb);
		}

		double rd = num / deta;
		if(rd > res) {
			// cout << "rd: " << rd << endl;
			// cout << "offset: " << offset << endl;
			res = rd;
			offset = d;
		}
	}
	// cout << "res: " << res << endl;
	return make_pair<double, int>(res, offset);
}

void
load_in() {
	ifstream fin("fin");
	assert(fin.is_open() == true);
	string line;
	while(getline(fin, line)) {
		istringstream sline(line);
		
		vector<double> dims;
		double m;
		while(sline >> m) {
			dims.push_back(m);
		}
		vin.push_back(dims);
	}
	fin.close();
}

vector<double>
search(const vector<double> &in_signal) {
	double max_sim = -INF;
	int max_index = -INF;
	
	for(size_t j = 0; j < vin.size(); ++j) {
		printf("%d: processing...\r", j);
		pair<double, int> p = cross_correlation(in_signal, vin[j]);
		if(max_sim < p.first) {
			max_sim = p.first;
			max_index = j;
		}
	}
	printf("\n");
	assert(max_index != -INF);
	printf("%d - %lf\n", max_index, max_sim);
	return vin[max_index];
}

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
void
test() {
	assert(vin.size() > 0);
	print_vector(vin[0]);
	printf("search now...\n");
	vector<double> res = search(vin[0]);
	printf("search over...\n");
	print_vector(res);
}

int
main() {
	print_now();
	cout << "loading now..." << endl;
	load_in();
	print_now();
	cout << "loading over..." << endl;
	test();
	print_now();
	return 0;
}
*/