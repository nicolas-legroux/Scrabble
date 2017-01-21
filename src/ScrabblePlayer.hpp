#ifndef SCRABBLE_PLAYER_HPP_
#define SCRABBLE_PLAYER_HPP_

#include <iostream>
#include <stack>

#include "ScrabbleRack.hpp"
#include "ScrabbleGrid.hpp"

class ScrabbleGrid;

class ScrabblePlayer{
	protected:
		unsigned int score = 0;
		unsigned int timesPlayed = 0;
		ScrabbleRack *rack;
		ScrabbleGrid *grid;

	public:
		class WordChoice{
			private:
				friend std::ostream& operator<<(std::ostream&, const WordChoice&);
				std::string word;
				unsigned int row;
				unsigned int column;
				bool vertical;
				std::set<unsigned int> blanks;
			public:
				WordChoice() : row(-1), column(-1), vertical(false) {} 
				WordChoice(const std::string &s, unsigned int r, unsigned c, bool v, 
						const std::stack<unsigned int> &_blanks) : 
					word(s), row(r), column(c), vertical(v) {
						std::stack<unsigned int> cp(_blanks);
						while(!cp.empty()){
							blanks.insert(cp.top());
							cp.pop();
						}
					}
				std::pair<unsigned int, unsigned int> computeStatistics(ScrabbleGrid *grid);
				unsigned int play(ScrabbleGrid *grid, ScrabbleRack *rack);
				WordChoice& transpose();

		};
		ScrabblePlayer(ScrabbleRack *_rack, ScrabbleGrid *_grid) : rack(_rack), grid(_grid) { }
		void addToScore(unsigned int s) { score += s; }
		unsigned int getScore() { return score; }
		virtual std::pair<bool, WordChoice> generateWordChoice() = 0;
		virtual bool playTurn();
};

class BestWordIA : public ScrabblePlayer{
	private:
		std::pair<unsigned int, std::vector<unsigned int>> findAnchors(unsigned int row);
		void leftPart(int limit, unsigned int anchorRow, unsigned int anchorColumn, bool vertical, 
				std::stack<unsigned int> *blanks, std::vector<WordChoice> *choices);
		void recursiveLeftPart(std::string *s, unsigned int node, int limit, 
				unsigned int anchorRow, unsigned int anchorColumn, bool vertical,
				std::stack<unsigned int> *blanks, std::vector<WordChoice> *choices);
		void extendRight(std::string *s, unsigned int node, bool isTerminal, unsigned int anchorRow, 
				unsigned int anchorColumn, unsigned int currentIdx, unsigned int prefixSize, 
				bool vertical, std::stack<unsigned int> *blanks, std::vector<WordChoice> *choices);
		void generateHorizontalWordChoices(bool vertical, std::vector<WordChoice> *choices);

	public:
		BestWordIA(ScrabbleRack *_rack, ScrabbleGrid *_grid) : ScrabblePlayer(_rack, _grid) {}
		std::pair<bool, WordChoice> generateWordChoice() override;
};

std::ostream& operator<<(std::ostream &os, const ScrabblePlayer::WordChoice &wc);

#endif /* SCRABBLE_PLAYER_HPP_ */
