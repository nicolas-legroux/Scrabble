#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Trie.hpp"
#include "ScrabbleRack.hpp"
#include "ScrabbleGrid.hpp"
#include "ScrabblePlayer.hpp"
#include "ScrabbleMain.hpp"

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);

	auto t = std::time(0);
	std::cout << "Initialized random generator with " << t << "\n\n";
	std::srand(t);

	std::ifstream dict("./assets/dic_fr.txt");
	Trie trie(dict);
	
	ScrabbleGrid grid(&trie);
	ScrabbleStack stack;
	ScrabbleRack rack;
	BestWordIA player(&grid, &rack);

	ScrabbleMain mainWindow(&grid, &rack, &player);
	mainWindow.show();

	return app.exec();
}
