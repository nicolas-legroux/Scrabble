#ifndef SCRABBLEGRIDWIDGET_H
#define SCRABBLEGRIDWIDGET_H

#include <QWidget>
#include <QString>

#include <vector>

#include "ScrabbleGridElement.hpp"
#include "ScrabbleGrid.hpp"

class ScrabbleGrid;

const QString DOUBLE_CHAR_COLOR = "#A3BBDB";
const QString TRIPLE_CHAR_COLOR = "#0088CF";
const QString DOUBLE_WORD_COLOR = "#E78A71";
const QString TRIPLE_WORD_COLOR = "#DE0016";
const QString DEFAULT_COLOR = "#2F9188";
const QString TILE_COLOR = "#F2E2CE";
const QString DEFAULT_BORDER_COLOR = "#D8E5DC";

class ScrabbleGridWidget : public QWidget
{
    Q_OBJECT
private:
    std::vector<ScrabbleGridElement*> gridElements;
    std::vector<QString> emptyColors;
    ScrabbleGridElement* & get(unsigned int row, unsigned int column){ return gridElements[15*row + column]; }
    QString & getColor(unsigned int row, unsigned int column) { return emptyColors[15*row + column]; }

public:
    explicit ScrabbleGridWidget(QWidget *parent = 0);
    void placeLetter(unsigned int row, unsigned int column, char letter, int score);
	void updatePositions(ScrabbleGrid *grid, 
			const std::vector<std::pair<unsigned int, unsigned int>> &positions);
signals:

public slots:
};

#endif // SCRABBLEGRIDWIDGET_H
