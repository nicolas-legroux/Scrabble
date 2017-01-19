#include "ScrabbleRack.hpp"
#include <algorithm>

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

std::string ScrabbleRack::convertToString() const {
	std::string s;
	for(unsigned int i=0; i<rack.size()-1; ++i){
		for(unsigned int j=0; j<rack[i]; ++j){
			s.push_back('A' + i);
			s.push_back(' ');
		}
	}

	for(unsigned int j=0; j<rack.back(); ++j){
		s.push_back('*');
		s.push_back(' ');
	}

	return s;
}
