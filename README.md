## Description

A program which plays Scrabble, in French. The program implements the algorithm of Appel and Jacobson (https://pdfs.semanticscholar.org/da31/cb24574f7c881a5dbf008e52aac7048c9d9c.pdf) to find the highest scoring word given a configuration of the grid.

## Installation

	git clone https://github.com/nicolas-legroux/Scrabble.git
	cd Scrabble
	cmake .
	make

## Running the program

Launch the program in its root directory :

	cd Scrabble
	./Scrabble

At launch, you get to choose whether the rack should be drawn automatically (using the letter distribution of the French Scrabble game) or if it should be manually entered at each iteration on the standard input.

The blank tile is represented by the '*' character.
