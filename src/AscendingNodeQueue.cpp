#include <algorithm>
#include "AscendingNodeQueue.h"

void NodeList::push(Node* obj) {
	pq.insert(pq.begin(), obj);
	sortList();
}

//最小値である先頭要素をpopして返す
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

// 配列内の昇順ソート
// 比較演算子の通常のオーバーロードが言語仕様の厳格さ故に出来なかったのでかなり強引
// イテレータや比較演算関数オブジェクトの概念が難しい為、詳しくは別途検証用プログラムを参照されたし
void NodeList::sortList() {
	sort(pq.begin(), pq.end(), NodeCompare());
}