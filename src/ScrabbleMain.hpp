#ifndef SCRABBLE_MAIN_HPP
#define SCRABBLE_MAIN_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

#include "ScrabbleGrid.hpp"
#include "ScrabbleRack.hpp"
#include "ScrabbleGridWidget.hpp"
#include "ScrabbleRackWidget.hpp"

class ScrabbleMain : public QWidget{
	Q_OBJECT
	private:
		ScrabbleGrid *grid;
		ScrabblePlayer *player = nullptr;
		ScrabbleStack *stack = nullptr;
		ScrabbleRack *rack = nullptr;
		ScrabbleGridWidget *gridWidget = nullptr;
		ScrabbleRackWidget *rackWidget = nullptr;
		QPushButton *button = nullptr;
		QPushButton *discardButton = nullptr;
		QPushButton *cancelButton = nullptr;
		QLabel *infoWidget = nullptr;
		QLabel *scoreWidget = nullptr;
		QRadioButton *automatic = nullptr;
		QRadioButton *manual = nullptr;
		bool gameIsFinished = false;
		bool drawLetters = true;
		bool noMoreLetters = false;

		void initialize();
	public:
		ScrabbleMain(ScrabbleGrid *grid, QWidget *parent = nullptr);
		~ScrabbleMain(){
			if(stack){
				delete stack;
			}
			if(rack){
				delete rack;
			}
			if(player){
				delete player;
			}
		}
	signals:

	public slots:
		void playTurn();
		void chooseRack();
		void manualPlay(unsigned int row, unsigned int column);
		void discardRack();	
		void cancelLastPlay();
};

#endif
