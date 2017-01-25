#ifndef SCRABBLE_CONSTANTS_HPP
#define SCRABBLE_CONSTANTS_HPP

#include <vector>

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


#endif
