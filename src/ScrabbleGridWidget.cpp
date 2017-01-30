#include "ScrabbleGridWidget.hpp"
#include "ScrabbleMain.hpp"

#include <QGridLayout>

ScrabbleGridWidget::ScrabbleGridWidget(ScrabbleMain *main, QWidget *parent) : QWidget(parent),
    gridElements(15*15), emptyColors(15*15, DEFAULT_COLOR){

    // COLORS

    // Triple word
    for(unsigned int i=0; i<15; i += 7){
        for(unsigned int j=0; j<15; j += 7){
            getColor(i, j) = TRIPLE_WORD_COLOR;
        }
    }

    // Double word
    for(unsigned int i=1; i<14; ++i){
        getColor(i, i) = DOUBLE_WORD_COLOR;
        getColor(i, 14-i) = DOUBLE_WORD_COLOR;
    }

    // Triple char
    for(unsigned int i=1; i<15; i += 4){
        for(unsigned int j=1; j<15; j += 4){
            if(getColor(i, j) == DEFAULT_COLOR){
                getColor(i, j) = TRIPLE_CHAR_COLOR;
            }
        }
    }
    getColor(5, 5) = TRIPLE_CHAR_COLOR;
    getColor(5, 9) = TRIPLE_CHAR_COLOR;
    getColor(9, 5) = TRIPLE_CHAR_COLOR;
    getColor(9, 9) = TRIPLE_CHAR_COLOR;

    // Double char
    getColor(0, 3) = DOUBLE_CHAR_COLOR;
    getColor(0, 11) = DOUBLE_CHAR_COLOR;
    getColor(14, 3) = DOUBLE_CHAR_COLOR;
    getColor(14, 11) = DOUBLE_CHAR_COLOR;
    getColor(2, 6) = DOUBLE_CHAR_COLOR;
    getColor(2, 8) = DOUBLE_CHAR_COLOR;
    getColor(12, 6) = DOUBLE_CHAR_COLOR;
    getColor(12, 8) = DOUBLE_CHAR_COLOR;
    getColor(3, 0) = DOUBLE_CHAR_COLOR;
    getColor(3, 7) = DOUBLE_CHAR_COLOR;
    getColor(3, 14) = DOUBLE_CHAR_COLOR;
    getColor(11, 0) = DOUBLE_CHAR_COLOR;
    getColor(11, 7) = DOUBLE_CHAR_COLOR;
    getColor(11, 14) = DOUBLE_CHAR_COLOR;
    getColor(6, 2) = DOUBLE_CHAR_COLOR;
    getColor(6, 6) = DOUBLE_CHAR_COLOR;
    getColor(6, 8) = DOUBLE_CHAR_COLOR;
    getColor(6, 12) = DOUBLE_CHAR_COLOR;
    getColor(8, 2) = DOUBLE_CHAR_COLOR;
    getColor(8, 6) = DOUBLE_CHAR_COLOR;
    getColor(8, 8) = DOUBLE_CHAR_COLOR;
    getColor(8, 12) = DOUBLE_CHAR_COLOR;
    getColor(7, 3) = DOUBLE_CHAR_COLOR;
    getColor(7, 11) = DOUBLE_CHAR_COLOR;

    QGridLayout *layout = new QGridLayout;

    layout->setVerticalSpacing(0);
    layout->setHorizontalSpacing(0);

    layout->setRowMinimumHeight(0, 100);
    layout->setColumnMinimumWidth(0, 100);

    for(unsigned int i=0; i<15; ++i){
        for(unsigned int j=0; j<15; ++j){
            get(i, j) = new ScrabbleGridElement(i, j, getColor(i, j));
			get(i, j)->setCursor(Qt::PointingHandCursor);
			QObject::connect(get(i, j), SIGNAL(clicked(unsigned int, unsigned int)), main, 
					SLOT(manualPlay(unsigned int, unsigned int)));
            layout->addWidget(get(i, j), i, j);
        }
    }

    for(int i=0; i<15; ++i){
        layout->setRowMinimumHeight(i, 50);
        layout->setColumnMinimumWidth(i, 50);
    }

    setLayout(layout);
    setFixedSize(800, 800);
}

void ScrabbleGridWidget::placeLetter(unsigned int row, unsigned int column, char letter, int score){
    get(row, column)->setLetter(letter);
    get(row, column)->setScore(score);
    get(row, column)->setBackgroundColor(TILE_COLOR);
}

void ScrabbleGridWidget::updatePositions(ScrabbleGrid *grid, 
		const std::vector<std::pair<unsigned int, unsigned int>> &positions){
	for(const auto &position : positions){
		auto row = position.first;
		auto column = position.second;
		if(grid->isAvailable(row, column)){
			get(row, column)->clearLetter();
			get(row, column)->clearScore();
			get(row, column)->setBackgroundColor(getColor(row, column));
			get(row, column)->resetBorderColor();
		}
		else{
			char letter = grid->get(row, column);
			get(row, column)->setLetter(letter);
			get(row, column)->setBackgroundColor(TILE_COLOR);
			if(getColor(row, column) !=	DEFAULT_COLOR){
				get(row, column)->setBorderColor(getColor(row, column));
			}
			int score = 0;
			if(!grid->isBlank(row, column)){
				score = LETTER_POINTS_FR[letter - 'A'];
			}
			get(row, column)->setScore(score);
		}
	}
}

