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
	ScrabbleManualRack rack;
	
	BestWordIA player(&rack, &grid);
	
	while(player.playTurn()){
		std::cout << '\n' << grid << "\n\n\n";
	}

}
