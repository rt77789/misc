
#include "exindex.h"
#include "node.h"
#include "utils.h"

#include <iostream>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <vector>
#include <set>
#include <cstdlib>
#include <fstream>
#include <sstream>


// construct function.
ExternalIndex::ExternalIndex():root(0), label_num(0),node_num(0) {
}

// Build index: finName is the input file name, foutName is the output file name (index file name).
bool
ExternalIndex::buildIndex(const char *foutName, const char *finName) {
	// First loadInput data into inData.
	if(!loadInput(finName)) return false;
	// Second build the tree-node structrue.
	vector<size_t> belong;
	for(size_t i = 0; i < inData.size(); ++i)
		belong.push_back(i);
	
	fout = fopen(foutName, "wb");
	
	if(NULL == fout) {
		perror("NULL == fout");
		return false;
	}
	
	int root_label = buildNodeTree(belong);
#ifdef DEBUG
	cout << "root_label: " << root_label << endl;
#endif
	// [ And write the tree-node structre(actually it's Blocks) into the external file.
	// [ It's a traveling process of the node-tree.
	// write block process included in buildNodeTree.

	// write root_label to the last size_t of index file.
	if(1 != fwrite(&root_label, sizeof(root_label), 1, fout)) {
		perror("1 != fwrite(&root_label, sizeof(root_label), 1, fout)");
		return false;
	}
	if(fout != NULL)
		fclose(fout);
	
	return true;
}

// Load the index file: indexName is the index file name.
bool
ExternalIndex::loadIndex(const char *indexName) {
	findex = fopen(indexName, "rb");
	if(NULL == findex) {
		perror("NULL == findex");
		return false;
	}
	// get the root_label.
	if(fseek(findex, - sizeof(int), SEEK_END)) {
		perror("fseek(findex, 0, SEEK_END)");
		return false;
	}
	//long index_size = ftell(findex);

	//size_t root_label = index_size / sizeof(Block) - 1;
	int root_label = INF;
	if(1 != fread(&root_label, sizeof(int), 1, findex)) {
		perror("1 != fread(&root_label, sizeof(size_t), 1, findex)");
		exit(0);
	}
#ifdef DEBUG
	cout << "root_label: " << root_label << endl;
#endif
	assert(root_label != INF);
	
	root = loadNodeTree(root_label);
	
	if(NULL != findex) {
		fclose(findex);
	}
	return true;
}

// Load node-tree into memory from external blocks.
Node*
ExternalIndex::loadNodeTree(size_t offset) {
	cout << "+" << endl;
	cout << offset << endl;
	Block *block = readBlock(offset);
	Node *node = new Node();

	assert(node != NULL);

	vector<double>* dims = node->get_pos();
	
	for(size_t i = 0; i < DIM_NUM; ++i)
		dims->push_back(block->dims[i]);
	
	//cout << "test" << endl;
	for(size_t i = 0; i < CH_NUM; ++i) {
		if(block->child[i] >= 0) {
			node->insert_child( loadNodeTree(block->child[i]) );
		}
	}
	cout << "-" << endl;
	// Free the block memory.
	if(block != NULL) {
		delete block;
	}
	
	return node;
}

// Find the most similarity signals for inputing query.
vector<double>
ExternalIndex::find(const vector<double> &query) const {
	Node *ptr = find(root, query);
	vector<double> res;
	vector<double> *dims = ptr->get_pos();
	for(size_t i = 0; i < DIM_NUM; ++i)
		res.push_back((*dims)[i]);
	return res;
}

// Find the most similarity Node for inputing query.
Node*
ExternalIndex::find(Node *ptr, const vector<double> &query) const {
	
	assert((size_t)ptr->get_dimension() == query.size());

	vector<Node*> *child = ptr->get_child();
	double sim = 0;
	Node *nn = NULL;

	for(size_t i = 0; i < child->size(); ++i) {

		vector<double> *pos = (*child)[i]->get_pos();
		pair<double, int> temp = cross_correlation(*pos, query);
		if(temp.first > sim) {
			sim = temp.first;
			nn = (*child)[i];
		}
		cout << "temp.sim: " << temp.first << "\ttemp.offset: " << temp.second << endl;
	}
	cout << string(80, '-') << endl;

	if(nn != NULL) {
		return find(nn, query);
	}
	
	vector<double> *pos = ptr->get_pos();

	pair<double, int> temp = cross_correlation(*pos, query);

	cout << "sim: " << temp.first << " - [ ";

	for(size_t i = 0; i < 	pos->size(); ++i) {
		cout << (*pos)[i] << " ";
	}
	cout << " ]\n";
	
	return ptr;
}

// Load the input data into memory.
bool
ExternalIndex::loadInput(const char *_finName) {
	
#ifdef DEBUG
	cout << "loadInput now...\n";
#endif

	ifstream fin(_finName);
	
	assert(fin.is_open() == true);
	
	string line;
	while(getline(fin, line)) {
		istringstream sline(line);
		
		vector<double> dims;
		string m;
		while(sline >> m) {
			dims.push_back(sci2double(m));
		}
		dims.reserve(dims.size());
		inData.push_back(dims);
	}
	fin.close();
	
	inData.reserve(inData.size());
#ifdef DEBUG
	cout << "loadInput over...\n";
#endif
	return true;
}

// Build Node Tree and return the current Node*.
size_t
ExternalIndex::buildNodeTree(vector<size_t> &belong) {
	int K = CH_NUM;
	
	assert(belong.size() > 0);
	assert(inData.size() > 0);
#ifdef DEBUG
	cout << "label: " << label_num++ << " " << belong.size() << endl;
#endif
	Block block;
	
	for(size_t j = 0; j < inData[0].size(); ++j) {
		block.dims[j] = 0;
		for(size_t i = 0; i < belong.size(); ++i) {
			block.dims[j] += inData[belong[i]][j];
		}
		block.dims[j] /= belong.size();
	}
	
	if(belong.size() > 1) {
		vector<size_t> rbelong[K];
		vector<int> medoids = rand_medoids(K, belong.size());
		vector<double> means[K];
		
		for(int i = 0; i < K; ++i) {
			means[i].assign(inData[ belong[medoids[i]] ].begin(), inData[ belong[medoids[i]] ].end() );
		}
		
		// Max iterative times.
		int iter_times = 5;
		double sl = 0;
		// Max square least Difference.
		double eps = 1;
		
		while(iter_times-- > 0) {
			for(int i = 0; i < K; ++i)
				rbelong[i].clear();
			
			double nsl = 0;
			
			for(size_t i = 0; i < belong.size(); ++i) {
				int index = belong[i];
				int mini = -1;
				// Max cross_correlation?
				double temp = -INF;
				for(int j = 0; j < K; ++j) {
					pair<double, int> res = cross_correlation(means[j], inData[index]);	
	
					// cout << i << ' ' << j << ' ' << res.first << ' ' << res.second << endl;
					if(temp < res.first) {
						temp = res.first;
						mini = j;
					}
				}
				assert(mini != -1);
				nsl += temp;
				
				rbelong[mini].push_back(index);
			}
			
			for(int i = 0; i < K; ++i) {
				for(size_t h = 0; h < means[i].size(); ++h) {
					means[i][h] = 0;
					for(size_t j = 0; j < rbelong[i].size(); ++j) {
						means[i][h] += inData[rbelong[i][j]][h];
					}
					means[i][h] /= rbelong[i].size();
				}
			}
			
			if(fabs(nsl - sl) < eps) break;
			sl = nsl;
		}

		size_t rbsum = 0;
		for(int i = 0; i < K; ++i) {
			// Mark -1 means no childs.
			block.child[i] = -1;
			if(rbelong[i].size() > 0) {
#ifdef DEBUG
				cout << "parent: " << label_num-1 << endl;
#endif			
				block.child[i] = buildNodeTree(rbelong[i]);
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

	size_t cur_label = writeBlock(&block);
	// cout << "cur_label: " << cur_label << endl;
	
	return cur_label;
}

// Get k random numbers from n.
vector<int>
ExternalIndex::rand_medoids(int k, int n) {
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

// Just compute the corss-correlation of sa an sb.
pair<double, int>
ExternalIndex::cross_correlation(const vector<double> &sa, const vector<double> &sb) const {
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

size_t
ExternalIndex::writeBlock(Block *block) {
	
	//memset(block->dims, 0, sizeof(block->dims));
	//memset(block->child, -1, sizeof(block->child));
	
	if(1 != fwrite(block, sizeof(Block), 1, fout)) {
		perror("1 != fwrite()");
		exit(0);
	}

	return node_num++;
}


Block*
ExternalIndex::readBlock(size_t offset) {
	Block *block = new Block();
	// Seek to offset postion, then read.
	
	if(fseek(findex, offset* sizeof(Block), SEEK_SET)) {
		perror("fseek(findex, offset * sizeof(Block), SEEK_SET)");
		exit(0);
	}
	
	// 358504562
	if(1 != fread(block, sizeof(Block), 1, findex)) {
		perror("1 != fread()");
		exit(0);
	}

	return block;
}

