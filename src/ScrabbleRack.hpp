#ifndef SCRABBLE_RACK_HPP_
#define SCRABBLE_RACK_HPP_

#include <vector>
#include <string>
#include <iostream>

#include "ScrabbleConstants.hpp"

class ScrabbleStack{
	private:
		std::vector<int> letterDistribution;
		std::vector<char> stack;
		unsigned int start;
		unsigned int end;
		unsigned int lettersRemaining() { return end - start; }
	public:
		// Build stack with initial letter distribution
		ScrabbleStack(const std::vector<int> &letter_distribution = LETTER_DISTRIBUTION_FR);

		// Build a new stack from the letter distribution
		void setFromDistribution();

		// Shuffle the remaining elements in the stack
		void shuffle();

		// Check whether the stack is empty
		bool empty() const { return start == end; }

		// Draw n letters from the stack. If there are less than n letters remaining, less than n letters 
		// will be returned
		std::vector<char> drawLetters(unsigned int n);
};

class ScrabbleRack{
	protected:
		friend std::ostream& operator<<(std::ostream &os, const ScrabbleRack &rack);
		
		// 26 letters + a blank
		std::vector<unsigned int> rack = std::vector<unsigned int>(27, 0);
		
		ScrabbleStack *stack;
		
		unsigned int size = 0;
		bool finished = false;

	public:
		ScrabbleRack(ScrabbleStack *_stack = nullptr) : stack(_stack) { 
		}
		
		// Check whether the stack from which the rack is drawn is empty
		virtual bool stackIsEmpty() const { return stack->empty(); }
		
		// Draw letters from the stack to complete the rack
		virtual void draw();

		// Empty the rack
		void clear(); 

		// Reset the rack (by also resetting the stack)
		void reset();

		// Remove letters from the rack
		void remove(const std::vector<char> &letters);

		// Check whether the rack contains a certain letter
		bool hasLetter(char c) const {
 			return rack[c-'A'] > 0;
		}

		// Check whether the rack contains the blank letter
		bool hasBlank() const {
			return hasLetter(BLANK);
		}

		// Add a letter to the rack
		void addLetter(char c){
			++rack[c-'A'];
			++size;
		}
		
		// Add a blank tile to the rack
		void addBlank() { addLetter(BLANK); }

		// Remove a blank tile from the rack
		void removeBlank() { removeLetter(BLANK); }

		// Remove a letter from the rack
		void removeLetter(char c){
			--rack[c-'A'];
			--size;
		}

		// Check whether the letters should be drawn from the stack or from user input
		bool requireManualDraw() {
			return stack == nullptr;
		}

		// Add a set of letters to the rack
		void addLetters(const std::vector<char> &v){
			if(v.size() < 7 - size){
				finished = true;
			}
			for(char c : v){
				addLetter(c);
			}
		}

		// Check whether there are still letters to be drawn
		bool tilesFinished(){
			return finished || (stack != nullptr && stack->empty());
		}

		// Check whether the rack is empty
		bool empty() const { return size == 0; }

		// Return the rack as a vector of chars
		std::vector<char> getLetters();

		virtual ~ScrabbleRack() {} 
};

// A classs used to play in the Terminal
class ScrabbleManualRack : public ScrabbleRack{
	private:
		bool emptyStack = false;
	public: 
		ScrabbleManualRack() :  ScrabbleRack() {
			draw();
		}
		bool stackIsEmpty() const override { return emptyStack; }
		void draw() override;
};

std::ostream& operator<<(std::ostream &os, const ScrabbleRack &rack);

#endif /* SCRABBLE_RACK_HPP_ */


