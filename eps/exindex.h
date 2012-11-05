
#ifndef EPS_EXINDEX_XIAOE
#define EPS_EXINDEX_XIAOE

#include "block.h"
#include "node.h"
#include "utils.h"

/**
** This is the external index class.
** For build index & load index & find a specific query and return a Node*.
** @author rt77789/xiaoe.
*/

class ExternalIndex {
private:
	Node *root;
	vector<vector<double> > inData;
	int label_num;
	size_t node_num;
	FILE *fout, *findex;
	
	// Load input data into inData(in memory).
	bool loadInput(const char *_finName);
	
	// Build the node tree using inData, belong is the mark array;
	// finally return the Node*, it's root.
	size_t buildNodeTree(vector<size_t> &belong);
	
	// Compute the cross-correlation of sa and sb, and return the similarity & offset pair.
	pair<double, int> cross_correlation(const vector<double> &sa, const vector<double> &sb) const;
	
	// Get k random number from n, and return the vector of k.
	vector<int> rand_medoids(int k, int n);
	
	// Write a block into external file.
	size_t writeBlock(Block *block);
	
	// Read a block according to offset.
	Block* readBlock(size_t offset);
	
	// Load the index into a node-tree.
	Node* loadNodeTree(size_t offset);
	
	// Just find a specific query, and return the most similarity(in this approach) Node*.
	Node* find(Node *ptr, const vector<double> &query) const;
	
public:
	ExternalIndex();
	
	// Build the index and write into external file.
	bool buildIndex(const char *foutName, const char *finName);
	
	// Load the external index, store it into memory (Node-tree).
	bool loadIndex(const char *indexName);
	
	vector<double> find(const vector<double> &query) const;
};

#endif
