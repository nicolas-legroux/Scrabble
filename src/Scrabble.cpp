#include <iostream>
#include <bitset>
#include "Trie.hpp"
#include "ScrabbleRack.hpp"
#include "ScrabbleGrid.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

void print_boolean(unsigned int i){
	std::bitset<32> x(i);
	std::cout << x << '\n';
}

int main() {
	std::srand(std::time(0));
	std::ifstream dict("dic_fr.txt");
	Trie trie(dict);
	ScrabbleGrid grid(&trie);

	while(true){

		std::cout << grid << "\n";

		grid.transpose(); 

		std::cout << grid << '\n';

		grid.transpose();

		std::cout << "Enter a word to put in the grid (or 'q' to quit) : ";
		std::string word;
		std::cin >> word;

		std::transform(word.begin(), word.end(), word.begin(), ::toupper);
		
		if(word == "Q"){
			return 0;
		}

		bool awaitingAnswer = true;
		bool horizontal = true;
		std::string h;

		while(awaitingAnswer){
			std::cout << "Is the word horizontal (y/n) ? ";
			std::cin >> h;
			std::transform(h.begin(), h.end(), h.begin(), ::toupper);
			if(h == "Y"){
				awaitingAnswer = false;
			}
			else if(h == "N"){
				awaitingAnswer = false;
				horizontal = false;
			}
		}

		int blank;
		std::cout << "Enter the blank index of the word (-1 if no blank) : ";
		std::cin >> blank;

		unsigned int row;
		std::cout << "Enter the row of the word : ";
		std::cin >> row;

		unsigned int column;
		std::cout << "Enter the column of the word : ";
		std::cin >> column;

		unsigned int u_blank = (blank >= 0) ? blank : INT_MAX;

		unsigned int score = grid.placeWord(word, row, column, !horizontal, {u_blank});

		std::cout << "The word '" << word << "' scores " << score << " points.\n\n";
	}
}
