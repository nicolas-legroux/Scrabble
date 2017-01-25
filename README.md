## Description

A program which plays Scrabble, in French. The program implements the algorithm of Appel and Jacobson (https://pdfs.semanticscholar.org/da31/cb24574f7c881a5dbf008e52aac7048c9d9c.pdf) to find the highest scoring word given a configuration of the grid.

## Installation

On UNIX :

	git clone https://github.com/nicolas-legroux/Scrabble.git
	cd Scrabble
	cmake .
	make
	
To compile the program you need to have QT v5 installed. On Ubuntu you can run :

	sudo apt-get install qt5-default

## Running the program

Launch the program in its root directory :

	cd Scrabble
	./Scrabble

