#ifndef SCRABBLEGRIDELEMENT_H
#define SCRABBLEGRIDELEMENT_H

#include <QFrame>
#include <QLabel>
#include <QString>

class ScrabbleGridElement : public QFrame{
	Q_OBJECT
private:
    unsigned int row;
    unsigned int column;
	QString backgroundColor;	
	QString borderColor;
    QLabel *letter;
    QLabel *score;
	void setCSS();
protected:
	void mousePressEvent(QMouseEvent* event);
public:
    explicit ScrabbleGridElement(unsigned int _r, unsigned int _c, const QString &_color,  
			unsigned int scoreHorizontalOffset = 36,
			unsigned int scoreVerticalOffset = 25);
    void setLetter(char c);
    void setScore(int c);
    void clearLetter();
    void clearScore();
    void setBackgroundColor(const QString &color);
	void setBorderColor(const QString &color);
	void resetBorderColor();
signals:
	void clicked(unsigned int row, unsigned int column);
};

#endif // SCRABBLEGRIDELEMENT_H
