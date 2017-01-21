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

void ScrabbleRack::draw(){
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

void ScrabbleManualRack::draw() {
	std::cout << "The rack is currently missing " << (7 - size) << " letters. " <<
		"Add the following to the rack : ";
	std::string s;
	std::cin >> s;
	if(s.size() > (7-size)){
		s.resize(7-size);
	}
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	for(char c : s){
		if(c == '*'){
			addBlank();
		}
		else{
			addLetter(c);
		}
	}
	if(size < 7){
		std::cout << "Stack is now empty.\n";
		emptyStack = true;
	}
}

void ScrabbleManualRack::remove() {
	std::cout << "Type the letters to remove from the rack : ";
	std::string s;
	std::cin >> s;
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	for(char c : s){
		if(c == '*'){
			removeBlank();
		}
		else{
			removeLetter(c);
		}
	}
}

std::ostream& operator<<(std::ostream &os, const ScrabbleRack &rack){
	os << '{';
	for(unsigned int i=0; i<rack.rack.size()-1; ++i){
		for(unsigned int j=0; j<rack.rack[i]; ++j){
			char c = 'A' + i;
			os << c;
		}
	}

	for(unsigned int j=0; j<rack.rack.back(); ++j){
		os << '*';
	}
	os << '}';
	return os;
}


