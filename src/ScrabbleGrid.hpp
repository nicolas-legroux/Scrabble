#ifndef SCRABBLE_GRID_HPP_
#define SCRABBLE_GRID_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <climits>
#include <set>

#include "Trie.hpp"
#include "ScrabblePlayer.hpp"
#include "ScrabbleConstants.hpp"

class BestWordIA;
class ScrabbleGridWidget;

class ScrabbleGrid{
	private:

		// Represent crosschecks as bit vectors using an int
		class Crosscheck{
			private:
				unsigned int data = 0;
			public:
				bool valid() const { return data & (1<<30); }
				void makeValid() { data |= (1<<30); }
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
		friend BestWordIA;
		friend ScrabbleGridWidget;
		
		// The grid
		std::vector<char> grid = std::vector<char>(15*15);
		
		// The crosschecks
		std::vector<Crosscheck> horizontalCrosschecks = std::vector<Crosscheck>(15*15);
		std::vector<Crosscheck> verticalCrosschecks = std::vector<Crosscheck>(15*15);
		
		// Is the grid currently transposed ?
		bool transposed = false;
		
		// Where have the blanks been played ?
		std::set<std::pair<unsigned int, unsigned int>> blanks;
		
		// Trie containing the dictionary
		Trie *trie;
		
		// Pointers that are swaped during a transposition
		std::vector<Crosscheck> *hzCrosschecks;
		std::vector<Crosscheck> *vcCrosschecks;
		
		
		// Helper functions for the grid		
		char & get(unsigned int row, unsigned int column) { return grid[15*row + column]; }
		const char & get(unsigned int row, unsigned int column) const { 
			return grid[15*row + column]; 
		}
		
		// Helper functions for the crosschecks
		Crosscheck & getHorizontalCrosscheck(unsigned int row, unsigned int column){
			return (*hzCrosschecks)[15*row + column];
		}
		const Crosscheck & getHorizontalCrosscheck(unsigned int row, unsigned int column) const {
			return (*hzCrosschecks)[15*row + column];
		}
		// Notice the transposition
		Crosscheck & getVerticalCrosscheck(unsigned int row, unsigned int column){ 
			return (*vcCrosschecks)[15*column + row];
		}
		const Crosscheck & getVerticalCrosscheck(unsigned int row, unsigned int column) const {
			return (*vcCrosschecks)[15*column + row];
		}

		// Is the position an anchor ? (ie it has non trivial crosschecks)
		bool isAnchor(unsigned int row, unsigned int column){
			return getHorizontalCrosscheck(row, column).valid() || 
				getVerticalCrosscheck(row, column).valid();
		}
				
		// Is the position currently available (ie no tiles have been played yet) ?
		bool isAvailable(unsigned int row, unsigned int column) const {
			return get(row, column) <= 4;
		}

		// Check whether a blank has been played in the position
		bool isBlank(unsigned int row, unsigned int column){
			if(transposed){
				std::swap(row, column);
			}
			return blanks.find({row, column}) != blanks.end();
		}

		// Add a blank to the grid
		void addBlank(unsigned int row, unsigned int column){
			if(transposed){
				std::swap(row, column);
			}
			blanks.insert({row, column});
		}

		// Compute the scores of the letters directly above and under the given position
		std::pair<unsigned int, bool> computeAdjacentScore(unsigned int row, 
				unsigned int column);
		
		// Compute horizontal and vertical crosschecks
		std::pair<std::string, std::string> findPrefixSuffix(unsigned int row, unsigned int column);
		void computeCrosscheck(unsigned int row, unsigned int column);
		void computeCrosscheck(Crosscheck &crosscheck, const std::pair<std::string, std::string> &ps);
		void computeCrosschecks();

	public:
		ScrabbleGrid(Trie *_trie) : trie(_trie) { makeNewGrid(); }
		
		// Initialize a new grid
		void makeNewGrid();

		// Compute the score of a word played horizontally. If the word is 
		// to be played vertically, set the 'doTranspose' flag. If the word uses blank tiles, 
		// the index of thoses tiles in the word should be specified
		std::pair<unsigned int, std::vector<unsigned int>> computeScore(const std::string &word, 
				unsigned int firstRow, 	unsigned int firstColumn, bool doTranspose, 
				const std::set<unsigned int> &blanks = {}, bool calledFromPlace = false);

		// Place a new word in the grid. By default, the word is placed horizontally.
		// Set the 'doTranspose' flag to place it vertically
		// Return the score and the letters that must come from the rack
		std::pair<unsigned int, std::vector<unsigned int>> placeWord(const std::string &word, 
				unsigned int firstRow, unsigned int firstColumn, bool doTranspose, 
				const std::set<unsigned int> &blanks = {});
		
		// Tranpose the grid
		void transpose();

		unsigned int firstAvailableColumn(unsigned int row);

		bool empty() { return isAvailable(7, 7); }

		Trie* getTrie() { return trie; }
};

std::ostream& operator<<(std::ostream &os, const ScrabbleGrid &grid);
std::ostream& operator<<(std::ostream &os, const ScrabbleGrid::Crosscheck &cc);

#endif /* SCRABBLE_GRID_HPP_ */	
