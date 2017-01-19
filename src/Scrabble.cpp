#include <iostream>
#include <bitset>
#include "Trie.hpp"
#include "ScrabbleRack.hpp"
#include <cstdlib>
#include <ctime>

void print_boolean(unsigned int i){
	std::bitset<32> x(i);
	std::cout << x << '\n';
}

int main() {
	std::srand(std::time(0));
	ScrabbleStack stack;
	ScrabbleRack rack(&stack);

	do{
		std::cout << rack.convertToString() << '\n';
		rack.clear();
		rack.drawFromStack();
	} while(!rack.stackIsEmpty() || !rack.empty());

}
