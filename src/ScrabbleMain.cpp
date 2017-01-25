#include <QInputDialog>
#include <QGroupBox>
#include <QRadioButton>

#include "ScrabbleMain.hpp"

ScrabbleMain::ScrabbleMain(ScrabbleGrid *g, QWidget *parent) : 
	QWidget(parent), grid(g) {		
	
	QVBoxLayout *mainLayout = new QVBoxLayout();

	QGroupBox *gb = new QGroupBox("Choisir un mode de sélection des lettres", this);
	QVBoxLayout *layout = new QVBoxLayout();
	automatic = new QRadioButton("Automatique");
	manual = new QRadioButton("Manuel");
	automatic->setChecked(true);
	QPushButton *validation = new QPushButton("Valider");
	layout->addWidget(automatic);
	layout->addWidget(manual);
	gb->setLayout(layout);

	QObject::connect(validation, SIGNAL(clicked()), this, SLOT(chooseRack()));

	mainLayout->addWidget(gb);
	mainLayout->addWidget(validation);

	setLayout(mainLayout);
}

void ScrabbleMain::chooseRack(){
	if(!rack){
		if(automatic->isChecked()){
			stack = new ScrabbleStack;
			rack = new ScrabbleRack(stack);
		}
		else{
			rack = new ScrabbleRack;
		}
		QLayoutItem *child;
		while ((child = this->layout()->takeAt(0)) != 0) {
			delete child;
		}
		delete this->layout();
		initialize();
	}
}	

void ScrabbleMain::initialize(){
	
	player = new BestWordIA(grid, rack);
	
	QVBoxLayout *vLayout = new QVBoxLayout;

	QHBoxLayout *layout = new QHBoxLayout;

	gridWidget = new ScrabbleGridWidget;
	rackWidget = new ScrabbleRackWidget;
	button = new QPushButton("Tirer des lettres");
	button->setFont(QFont("Aria", 16));
	QWidget *rightWidget = new QWidget;

	button->setFixedWidth(300);
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(playTurn()));

	infoWidget = new QLabel();
	infoWidget->setFont(QFont("Arial", 13));
	infoWidget->setAlignment(Qt::AlignCenter);

	scoreWidget = new QLabel("Current score : 0");
	scoreWidget->setFont(QFont("Arial", 16));
	scoreWidget->setAlignment(Qt::AlignCenter);

	vLayout->addWidget(rackWidget);
	vLayout->addWidget(button);
	vLayout->addWidget(infoWidget);
	vLayout->addWidget(scoreWidget);
	vLayout->setAlignment(rackWidget, Qt::AlignHCenter);
	vLayout->setAlignment(button, Qt::AlignHCenter);

	rightWidget->setLayout(vLayout);

	layout->addWidget(gridWidget);
	layout->addWidget(rightWidget);
	
	this->setFixedSize(1400, 850);
	this->setLayout(layout);

	rackWidget->updateLetters(rack->getLetters());
}

void ScrabbleMain::playTurn() {
	if(drawLetters){
		if(rack->requireManualDraw()){
			bool ok = false;
			QString letters = QInputDialog::getText(this, 
					"Tirage de lettres", 
					"Indiquer les lettres à ajouter. Utiliser '*' pour indiquer un blanc.", 
					QLineEdit::Normal, QString(), &ok);
			if(!ok){
				return;
			}
			auto s = letters.toStdString();
			std::cout << s << '\n';
			std::transform(s.begin(), s.end(), s.begin(), 
					::toupper);
			std::vector<char> lettersToAdd;
			for(char c : s){
				if(c == '*'){
					lettersToAdd.push_back(BLANK);
				}
				else if ('A'<= c && c <= 'Z') {
					lettersToAdd.push_back(c);
				}
			}
			rack->addLetters(lettersToAdd);
		}
	
		else{
			rack->draw();
		}
	
		gameIsFinished = rack->empty();
		rackWidget->updateLetters(rack->getLetters());
		noMoreLetters = rack->tilesFinished();
		drawLetters = false;
		if(!gameIsFinished){
			button->setText("Jouer le meilleur coup");
		}
		else{
			button->setText("Reinitialiser la grille");
		}
	}
	else{
		if(!gameIsFinished){
			bool ok = player->playTurn();
			if(ok){
				const auto &lastPlay = player->getLastPlay();
				gridWidget->updatePositions(grid, lastPlay.getPositionsChanged());
				QString info = "Played '" + QString::fromStdString(lastPlay.getWord()) + "' and scored "
					+ QString::number(lastPlay.getScore()) + " points.";
				infoWidget->setText(info);
				rackWidget->updateLetters(rack->getLetters());
			}
			else{
				gameIsFinished = true;
				infoWidget->setText("No legal move was found.");
			}

			drawLetters = !rack->tilesFinished();

			if(!drawLetters && rack->empty()){
				gameIsFinished = true;
			}
			scoreWidget->setText("Current score : " + QString::number(player->getScore()));
			
			if(drawLetters){
				button->setText("Tirer des lettres");
			}
			else if(gameIsFinished){
				button->setText("Reinitialiser la grille");
			}
		}
		else{
			button->setText("Tirer des lettres");
			grid->makeNewGrid();
			std::vector<std::pair<unsigned int, unsigned int>> toUpdate;
			for(unsigned int row=0; row<15; ++row){
				for(unsigned int column=0; column<15; ++column){
					toUpdate.push_back({row, column});
				}
			}
			gridWidget->updatePositions(grid, toUpdate);
			player->reset();
			rack->reset();
			rackWidget->updateLetters(rack->getLetters());
			infoWidget->setText("");
			scoreWidget->setText("Current score : 0");
			gameIsFinished = false;
			drawLetters = true;
			noMoreLetters = false;
		}
	}
}
