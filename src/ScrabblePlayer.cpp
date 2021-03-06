#include <algorithm>
#include <cassert>

#include "ScrabblePlayer.hpp"

std::ostream& operator<<(std::ostream &os, const ScrabblePlayer::WordChoice &wc){
	char row = 'A' + wc.row;
	int column = wc.column+1;
	os << '{' << wc.word << ' ' << row << column;
	
	if(wc.vertical){
		os << " vertical";
	}
	else{
		os << " horizontal";
	}
	
	if(!wc.blanks.empty()){
		os << " blank={";
		for(unsigned int blank : wc.blanks){
			os << (blank+1);
		}
		os << '}';
	}

	os << '}';

	return os;
}

std::pair<unsigned int, unsigned int> ScrabblePlayer::WordChoice::computeStatistics(ScrabbleGrid *grid) const {
	auto result = grid->computeScore(word, row, column, vertical, blanks);
	return {result.first, result.second.size()};
}

unsigned int ScrabblePlayer::WordChoice::play(ScrabbleGrid *grid, 
		ScrabbleRack *rack){
	auto result = grid->placeWord(word, row, column, vertical, blanks);
	
	// Update the rack
	std::vector<char> lettersToRemove;
	for(auto i : result.second){
		if(blanks.find(i) == blanks.end()){
			lettersToRemove.push_back(word[i]);
		}
		else{
			lettersToRemove.push_back(BLANK);
		}
	}
	rack->remove(lettersToRemove);

	for(auto i : result.second){
		if(!vertical){
			positionsChanged.push_back({row, column+i});
		}
		else{
			positionsChanged.push_back({row+i, column});
		}
	}

	score = result.first;

	return result.first;
}

ScrabblePlayer::WordChoice& ScrabblePlayer::WordChoice::transpose(){
	std::swap(row, column);
	vertical = !vertical;
	return *this;
}

bool ScrabblePlayer::playWordChoice(WordChoice &wordChoice){
	std::cout << "Playing : " << wordChoice << ".\n";
	unsigned int s = wordChoice.play(grid, rack);
	plays.push_back(wordChoice);
	std::cout << "Scored " << s << " points.\n";
	score += s;
	std::cout << "Total score : " << score << ".\n";
	std::cout << '\n' << *grid << "\n\n";
	return true;
}

bool ScrabblePlayer::playTurn(){
	if(rack->empty()){
		return false;
	}
	else{
		++timesPlayed;
		std::cout << "Play #" << timesPlayed << '\n';
		std::cout << "Current rack : " << *rack << '\n';
		
		auto wc = generateWordChoice();
		if(!wc.first){
			std::cout << "No legal word was found.\n";
			return false;
		}
		WordChoice wordChoice = wc.second;
		return playWordChoice(wordChoice);
	}
}

void ScrabblePlayer::reset(){
	score = 0;
	timesPlayed = 0;
	plays.clear();
}

std::pair<unsigned int, std::vector<unsigned int>> BestWordIA::findAnchors(unsigned int row){
	unsigned int column = grid->firstAvailableColumn(row);
	if(column == 15){
		return {15, {}};
	}
	else{
		std::vector<unsigned int> anchors;
		for(unsigned int i = column; i<15; ++i){
			if(grid->getHorizontalCrosscheck(row, i).valid() || grid->getVerticalCrosscheck(row, i).valid()){
				anchors.push_back(i);
			}
		}
		return {column, anchors};
	}
}

void BestWordIA::extendRight(std::string *s, unsigned int node, bool isTerminal, 
		unsigned int anchorRow, unsigned int anchorColumn, unsigned int currentIdx, unsigned int prefixSize,
		bool vertical, std::stack<unsigned int> *blanks, std::vector<WordChoice> *choices){

	// Reached the end of the grid
	
	if(anchorColumn + currentIdx == 15){
		if(isTerminal){
			unsigned int row = anchorRow;
			unsigned int column = anchorColumn - prefixSize;
			if(vertical){
				std::swap(row, column);
			}
			choices->emplace_back(*s, row, column, vertical, *blanks);
		}
		return;
	}
	
	// The current square is available. If a correct word has been 
	// computed, add it to the list of choices. Then, look at the 
	// letters of the rack that could be used to extend the word, 
	// while verifying crosschecks.
	
	if(grid->isAvailable(anchorRow, anchorColumn + currentIdx)){
		if(isTerminal){
			unsigned int row = anchorRow;
			unsigned int column = anchorColumn - prefixSize;
			if(vertical){
				std::swap(row, column);
			}
			choices->emplace_back(*s, row, column, vertical, *blanks);
		}
		
		// A prefix has been found and it will be impossible to go any further
		if(node == 0 && !s->empty()){
			return;
		}

		std::vector<Trie::Edge> edges = grid->getTrie()->getEdges(node);
		for(const auto &edge : edges){
			char c = edge.getLetter();
			auto &crosscheck = grid->getHorizontalCrosscheck(anchorRow, anchorColumn + currentIdx);
			if(!crosscheck.valid() || crosscheck.check(c)){
				if(rack->hasLetter(c)){
					rack->removeLetter(c);
					unsigned int next = edge.getNextNode();
					bool nextTerminal = edge.isTerminal();
					s->push_back(c);
					extendRight(s, next, nextTerminal, anchorRow, anchorColumn, currentIdx+1, prefixSize, 
						vertical, blanks, choices);
					s->pop_back();
					rack->addLetter(c);
				}
				if(rack->hasBlank()){
					rack->removeBlank();
					blanks->push(s->size());
					s->push_back(c);
					unsigned int next = edge.getNextNode();
					bool nextTerminal = edge.isTerminal();
					extendRight(s, next, nextTerminal, anchorRow, anchorColumn, currentIdx+1, prefixSize, 
						vertical, blanks, choices);
					s->pop_back();
					blanks->pop();
					rack->addBlank();
				}
			}
		}
	}

	// There already is a letter in the current square. Check whether 
	// it is possible to extend the current prefix with that letter, 
	// and move right if that is the case.
	else{
		
		// A correct prefix has been found, but it will be impossible to extend further
		if(node==0){
			assert(!s->empty());
			return;
		}

		char c = grid->get(anchorRow, anchorColumn + currentIdx);
		std::vector<Trie::Edge> edges = grid->getTrie()->getEdges(node);
		for(const auto &edge : edges){
			if(edge.getLetter() == c){
				s->push_back(c);
				unsigned int next = edge.getNextNode();
				bool nextTerminal = edge.isTerminal();
				extendRight(s, next, nextTerminal, anchorRow, anchorColumn, currentIdx+1, 
						prefixSize, vertical, blanks, choices);
				s->pop_back();
				return;
			}
		}
	}
}

void BestWordIA::recursiveLeftPart(std::string *s, unsigned int node, int limit, 
		unsigned int anchorRow, unsigned int anchorColumn, bool vertical, 
		std::stack<unsigned int> *blanks, std::vector<WordChoice> *choices){
	
	// A word has been found but it won't be anchored
	
	if(node == 0 && !s->empty()){
		return; 
	}

	// Extend to the right starting from the anchor square
	
	extendRight(s, node, false, anchorRow, anchorColumn, 0, s->size(), vertical, blanks, choices);
	
	// Compute all possible prefixes formed with letters from the rack
	
	if(limit > 0){
		std::vector<Trie::Edge> edges = grid->getTrie()->getEdges(node);
		for(const auto &edge : edges){
			char c = edge.getLetter();
			if(rack->hasLetter(c)){
				rack->removeLetter(c);
				s->push_back(c);
				unsigned int nextNode = edge.getNextNode();
				recursiveLeftPart(s, nextNode, limit-1, anchorRow, anchorColumn, vertical, blanks, choices);
				s->pop_back();
				rack->addLetter(c);
			}
			if(rack->hasBlank()){
				rack->removeBlank();
				blanks->push(s->size());
				s->push_back(c);
				unsigned int nextNode = edge.getNextNode();
				recursiveLeftPart(s, nextNode, limit-1, anchorRow, anchorColumn, vertical, blanks, choices);
				s->pop_back();
				blanks->pop();
				rack->addBlank();
			}
		}
	}
}

void BestWordIA::leftPart(int limit, unsigned int anchorRow, unsigned int anchorColumn, 
		bool vertical, std::stack<unsigned int> *blanks, std::vector<WordChoice> *choices){

	// The square left of the anchor square has a letter. Compute the prefix that
	// is already there, and try to extend right
	
	if(anchorColumn>0 && !grid->isAvailable(anchorRow, anchorColumn-1)){
		int index = anchorColumn - 1;
		std::string s;
		while(index >= 0 && !grid->isAvailable(anchorRow, index)){
			s.push_back(grid->get(anchorRow, index));
			--index;
		}
		std::reverse(s.begin(), s.end());
		unsigned int node = grid->trie->findPrefix(s);
		extendRight(&s, node, false, anchorRow, anchorColumn, 0, s.size(), vertical, blanks, choices);		
	}

	// The square left of the anchor square is not occupied. Extend right after 
	// computing all possible prefixes with letters from the rack
	
	else{
		std::string s;
		recursiveLeftPart(&s, 0, limit, anchorRow, anchorColumn, vertical, blanks, choices);
	}
}

void BestWordIA::generateHorizontalWordChoices(bool vertical, std::vector<WordChoice> *choices){
	for(unsigned int row = 0; row < 15; ++row){
		auto anchors = findAnchors(row);
		std::vector<std::pair<unsigned int, unsigned int>> anchorsAndLimits;
		for(unsigned int i=0; i<anchors.second.size(); ++i){
			if (i==0){
				anchorsAndLimits.push_back({anchors.second[i], anchors.first});
			}
			else{
				anchorsAndLimits.push_back({anchors.second[i], anchors.second[i-1]+1});
			}
		}

		for(const auto &anchorAndLimit : anchorsAndLimits){
			unsigned int anchor = anchorAndLimit.first;
			int limit = anchor - anchorAndLimit.second;
			std::stack<unsigned int> blanks;
			leftPart(limit, row, anchor, vertical, &blanks, choices);
		}
	}
}

std::pair<bool, ScrabblePlayer::WordChoice> BestWordIA::generateWordChoice(){
	std::vector<WordChoice> choices;

	// Generate all horizontal word choices
	generateHorizontalWordChoices(false, &choices);	
	unsigned int numberHorizontalMoves = choices.size();

	// Generate vertical moves
	if(!grid->isAvailable(7, 7)){
		grid->transpose();	
		generateHorizontalWordChoices(true, &choices);
		grid->transpose();
	}

	if(choices.size() == 0){
		return {false, WordChoice()};
	}

	std::cout << "Found " << numberHorizontalMoves << " horizontal and " 
		<< choices.size() - numberHorizontalMoves << " vertical moves.\n";

	/*
	for(const auto &wc : choices){
		std::cout << wc << '\n';
	}
	*/

	WordChoice &bestChoice = choices.front();
	auto stats = bestChoice.computeStatistics(grid);

	unsigned int bestScore = stats.first;
	unsigned int mostLetters = stats.second;

	for(unsigned int i=1; i<numberHorizontalMoves; ++i){
		stats = choices[i].computeStatistics(grid);
		if(stats.first > bestScore || (stats.first == bestScore && stats.second > mostLetters)){
			bestChoice = choices[i];
			bestScore = stats.first;
			mostLetters = stats.second;
		}
	}

	if(numberHorizontalMoves != choices.size()){
		grid->transpose();
		for(unsigned int i=numberHorizontalMoves; i<choices.size(); ++i){
			stats = choices[i].transpose().computeStatistics(grid);
			if(stats.first > bestScore || (stats.first == bestScore && stats.second > mostLetters)){
				bestChoice = choices[i].transpose();
				bestScore = stats.first;
				mostLetters = stats.second;
			}
			// Tranpose back, otherwise possible bug if numberHorizontalMoves == 0
			else{
				choices[i].transpose();
			}
		}
		grid->transpose();
	}

	return {true, bestChoice};
}

bool BestWordIA::playWord(const std::string &word, unsigned int row, unsigned int column){

	std::vector<WordChoice> choices;

	// Generate all horizontal word choices
	generateHorizontalWordChoices(false, &choices);	
	unsigned int numberHorizontalMoves = choices.size();

	// Generate vertical moves
	if(!grid->isAvailable(7, 7)){
		grid->transpose();	
		generateHorizontalWordChoices(true, &choices);
		grid->transpose();
	}

	WordChoice bestChoice;
	unsigned int bestScore = 0;

	for(unsigned int i=1; i<numberHorizontalMoves; ++i){
		auto &wc = choices[i];
		if(wc.getWord() == word && wc.getRow() == row && wc.getColumn() == column){
			auto stats = wc.computeStatistics(grid);
			if(stats.first > bestScore){
				bestChoice = wc;
				bestScore = stats.first;
			}
		}
	}

	if(numberHorizontalMoves != choices.size()){
		grid->transpose();
		for(unsigned int i=numberHorizontalMoves; i<choices.size(); ++i){
			auto &wc = choices[i].transpose();
			if(wc.getWord() == word && wc.getRow() == column && wc.getColumn() == row){
				auto stats = wc.computeStatistics(grid);
				if(stats.first > bestScore){
					bestChoice = wc.transpose();
					bestScore = stats.first;
				}
			}
		}
		grid->transpose();
	}
	
	if(bestScore != 0){
		playWordChoice(bestChoice);
		return true;
	}

	else{
		return false;
	}
}

void ScrabblePlayer::cancelLastPlay(){
	if(plays.size()>0){
		WordChoice lastPlay = plays.back();
		plays.pop_back();
		score -= lastPlay.getScore();
		--timesPlayed;
		grid->reset(lastPlay.getPositionsChanged());
		std::vector<unsigned int> indexes;
		unsigned int start = (lastPlay.isVertical()) ? lastPlay.getRow() : lastPlay.getColumn();
		
		for(const auto &position : lastPlay.getPositionsChanged()){
			if(lastPlay.isVertical()){
				indexes.push_back(position.first - start);
			}
			else{
				indexes.push_back(position.second - start);
			}
		}

		for(auto i : indexes){
			if(lastPlay.getBlanks().find(i) == lastPlay.getBlanks().end()){
				rack->addLetter(lastPlay.getWord()[i]);
			}
			else{
				rack->addBlank();
			}
		}
	}
}
