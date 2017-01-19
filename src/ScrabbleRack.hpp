#include <vector>
#include <string>

const char BLANK = 'Z'+1;

const std::vector<int> letter_distribution_fr = { 
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
		ScrabbleStack(const std::vector<int> &letter_distribution = letter_distribution_fr);

		void shuffle();

		bool empty() const { return start == end; }

		std::vector<char> drawLetters(unsigned int n);
};

class ScrabbleRack{
	private:
		std::vector<unsigned int> rack = std::vector<unsigned int>(27, 0);
		unsigned int size = 0;
		ScrabbleStack *stack;
	public:
		ScrabbleRack(ScrabbleStack *_stack) : stack(_stack) { 
			drawFromStack();
		}
		
		bool stackIsEmpty() { return stack->empty(); }
		
		void drawFromStack();

		void clear(); 

		std::string convertToString() const;

		bool empty() const { return size == 0; }
};
