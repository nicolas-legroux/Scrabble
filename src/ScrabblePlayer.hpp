#ifndef SCRABBLE_PLAYER_HPP_
#define SCRABBLE_PLAYER_HPP_

#include <iostream>
#include <stack>

#include "ScrabbleRack.hpp"
#include "ScrabbleGrid.hpp"
#include "ScrabbleGridWidget.hpp"

class ScrabbleGrid;
class ScrabbleGridWidget;

class ScrabblePlayer{
	public:
		class WordChoice{
			private:
				friend std::ostream& operator<<(std::ostream&, const WordChoice&);
				std::string word;
				unsigned int row;
				unsigned int column;
				bool vertical;
				std::set<unsigned int> blanks;
				unsigned int score = 0;
				std::vector<std::pair<unsigned int, unsigned int>> positionsChanged;
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
				std::pair<unsigned int, unsigned int> computeStatistics(ScrabbleGrid *grid) const;
				unsigned int play(ScrabbleGrid *grid, ScrabbleRack *rack);
				WordChoice& transpose();
				const std::vector<std::pair<unsigned int, unsigned int>> & getPositionsChanged() const{
					return positionsChanged; 
				}
				const std::string & getWord() const { return word; }
				unsigned int getRow() const { return row; }
				unsigned int getColumn() const { return column; }
				bool isVertical() const { return vertical; }
				unsigned int getScore() const { return score; }
		};
	protected:
		ScrabbleGrid *grid;
		ScrabbleRack *rack;
		unsigned int score = 0;
		unsigned int timesPlayed = 0;
		std::vector<WordChoice> plays;
	public:
		ScrabblePlayer(ScrabbleGrid *_grid, ScrabbleRack *r) : 
			grid(_grid), rack(r) { }
		void addToScore(unsigned int s) { score += s; }
		unsigned int getScore() { return score; }
		virtual std::pair<bool, WordChoice> generateWordChoice() = 0;
		virtual bool playTurn();
		const WordChoice & getLastPlay() const { return plays.back(); }
		void reset();
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
		BestWordIA(ScrabbleGrid *g, ScrabbleRack *r) : ScrabblePlayer(g, r) {}
		std::pair<bool, WordChoice> generateWordChoice() override;
};

std::ostream& operator<<(std::ostream &os, const ScrabblePlayer::WordChoice &wc);

#endif /* SCRABBLE_PLAYER_HPP_ */
