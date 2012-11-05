#include <iostream>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <vector>
#include <set>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

const int INF = 100000000;
int label_num = 0;
vector<vector<double> > vin;

struct Node {
	// 199998 -> root_label.
	// [label|dims|childs] -> a block, write the block into the external space.
	
	vector<double> pos;
	vector<Node*> child;
	
	Node() {}
	Node(const vector<double> &_pos) {
		pos.assign(_pos.begin(), _pos.end());
	}

	void insert_child(Node* _ch) {
		child.push_back(_ch);
	}
	void assign(vector<double> &_pos) {
		pos.assign(_pos.begin(), _pos.end());
	}

	int get_dimension() const {
		return pos.size();
	}
	
	vector<double>* get_pos() {
		return &pos;
	}
	
	vector<Node*>* get_child() {
		return &child;
	}
};

vector<int>
rand_medoids(int k, int n) {
	int seeds[n];
	srand(time(0));

	for(int i = 0; i < n; ++i) {
		seeds[i] = i;
	}
	
	for(int i = 0; i < k && i < n; ++i) {
		int index = rand() % (n-i) + i;

		int temp = seeds[i];
		seeds[i] = seeds[index];
		seeds[index] = temp;
	}

	vector<int> res;

	for(int i = 0; i < k && i < n; ++i) {
		res.push_back(seeds[i]);
	}
	
	return res;
}


pair<double, int>
cross_correlation(const vector<double> &sa, const vector<double> &sb) {
	double ma = 0, mb = 0;

	assert(sa.size() == sb.size());

	int len = sa.size();

	for(int i = 0; i < len; ++i) {
		ma += sa[i];
		mb += sb[i];
	}

	ma /= len;	
	mb /= len;

	double res = -INF;
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
			res = rd;
			offset = d;
		}
	}
	// cout << "res: " << res << endl;
	return make_pair<double, int>(res, offset);
}

Node*
build_tree(int K, vector<int> &belong, vector<vector<double> > &sigs) {

	assert(belong.size() > 0);
	assert(sigs.size() > 0);

	cout << "label: " << label_num++ << " " << belong.size() << endl;

	Node *ptr = new Node();
	vector<double> mean(sigs[0].size(), 0);

	for(size_t j = 0; j < sigs[0].size(); ++j) {
		for(size_t i = 0; i < belong.size(); ++i) {
			mean[j] += sigs[belong[i]][j];
		}
		mean[j] /= belong.size();
	}
	ptr->assign(mean);

	if(belong.size() > 1) {

		vector<int> rbelong[K];
		vector<int> medoids = rand_medoids(K, belong.size());

		for(size_t i = 0; i < belong.size(); ++i) {
			int index = belong[i];
			int mini = -1;
			// Max cross_correlation?
			double temp = -INF;
			for(int j = 0; j < K; ++j) {
				pair<double, int> res = cross_correlation(sigs[ belong[medoids[j]] ], sigs[index]);	

				// cout << i << ' ' << j << ' ' << res.first << ' ' << res.second << endl;
				if(temp < res.first) {
					temp = res.first;
					mini = j;
				}
			}
			assert(mini != -1);

			rbelong[mini].push_back(index);
		}

		size_t rbsum = 0;
		for(int i = 0; i < K; ++i) {
			if(rbelong[i].size() > 0) {
				cout << "parent: " << label_num-1 << endl;
				Node *ch = build_tree(K, rbelong[i], sigs);

				ptr->insert_child(ch);
			}
			else {
				perror("rbelong[i].size() <= 0\n");
			}
			rbsum += rbelong[i].size();
		}
		assert(rbsum == belong.size());
	}
	else if(belong.size() == 1) {
		//
	}
	
	return ptr;
}	

vector<double>
find_similar(Node *ptr, const vector<double> &sa) {
	assert((size_t)ptr->get_dimension() == sa.size());

	vector<Node*> *child = ptr->get_child();
	double sim = 0;
	Node *nn = NULL;

	for(size_t i = 0; i < child->size(); ++i) {

		vector<double> *pos = (*child)[i]->get_pos();
		pair<double, int> temp = cross_correlation(*pos, sa);
		if(temp.first > sim) {
			sim = temp.first;
			nn = (*child)[i];
		}
		cout << "temp.sim: " << temp.first << "\ttemp.offset: " << temp.second << endl;
	}
	cout << string(80, '-') << endl;

	if(nn != NULL) {
		return find_similar(nn, sa);
	}
	
	vector<double> *pos = ptr->get_pos();

	pair<double, int> temp = cross_correlation(*pos, sa);

	cout << "sim: " << temp.first << " - [ ";

	for(size_t i = 0; i < 	pos->size(); ++i) {
		cout << (*pos)[i] << " ";
	}
	cout << " ]\n";
	
	return *pos;
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
		dims.reserve(dims.size());
		vin.push_back(dims);
	}
	fin.close();
	
	vin.reserve(vin.size());
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
	
	vector<int> belong;
	for(size_t i = 0; i < vin.size(); ++i)
		belong.push_back(i);

	Node *root = build_tree(2, belong, vin);
	
	vector<double> res = find_similar(root, vin[0]);
	
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
