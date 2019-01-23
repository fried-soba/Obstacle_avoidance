#pragma once

#include "Node.h"

// �\�[�g�Ɏg����r���Z�֐��I�u�W�F�N�g
struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->score < b->score;
	}
};

class NodeList {
private:
	vector<Node*> pq;
	Node* tmp;
public:
	void push(Node* obj);
	Node* pop();
	Node* top();
	int size();
	bool empty();
	void sortList();
};