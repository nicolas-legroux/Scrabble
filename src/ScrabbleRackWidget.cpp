#include "ScrabbleRackWidget.hpp"
#include "ScrabbleConstants.hpp"

ScrabbleRackWidget::ScrabbleRackWidget(QWidget *parent) : QWidget(parent) {
	setFixedSize(500, 100);
}

void ScrabbleRackWidget::updateLetters(const std::vector<char> &v){
	
	if (this->layout()){
		for(unsigned int i=0; i<tiles.size(); ++i){
			this->layout()->removeWidget(tiles[i]);
			delete tiles[i];
		}
		tiles.clear();
		delete this->layout();
	}

	QHBoxLayout *layout = new QHBoxLayout;
	for(unsigned int i=0; i<v.size(); ++i){
		tiles.push_back(new ScrabbleGridElement(-1, -1, TILE_COLOR, 42, 27));
		if(v[i] == BLANK){
			tiles[i]->setScore(0);
		}
		else{
			tiles[i]->setLetter(v[i]);
			tiles[i]->setScore(LETTER_POINTS_FR[v[i] - 'A']);
		}
		tiles[i]->setFixedSize(60, 60);
		layout->addWidget(tiles[i]);
	}

	setLayout(layout);
}
