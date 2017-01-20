#include "ScrabbleRack.hpp"
#include <algorithm>
#include <cassert>

ScrabbleStack::ScrabbleStack(const std::vector<int> &letter_distribution){
	for(unsigned int i=0; i<letter_distribution.size(); ++i){
		for(int j=0; j<letter_distribution[i]; ++j){
			stack.push_back('A' + i);
		}
	}

	start = 0;
	end = stack.size();

	shuffle();
}

void ScrabbleStack::shuffle() {
	std::random_shuffle(stack.begin() + start, stack.begin() + end);
}

std::vector<char> ScrabbleStack::drawLetters(unsigned int n){
	n = std::min(n, lettersRemaining());
	std::vector<char> letters;
	for(unsigned int i=0; i<n; ++i){
		letters.push_back(stack[start++]);
	}
	return letters;
}

void ScrabbleRack::drawFromStack(){
	unsigned int n = 7 - size;
	std::vector<char> letters = stack->drawLetters(n);
	for(char l : letters){
		++rack[l-'A'];
	}
	size += letters.size();
}

void ScrabbleRack::clear() {
	for(unsigned int i=0; i<rack.size(); ++i){
		rack[i] = 0;
	}
	size = 0;
}

void ScrabbleRack::remove(const std::vector<char> &letters){
	for(char c : letters){
		assert(rack[c - 'A'] > 0);
		--rack[c - 'A'];
		--size;
	}
}

std::ostream& operator<<(std::ostream &os, const ScrabbleRack &rack){
	for(unsigned int i=0; i<rack.rack.size()-1; ++i){
		for(unsigned int j=0; j<rack.rack[i]; ++j){
			char c = 'A' + i;
			os << c;
		}
	}

	for(unsigned int j=0; j<rack.rack.back(); ++j){
		os << '*';
	}

	return os;
}
