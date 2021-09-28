#include <iostream>
#include "RBTree.hpp"

int main() {
	RBTree<int> t;

	t.insert(5);
	t.print();
	std::cout << "============================================" << std::endl;
	t.insert(1);
	t.print();
	std::cout << "============================================" << std::endl;
	t.insert(10);
	t.print();
	std::cout << "============================================" << std::endl;
	t.insert(6);
	t.print();
	std::cout << "============================================" << std::endl;
	t.insert(15);
	t.print();
	std::cout << "============================================" << std::endl;
	t.insert(20);
	t.print();
	std::cout << "============================================" << std::endl;
	t.insert(25);
	t.print();
	std::cout << "============================================" << std::endl;
	return (0);
}