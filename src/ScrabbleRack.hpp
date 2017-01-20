#ifndef SCRABBLE_RACK_HPP_
#define SCRABBLE_RACK_HPP_

#include <vector>
#include <string>
#include <iostream>

const char BLANK = 'Z'+1;

const std::vector<int> LETTER_DISTRIBUTION_FR = { 
	9, // A 
	2, // B
	2, // C
	3, // D
	15, // E
	2, // F
	2, // G
	2, // H
	8, // I
	1, // J
	1, // K
	5, // L
	3, // M
	6, // N
	6, // O
	2, // P
	1, // Q
	6, // R
	6, // S
	6, // T
	6, // U
	2, // V
	1, // W
	1, // X
	1, // Y
	1, // Z
	2  // BLANK
};

class ScrabbleStack{
	private:
		std::vector<char> stack;
		unsigned int start;
		unsigned int end;
		unsigned int lettersRemaining() { return end - start; }
	public:
		// Build stack with initial letter distribution
		ScrabbleStack(const std::vector<int> &letter_distribution = LETTER_DISTRIBUTION_FR);

		// Shuffle the remaining elements in the stack
		void shuffle();

		// Check whether the stack is empty
		bool empty() const { return start == end; }

		// Draw n letters (max, can be lower) from the stack
		std::vector<char> drawLetters(unsigned int n);
};

class ScrabbleRack{
	private:
		friend std::ostream& operator<<(std::ostream &os, const ScrabbleRack &rack);
		std::vector<unsigned int> rack = std::vector<unsigned int>(27, 0);
		unsigned int size = 0;
		ScrabbleStack *stack;
	public:
		ScrabbleRack(ScrabbleStack *_stack) : stack(_stack) { 
			drawFromStack();
		}
		
		// Check whether the stack from which the rack is drawn is empty
		bool stackIsEmpty() { return stack->empty(); }
		
		// Draw letters from the stack to complete the rack
		void drawFromStack();

		// Empty the rack
		void clear(); 

		// Remove letters from the rack
		void remove(const std::vector<char> &letters);

		// Check whether the rack is empty
		bool empty() const { return size == 0; }
};

std::ostream& operator<<(std::ostream &os, const ScrabbleRack &rack);

#endif /* SCRABBLE_RACK_HPP_ */
