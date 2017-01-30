#include <algorithm>
#include <cassert>

#include <sstream>
#include "ScrabbleGrid.hpp"
#include "ScrabbleRack.hpp"

ScrabbleGrid::ScrabbleGrid(Trie *t) : trie(t) {
	std::fill(defaultGrid.begin(), defaultGrid.end(), 0);

	// Fill in Triple words
	for(unsigned int i=0; i<15; i += 7){
		for(unsigned int j=0; j<15; j += 7){
			getDefaultGrid(i, j) = TRIPLE_WORD;
		}
	}

	// Fill in Double words
	for(unsigned int i=1; i<14; ++i){
		getDefaultGrid(i, i) = DOUBLE_WORD;
		getDefaultGrid(i, 14-i) = DOUBLE_WORD;
	}

	// Fill in Triple characters
	for(unsigned int i=1; i<15; i += 4){
		for(unsigned int j=1; j<15; j += 4){
			if(getDefaultGrid(i, j) == 0){
				getDefaultGrid(i, j) = TRIPLE_CHAR;
			}
		}
	}
	getDefaultGrid(5, 5) = TRIPLE_CHAR;
	getDefaultGrid(5, 9) = TRIPLE_CHAR;
	getDefaultGrid(9, 5) = TRIPLE_CHAR;
	getDefaultGrid(9, 9) = TRIPLE_CHAR;

	// Fill in double characters
	getDefaultGrid(0, 3) = DOUBLE_CHAR;
	getDefaultGrid(0, 11) = DOUBLE_CHAR;
	getDefaultGrid(14, 3) = DOUBLE_CHAR;
	getDefaultGrid(14, 11) = DOUBLE_CHAR;
	getDefaultGrid(2, 6) = DOUBLE_CHAR;
	getDefaultGrid(2, 8) = DOUBLE_CHAR;
	getDefaultGrid(12, 6) = DOUBLE_CHAR;
	getDefaultGrid(12, 8) = DOUBLE_CHAR;
	getDefaultGrid(3, 0) = DOUBLE_CHAR;
	getDefaultGrid(3, 7) = DOUBLE_CHAR;
	getDefaultGrid(3, 14) = DOUBLE_CHAR;
	getDefaultGrid(11, 0) = DOUBLE_CHAR;
	getDefaultGrid(11, 7) = DOUBLE_CHAR;
	getDefaultGrid(11, 14) = DOUBLE_CHAR;
	getDefaultGrid(6, 2) = DOUBLE_CHAR;
	getDefaultGrid(6, 6) = DOUBLE_CHAR;
	getDefaultGrid(6, 8) = DOUBLE_CHAR;
	getDefaultGrid(6, 12) = DOUBLE_CHAR;
	getDefaultGrid(8, 2) = DOUBLE_CHAR;
	getDefaultGrid(8, 6) = DOUBLE_CHAR;
	getDefaultGrid(8, 8) = DOUBLE_CHAR;
	getDefaultGrid(8, 12) = DOUBLE_CHAR;
	getDefaultGrid(7, 3) = DOUBLE_CHAR;
	getDefaultGrid(7, 11) = DOUBLE_CHAR;

	makeNewGrid();
}

void ScrabbleGrid::makeNewGrid(){
	// Set the grid using the initial grid
	std::copy(defaultGrid.cbegin(), defaultGrid.cend(), grid.begin());
	
	// Set the crosschecks
	std::fill(horizontalCrosschecks.begin(), horizontalCrosschecks.end(), Crosscheck());
	std::fill(verticalCrosschecks.begin(), verticalCrosschecks.end(), Crosscheck());
	hzCrosschecks = &horizontalCrosschecks;
	vcCrosschecks = &verticalCrosschecks;
	// Set the initial anchor
	getHorizontalCrosscheck(7, 7).makeValid();
	for(char c='A'; c <= 'Z'; ++c){
		getHorizontalCrosscheck(7, 7).set(c);
	}
	
	transposed = false;

	blanks.clear();	
}

void ScrabbleGrid::reset(const std::vector<std::pair<unsigned int, unsigned int>> &positions){
	for(const auto &position : positions){
		get(position.first, position.second) = getDefaultGrid(position.first, position.second);
		blanks.erase({position.first, position.second});
	}

	computeCrosschecks();

	if(isAvailable(7, 7)){
		getHorizontalCrosscheck(7, 7).makeValid();
		for(char c='A'; c <= 'Z'; ++c){
			getHorizontalCrosscheck(7, 7).set(c);
		}
	}
}

std::ostream& operator<<(std::ostream &os, const ScrabbleGrid &g){
	for(unsigned int i=0; i < 15*15; ++i){
		if(g.grid[i] == 0){
			os << ' ';
		}
		else if(g.grid[i] == DOUBLE_CHAR){
			os << '+';
		}
		else if(g.grid[i] == TRIPLE_CHAR){
			os << '*';
		}
		else if(g.grid[i] == DOUBLE_WORD){
			os << '2';
		}
		else if(g.grid[i] == TRIPLE_WORD){
			os << '3';
		}
		else{
			os << g.grid[i];
		}
		if((i+1)%15 == 0){
			os << '\n';
		}
	}	

	if(!g.blanks.empty()){
		std::cout << "\nBlanks : ";
		for(auto blank : g.blanks){
			char row = 'A' + blank.first;
			int column = blank.second;
			std::cout << row << column << ' ';
		}
		std::cout << '\n';
	}
	/*
	std::cout << "Horizontal crosschecks :\n";

	for(unsigned int row = 0; row < 15; ++row){
		for(unsigned int column = 0; column < 15; ++column){
			const auto &cc = g.getHorizontalCrosscheck(row, column);
			if(cc.valid()){
				os << "\t(" << row << ',' << column << ") : " << cc << '\n';
			}
		}
	}

	std::cout << "Vertical crosschecks :\n";
	for(unsigned int row = 0; row < 15; ++row){
		for(unsigned int column = 0; column < 15; ++column){
			const auto &cc = g.getVerticalCrosscheck(row, column);
			if(cc.valid()){
				os << "\t(" << row << ',' << column << ") : " << cc << '\n';
			}
		}
	}
	*/
		
	return os;
}

std::ostream& operator<<(std::ostream &os, const ScrabbleGrid::Crosscheck &crosscheck){
	os << '{';
	for(char c='A'; c <= 'Z'; ++c){
		if(crosscheck.check(c)){
			os << c;
		}
	}
	os << '}';
	return os;
}

void ScrabbleGrid::transpose(){
	for(unsigned int row=0; row<15; ++row){
		for(unsigned int column = row+1; column<15; ++column){
			std::swap(get(row, column), get(column, row));
		}
	}
	std::swap(hzCrosschecks, vcCrosschecks);
	transposed = !transposed;	
}

std::pair<unsigned int, std::vector<unsigned int>> ScrabbleGrid::computeScore(
		const std::string &word, unsigned int row, unsigned int column, 
		bool doTranspose, const std::set<unsigned int> &blanks, bool calledFromPlace){
	unsigned int adjacentScore = 0;
	unsigned int wordScore = 0;
	unsigned int wordMultiplier = 1;
	std::vector<unsigned int> positionsFromRack;
	
	if(doTranspose){
		transpose();
		std::swap(row, column);
	}

	for(unsigned int i=0; i < word.size(); ++i){

		assert(row >= 0 && row < 15 && column >= 0 && column <= 15);

		if(isAvailable(row, column)){
			
			// Add to current wordScore

			unsigned int characterScore = 0;
			if(calledFromPlace)
				std::cout << i << ' ';
			
			positionsFromRack.push_back(i);

			// Not a blank
			if(blanks.find(i) == blanks.end()){
				characterScore = LETTER_POINTS_FR[word[i] - 'A'];
			}

			if(get(row, column) == DOUBLE_CHAR){
				characterScore *= 2;
			}
			else if(get(row, column) == TRIPLE_CHAR){
				characterScore *= 3;
			}

			wordScore += characterScore;

			if(calledFromPlace){
				std::cout << characterScore << ' ';
			}

			if(get(row, column) == DOUBLE_WORD){
				wordMultiplier *= 2;
			}
			else if(get(row, column) == TRIPLE_WORD){
				wordMultiplier *= 3;
			}

			// Compute the adjacent score
			
			auto adjacentS = computeAdjacentScore(row, column);
			if(adjacentS.second){
				unsigned int adjacent_multiplier = 1;
				if(get(row, column) == DOUBLE_WORD){
					adjacent_multiplier *= 2;
				}
				else if(get(row, column) == TRIPLE_WORD){
					adjacent_multiplier *= 3;
				}
				unsigned int verticalScore = adjacent_multiplier * (adjacentS.first + characterScore);
				
				if (calledFromPlace)
					std::cout << adjacentS.first << ' ' << adjacent_multiplier << ' ' << verticalScore;

				adjacentScore += verticalScore;
			}
			
			if(calledFromPlace)
				std::cout << '\n';
		}

		else{
			assert(get(row,column) == word[i]);

			if(!isBlank(row, column)){
				wordScore += LETTER_POINTS_FR[word[i] - 'A'];
			}
		}

		++column;
	}

	if(doTranspose){
		transpose();
	}

	if(positionsFromRack.size()==7){
		/* Scrabble ! */
		adjacentScore += 50;
	}

	return {adjacentScore + wordScore * wordMultiplier, positionsFromRack};
}

std::pair<unsigned int, std::vector<unsigned int>> ScrabbleGrid::placeWord(const std::string &word, 
		unsigned int row, unsigned int column, bool doTranspose, const std::set<unsigned int> &blanks){
	if(doTranspose){
		transpose();
		std::swap(row, column);
	}

	auto returnData = computeScore(word, row, column, false, blanks, true);
	
	for(unsigned int i=0; i < word.size(); ++i){

		if(isAvailable(row, column)){
			// Should a blank be added to the grid ?
			if(blanks.find(i) != blanks.end()){
				addBlank(row, column);
			}
		}

		get(row, column) = word[i];
		
		++column;
	}

	if(doTranspose){
		transpose();
	}

	// Compute the crosschecks
	computeCrosschecks();

	return returnData;
}

std::pair<unsigned int, bool> ScrabbleGrid::computeAdjacentScore(unsigned int row, unsigned int column){
	bool lettersAbove = true;
	bool lettersBelow = true;

	if(row == 0 || isAvailable(row-1, column)){
		lettersAbove = false;
	}
	if(row == 14 || isAvailable(row+1, column)){
		lettersBelow = false;
	}

	bool hasAdjacentScore = lettersBelow || lettersAbove;

	if(!hasAdjacentScore){
		return {0, false};
	}
	else{
		unsigned int score = 0;
		if(lettersAbove){
			for(int i = row-1; i >=0; --i){
				if(isAvailable(i, column)){
					break;
				}
				if(!isBlank(i, column)){
					score += LETTER_POINTS_FR[get(i, column) - 'A'];
				}
			}
		}

		if(lettersBelow){
			for(int i = row+1; i < 15; ++i){
				if(isAvailable(i, column)){
					break;
				}
				if(!isBlank(i, column)){
					score += LETTER_POINTS_FR[get(i, column) - 'A'];
				}
			}
		}

		return {score, true};
	}
}

void ScrabbleGrid::computeCrosscheck(Crosscheck &crossCheck, const std::pair<std::string, std::string> &ps){
	const std::string &prefix = ps.first;
	const std::string &suffix = ps.second;
	
	if(!prefix.empty() || !suffix.empty()){
		crossCheck.makeValid();
	}

	unsigned int node = 0;
	if(!prefix.empty()){
		node = trie->findPrefix(prefix);
		if(node == 0){
			return; 
		}
	}

	if(node != 0 || !suffix.empty()){
		for(char c = 'A'; c <= 'Z'; ++c){
			std::stringstream ss;
			ss << c << suffix;
			if(trie->checkSuffix(node, ss.str())){
				crossCheck.set(c);
			}
		}
	}
}

std::pair<std::string, std::string> ScrabbleGrid::findPrefixSuffix(unsigned int row, 
		unsigned int column){
	std::string prefix;
	std::string suffix;
	
	unsigned int before = row;
	while(before > 0 && !isAvailable(--before, column)){
		prefix.push_back(get(before, column));
	}
	unsigned int after = row;
	while(after < 14 && !isAvailable(++after, column)){
		suffix.push_back(get(after, column));
	}
	std::reverse(prefix.begin(), prefix.end());
	
	return {prefix, suffix};
}

void ScrabbleGrid::computeCrosscheck(unsigned int row, unsigned int column){
	Crosscheck &crosscheck = getHorizontalCrosscheck(row, column);
	crosscheck.clear();
	if(isAvailable(row, column)){
		computeCrosscheck(crosscheck, findPrefixSuffix(row, column));
	}
}

void ScrabbleGrid::computeCrosschecks(){
	for(unsigned int row = 0; row<15; ++row){
		for(unsigned int column=0; column<15; ++column){
			computeCrosscheck(row, column);
		}
	}

	transpose();
	for(unsigned int row=0; row<15; ++row){
		for(unsigned int column = 0; column < 15; ++column){
			computeCrosscheck(row, column);
		}
	}
	transpose();
}

unsigned int ScrabbleGrid::firstAvailableColumn(unsigned int row){
	for(unsigned int column = 0; column < 15; ++column){
		if(isAvailable(row, column)){
			return column;
		}
	}
	return 15;
}
