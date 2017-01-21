#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Trie.hpp"
#include "ScrabbleRack.hpp"
#include "ScrabbleGrid.hpp"
#include "ScrabblePlayer.hpp"

int main() {

	auto t = std::time(0);
	std::cout << "Initialized random generator with " << t << "\n\n";
	std::srand(t);

	std::ifstream dict("./assets/dic_fr.txt");
	Trie trie(dict);
	ScrabbleGrid grid(&trie);
	ScrabbleStack stack;

	std::cout << "Choose between automatic (type 'A') and manual (type 'M') rack : ";
	char c;
	std:: cin >> c;

	ScrabbleRack *rack;
	
	if(c == 'a' || c == 'A'){
		rack = new ScrabbleRack(&stack);
	}
	else if(c == 'm' || c == 'M'){
		rack = new ScrabbleManualRack();
	}
	else{
		std::cout << "Incorrect value, exiting.";
		return 0;
	}
	
	BestWordIA player(rack, &grid);
	
	while(player.playTurn()){
		std::cout << '\n' << grid << "\n\n\n";
	}

}
