#include <algorithm>
#include "AscendingNodeQueue.h"

void NodeList::push(Node* obj) {
	pq.insert(pq.begin(), obj);
	sortList();
}

//�ŏ��l�ł���擪�v�f��pop���ĕԂ�
Node* NodeList::pop() {
	tmp = pq[0];
	pq.erase(pq.begin());
	return tmp;
}

Node* NodeList::top() {
	return pq.front();
}

int NodeList::size() {
	return pq.size();
}

bool NodeList::empty() {
	return pq.empty();
}

// �z����̏����\�[�g
// ��r���Z�q�̒ʏ�̃I�[�o�[���[�h������d�l�̌��i���̂ɏo���Ȃ������̂ł��Ȃ苭��
// �C�e���[�^���r���Z�֐��I�u�W�F�N�g�̊T�O������ׁA�ڂ����͕ʓr���ؗp�v���O�������Q�Ƃ��ꂽ��
void NodeList::sortList() {
	sort(pq.begin(), pq.end(), NodeCompare());
}