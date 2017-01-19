#ifndef SCRABBLE_GRID_HPP_
#define SCRABBLE_GRID_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <climits>
#include <set>
#include "Trie.hpp"

#define DOUBLE_CHAR 1
#define TRIPLE_CHAR 2
#define DOUBLE_WORD 3
#define TRIPLE_WORD 4

const std::vector<unsigned int> LETTER_POINTS_FR = {
	1, // A
	3, // B
	3, // C
	2, // D
	1, // E
	4, // F
	2, // G
	4, // H
	1, // I
	8, // J
	10, // K
	1, // L
	2, // M
	1, // N
	1, // O
	3, // P
	8, // Q
	1, // R
	1, // S
	1, // T
	1, // U
	4, // V
	10, // W
	10, // X
	10, // Y
	10 // Z
};

class ScrabbleGrid{
	private:
		class Crosscheck{
			private:
				unsigned int data = 0;
			public:
				bool valid() const { return data != 0; }
				void clear() { data = 0; }
				void set(char c){
					data |= (1 << (c - 'A'));
				}
				bool check(char c) const {
					return data & (1 << (c - 'A'));
				}
		};
		friend std::ostream& operator<<(std::ostream &os, const ScrabbleGrid &grid);
		friend std::ostream& operator<<(std::ostream &os, const Crosscheck &crosscheck);
		std::vector<char> grid = std::vector<char>(15*15);
		std::vector<Crosscheck> horizontalCrosschecks = std::vector<Crosscheck>(15*15);
		std::vector<Crosscheck> verticalCrosschecks = std::vector<Crosscheck>(15*15);
		bool transposed = false;
		std::set<std::pair<unsigned int, unsigned int>> blanks;
		Trie *trie;
		std::vector<Crosscheck> *hzCrosschecks;
		std::vector<Crosscheck> *vcCrosschecks;
		
		/* Used in constructor */
		void makeNewGrid();
		
		/* Setters and getters */		
		
		char & get(unsigned int row, unsigned int column) { return grid[15*row + column]; }
		const char & get(unsigned int row, unsigned int column) const { return grid[15*row + column]; }
		Crosscheck & getHorizontalCrosscheck(unsigned int row, unsigned int column){
			return (*hzCrosschecks)[15*row + column];
		}
		const Crosscheck & getHorizontalCrosscheck(unsigned int row, unsigned int column) const {
			return (*hzCrosschecks)[15*row + column];
		}
		Crosscheck & getVerticalCrosscheck(unsigned int row, unsigned int column){
			return (*vcCrosschecks)[15*column + row];
		}
		const Crosscheck & getVerticalCrosscheck(unsigned int row, unsigned int column) const {
			return (*vcCrosschecks)[15*column + row];
		}
		bool isAnchor(unsigned int row, unsigned int column){
			return getHorizontalCrosscheck(row, column).valid() || 
				getVerticalCrosscheck(row, column).valid();
		}
		
		unsigned int computeScore(const std::string &word, unsigned int firstRow, unsigned int firstColumn, 
				bool doTranspose, const std::set<unsigned int> &blanks = {});
		
		bool isAvailable(unsigned int row, unsigned int column) const {
			return get(row, column) <= 4;
		}

		bool isBlank(unsigned int row, unsigned int column){
			if(transposed){
				std::swap(row, column);
			}
			return blanks.find({row, column}) != blanks.end();
		}

		void addBlank(unsigned int row, unsigned int column){
			if(transposed){
				std::swap(row, column);
			}
			blanks.insert({row, column});
		}

		std::pair<unsigned int, bool> computeAdjacentScore(unsigned int row, unsigned int column);

		std::pair<std::string, std::string> verticalCrosscheckHelper(unsigned int row, unsigned int column);
		void computeVerticalCrosscheck(unsigned int row, unsigned int column);
		std::pair<std::string, std::string> horizontalCrosscheckHelper(unsigned int row, unsigned int column);
		void computeHorizontalCrosscheck(unsigned int row, unsigned int column);

	public:
		ScrabbleGrid(Trie *_trie) : trie(_trie) { makeNewGrid(); }

		unsigned int placeWord(const std::string &word, unsigned int firstRow, unsigned int forstColumn, 
				bool doTranspose, const std::set<unsigned int> &blanks = {});
		
		void transpose();
};

std::ostream& operator<<(std::ostream &os, const ScrabbleGrid &grid);

#endif /* SCRABBLE_GRID_HPP_ */	
