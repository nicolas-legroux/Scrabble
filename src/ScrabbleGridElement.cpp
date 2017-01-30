#include "ScrabbleGridElement.hpp"
#include "ScrabbleGridWidget.hpp"

#include <QHBoxLayout>

ScrabbleGridElement::ScrabbleGridElement(unsigned int _r, 
		unsigned int _c, const QString &color, unsigned int scoreHorizontalOffset, 
		unsigned int scoreVerticalOffset) :  row(_r), column(_c), 
		backgroundColor(color), borderColor(DEFAULT_BORDER_COLOR){
    letter = new QLabel;
    letter->setStyleSheet("border : 0; background: none; color : black;");
    letter->setFont(QFont("Arial", 20));
    letter->setAlignment(Qt::AlignCenter);

    QHBoxLayout* textLayout = new QHBoxLayout;
    textLayout->addWidget(letter);
    this->setLayout(textLayout);

    score = new QLabel("", this);
    score->move(scoreHorizontalOffset, scoreVerticalOffset);
    score->setFont(QFont("Arial", 8));
    score->setStyleSheet("border : 0; background: none; color : black;");

    this->setCSS();
}

void ScrabbleGridElement::setScore(int s){
    score->setText(QString::number(s));
}

void ScrabbleGridElement::setLetter(char c){
    letter->setText(QChar(c));
}

void ScrabbleGridElement::clearScore(){
    score->setText("");
}

void ScrabbleGridElement::clearLetter(){
    letter->setText("");
}

void ScrabbleGridElement::setCSS(){
	QString css = "border : 3px solid " + borderColor + "; background-color : " + backgroundColor + ";";
	this->setStyleSheet(css);
}

void ScrabbleGridElement::setBackgroundColor(const QString &color){
    backgroundColor = color;
    this->setCSS();
}

void ScrabbleGridElement::setBorderColor(const QString &color){
	borderColor = color;
	this->setCSS();
}

void ScrabbleGridElement::resetBorderColor(){
	borderColor = DEFAULT_BORDER_COLOR;
	this->setCSS();
}

void ScrabbleGridElement::mousePressEvent(QMouseEvent* event){
	emit clicked(row, column);
}
