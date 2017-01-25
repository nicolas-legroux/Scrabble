#ifndef SCRABBLE_MAIN_HPP
#define SCRABBLE_MAIN_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "ScrabbleGrid.hpp"
#include "ScrabbleRack.hpp"
#include "ScrabbleGridWidget.hpp"
#include "ScrabbleRackWidget.hpp"

class ScrabbleMain : public QWidget{
	Q_OBJECT
	private:
		ScrabbleGrid *grid;
		ScrabbleRack *rack;
		ScrabblePlayer *player;
		ScrabbleGridWidget *gridWidget;
		ScrabbleRackWidget *rackWidget;
		QPushButton *button;
		QLabel *infoWidget;
		QLabel *scoreWidget;
		bool gameIsFinished = false;
		bool drawLetters = true;
		bool noMoreLetters = false;
	public:
		ScrabbleMain(ScrabbleGrid *g, ScrabbleRack *r, ScrabblePlayer *p, QWidget *parent = nullptr);

	signals:

	public slots:
		void playTurn();
};

#endif
