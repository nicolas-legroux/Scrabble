#ifndef SCRABBLE_RACK_WIDGET_HPP
#define SCRABBLE_RACK_WIDGET_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <vector>
#include "ScrabbleGridElement.hpp"
#include "ScrabbleGridWidget.hpp"

class ScrabbleRackWidget : public QWidget{
	Q_OBJECT
	private: 
		std::vector<ScrabbleGridElement*> tiles;
	public:
		explicit ScrabbleRackWidget(QWidget *parent = 0);
		void updateLetters(const std::vector<char> &v);
	signals:
	
	public slots:
};


#endif 
