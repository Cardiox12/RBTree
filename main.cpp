#include <iostream>
#include "RBTree.hpp"

int main() {
	RBTree<int> t;

	t.insert(5);
	t.insert(1);
	t.insert(10);
	t.insert(6);
	t.insert(15);
	return (0);
}