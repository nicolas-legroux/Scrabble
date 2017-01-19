#include <algorithm>
#include <cassert>
#include <sstream>
#include "ScrabbleGrid.hpp"

void ScrabbleGrid::makeNewGrid(){
	std::fill(grid.begin(), grid.end(), 0);
	std::fill(horizontalCrosschecks.begin(), horizontalCrosschecks.end(), Crosscheck());
	std::fill(verticalCrosschecks.begin(), verticalCrosschecks.end(), Crosscheck());

	hzCrosschecks = &horizontalCrosschecks;
	vcCrosschecks = &verticalCrosschecks;
	
	// Fill in Triple words
	for(unsigned int i=0; i<15; i += 7){
		for(unsigned int j=0; j<15; j += 7){
			get(i, j) = TRIPLE_WORD;
		}
	}

	// Fill in Double words
	for(unsigned int i=1; i<14; ++i){
		get(i, i) = DOUBLE_WORD;
		get(i, 14-i) = DOUBLE_WORD;
	}

	// Fill in Triple characters
	for(unsigned int i=1; i<15; i += 4){
		for(unsigned int j=1; j<15; j += 4){
			if(get(i, j) == 0){
				get(i, j) = TRIPLE_CHAR;
			}
		}
	}
	get(5, 5) = TRIPLE_CHAR;
	get(5, 9) = TRIPLE_CHAR;
	get(9, 5) = TRIPLE_CHAR;
	get(9, 9) = TRIPLE_CHAR;

	// Fill in double characters
	get(0, 3) = DOUBLE_CHAR;
	get(0, 11) = DOUBLE_CHAR;
	get(14, 3) = DOUBLE_CHAR;
	get(14, 11) = DOUBLE_CHAR;
	get(2, 6) = DOUBLE_CHAR;
	get(2, 8) = DOUBLE_CHAR;
	get(12, 6) = DOUBLE_CHAR;
	get(12, 8) = DOUBLE_CHAR;
	get(3, 0) = DOUBLE_CHAR;
	get(3, 7) = DOUBLE_CHAR;
	get(3, 14) = DOUBLE_CHAR;
	get(11, 0) = DOUBLE_CHAR;
	get(11, 7) = DOUBLE_CHAR;
	get(11, 14) = DOUBLE_CHAR;
	get(6, 2) = DOUBLE_CHAR;
	get(6, 6) = DOUBLE_CHAR;
	get(6, 8) = DOUBLE_CHAR;
	get(6, 12) = DOUBLE_CHAR;
	get(8, 2) = DOUBLE_CHAR;
	get(8, 6) = DOUBLE_CHAR;
	get(8, 8) = DOUBLE_CHAR;
	get(8, 12) = DOUBLE_CHAR;
	get(7, 3) = DOUBLE_CHAR;
	get(7, 11) = DOUBLE_CHAR;

	// Set the initial anchor
	for(char c='A'; c <= 'Z'; ++c){
		getHorizontalCrosscheck(7, 7).set(c);
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

unsigned int ScrabbleGrid::computeScore(const std::string &word, unsigned int row, 
		unsigned int column, bool doTranspose, const std::set<unsigned int> &blanks){
	unsigned int adjacentScore = 0;
	unsigned int wordScore = 0;
	unsigned int wordMultiplier = 1;
	unsigned int lettersUsedFromRack = 0;
	
	if(doTranspose){
		transpose();
		std::swap(row, column);
	}

	for(unsigned int i=0; i < word.size(); ++i){

		assert(row >= 0 && row < 15 && column >= 0 && column <= 15);

		if(isAvailable(row, column)){
			
			++lettersUsedFromRack;

			// Add to current wordScore

			unsigned int characterScore = 0;
			
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
				adjacentScore += adjacent_multiplier * (adjacentS.first + characterScore);
			}
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

	if(lettersUsedFromRack==7){
		/* Scrabble ! */
		adjacentScore += 50;
	}

	return adjacentScore + wordScore * wordMultiplier;
}

unsigned int ScrabbleGrid::placeWord(const std::string &word, unsigned int row, unsigned int column, 
		bool doTranspose, const std::set<unsigned int> &blanks){
	if(doTranspose){
		transpose();
		std::swap(row, column);
	}

	unsigned int first_column = column;

	unsigned int score = computeScore(word, row, column, false, blanks);
	std::vector<std::pair<unsigned int, unsigned int>> newPositions;

	for(unsigned int i=0; i < word.size(); ++i){

		bool wasAvailable = isAvailable(row, column);

		if(wasAvailable){
			newPositions.push_back({row, column});
		
			// Should a blank be added to the grid ?
			if(blanks.find(i) != blanks.end()){
				addBlank(row, column);
			}
			
			// The current position can no longer be used as an anchor
			getHorizontalCrosscheck(row, column).clear();
			getVerticalCrosscheck(row, column).clear();
		}

		get(row, column) = word[i];
		
		if(wasAvailable){
			if(row > 0){
				computeHorizontalCrosscheck(row-1, column);
			}
			if(row < 14){
				computeHorizontalCrosscheck(row+1, column);
			}
		}		
		
		++column;
	}

	if(first_column > 0){
		computeVerticalCrosscheck(row, first_column -1);
	}
	if(column < 15){
		computeVerticalCrosscheck(row, column);
	}

	if(doTranspose){
		transpose();
	}

	return score;
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
			for(int i = row+1; row < 15; ++i){
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

std::pair<std::string, std::string> ScrabbleGrid::verticalCrosscheckHelper(unsigned int row, 
		unsigned int column){
	std::string prefix;
	std::string suffix;
	
	unsigned int before = column;
	while(before > 0 && !isAvailable(row, --before)){
		prefix.push_back(get(row, before));
	}
	unsigned int after = column;
	while(after < 14 && !isAvailable(row, ++after)){
		suffix.push_back(get(row, after));
	}
	std::reverse(prefix.begin(), prefix.end());
	return {prefix, suffix};
}

void ScrabbleGrid::computeVerticalCrosscheck(unsigned int row, unsigned int column){
	if(isAvailable(row, column)){
		auto prefixSuffix = verticalCrosscheckHelper(row, column);
		for(char c = 'A'; c <= 'Z'; ++c){
			std::stringstream ss;
			ss << prefixSuffix.first << c << prefixSuffix.second;
			if(trie->checkWord(ss.str())){
				getVerticalCrosscheck(row, column).set(c);
			}
		}
	}
}

std::pair<std::string, std::string> ScrabbleGrid::horizontalCrosscheckHelper(unsigned int row, unsigned int column){
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

void ScrabbleGrid::computeHorizontalCrosscheck(unsigned int row, unsigned int column){
	if(isAvailable(row, column)){
		auto prefixSuffix = horizontalCrosscheckHelper(row, column);
		for(char c = 'A'; c <= 'Z'; ++c){
			std::stringstream ss;
			ss << prefixSuffix.first << c << prefixSuffix.second;
			if(trie->checkWord(ss.str())){
				getHorizontalCrosscheck(row, column).set(c);
			}
		}
	}
}

