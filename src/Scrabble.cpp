#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Trie.hpp"
#include "ScrabbleRack.hpp"
#include "ScrabbleGrid.hpp"
#include "ScrabblePlayer.hpp"

int main() {
	std::srand(std::time(0));
	std::ifstream dict("./assets/dic_fr.txt");
	Trie trie(dict);
	ScrabbleGrid grid(&trie);
	ScrabbleStack stack;
	ScrabbleRack rack(&stack);
	
	BestWordIA player(&rack, &grid);
	
	while(player.playTurn()){
		std::cout << '\n' << grid << "\n\n\n";
	}

}
