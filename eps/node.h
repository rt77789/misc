
#ifndef EPS_NODE_XIAOE
#define EPS_NODE_XIAOE

#include <vector>
#include <iostream>
using namespace std;

/**
** This is the tree-node structure in memory.
** 
** @author rt77789/xiaoe.
*/

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

#endif
