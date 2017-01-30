#include <QInputDialog>
#include <QGroupBox>
#include <QRadioButton>

#include "ScrabbleMain.hpp"

ScrabbleMain::ScrabbleMain(ScrabbleGrid *g, QWidget *parent) : 
	QWidget(parent), grid(g) {		
	
	QVBoxLayout *mainLayout = new QVBoxLayout();

	QGroupBox *gb = new QGroupBox("Choisir un mode de sélection des lettres");
	QVBoxLayout *layout = new QVBoxLayout();
	automatic = new QRadioButton("Automatique");
	manual = new QRadioButton("Manuel");
	automatic->setChecked(true);
	QPushButton *validation = new QPushButton("Valider");
	layout->addWidget(automatic);
	layout->addWidget(manual);
	gb->setLayout(layout);

	mainLayout->addWidget(gb);
	mainLayout->addWidget(validation);

	setLayout(mainLayout);
	
	QObject::connect(validation, SIGNAL(clicked()), this, SLOT(chooseRack()));
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
			delete child->widget();
			delete child;
		}
		delete this->layout();
		initialize();
	}
}	

void ScrabbleMain::initialize(){
	
	player = new BestWordIA(grid, rack);
	gridWidget = new ScrabbleGridWidget(this);
	rackWidget = new ScrabbleRackWidget;
	button = new QPushButton("Tirer des lettres");
	discardButton = new QPushButton("Jeter les lettres");
	cancelButton = new QPushButton("Annuler");
	button->setFont(QFont("Aria", 13));
	discardButton->setFont(QFont("Aria", 13));
	cancelButton->setFont(QFont("Aria", 13));

	QWidget * buttonContainer = new QWidget;
	QHBoxLayout * buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(button);
	buttonLayout->addWidget(discardButton);
	buttonLayout->addWidget(cancelButton);
	buttonContainer->setLayout(buttonLayout);

	// button->setFixedWidth(300);
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(playTurn()));
	QObject::connect(discardButton, SIGNAL(clicked()), this, SLOT(discardRack()));
	QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelLastPlay()));

	discardButton->setEnabled(false);
	cancelButton->setEnabled(false);

	infoWidget = new QLabel();
	infoWidget->setFont(QFont("Arial", 13));
	infoWidget->setAlignment(Qt::AlignCenter);

	scoreWidget = new QLabel("Score actuel : 0");
	scoreWidget->setFont(QFont("Arial", 16));
	scoreWidget->setAlignment(Qt::AlignCenter);

	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(rackWidget);
	vLayout->addWidget(buttonContainer);
	vLayout->addWidget(infoWidget);
	vLayout->addWidget(scoreWidget);
	vLayout->setAlignment(rackWidget, Qt::AlignHCenter);
	vLayout->setAlignment(button, Qt::AlignHCenter);
	
	QWidget *rightWidget = new QWidget;
	rightWidget->setLayout(vLayout);

	QHBoxLayout *layout = new QHBoxLayout;
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
			button->setText("Réinitialiser");
		}
		discardButton->setEnabled(true);
		cancelButton->setEnabled(false);
	}
	else{
		if(!gameIsFinished){
			bool ok = player->playTurn();
			if(ok){
				const auto &lastPlay = player->getLastPlay();
				gridWidget->updatePositions(grid, lastPlay.getPositionsChanged());
				QString info = "Le mot '" + QString::fromStdString(lastPlay.getWord()) + "' rapporte "
					+ QString::number(lastPlay.getScore()) + " points.";
				infoWidget->setText(info);
				rackWidget->updateLetters(rack->getLetters());
				cancelButton->setEnabled(true);
			}
			else{
				gameIsFinished = true;
				infoWidget->setText("Pas de mot valide trouvé avec cette configuration.");
			}

			drawLetters = !rack->tilesFinished();

			if(!drawLetters && rack->empty()){
				gameIsFinished = true;
			}
			scoreWidget->setText("Score actuel : " + QString::number(player->getScore()));
			
			if(drawLetters){
				button->setText("Tirer des lettres");
			}
			else if(gameIsFinished){
				button->setText("Réinitialiser");
			}

			if(rack->empty() || !drawLetters){
				discardButton->setEnabled(false);
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
			scoreWidget->setText("Score actuel : 0");
			gameIsFinished = false;
			drawLetters = true;
			noMoreLetters = false;
			discardButton->setEnabled(false);
			cancelButton->setEnabled(false);
		}
	}
}

void ScrabbleMain::manualPlay(unsigned int row, unsigned int column){
	if(!drawLetters && !gameIsFinished){
		bool ok = false;
		QString letters = QInputDialog::getText(this, "Tirage de lettres", 
					"Indiquer le mot à jouer.", 
					QLineEdit::Normal, QString(), &ok);
		if(!ok){
			return;
		}
		auto s = letters.toStdString();
		std::transform(s.begin(), s.end(), s.begin(), 
					::toupper);
		ok = player->playWord(s, row, column);
		
		if(!ok){
			return;
		}
		
		const auto &lastPlay = player->getLastPlay();
		gridWidget->updatePositions(grid, lastPlay.getPositionsChanged());
		QString info = "Le mot '" + QString::fromStdString(lastPlay.getWord()) + "' rapporte "
			+ QString::number(lastPlay.getScore()) + " points.";
		infoWidget->setText(info);
		rackWidget->updateLetters(rack->getLetters());
			
		drawLetters = !rack->tilesFinished();
		if(!drawLetters && rack->empty()){
			gameIsFinished = true;
		}

		scoreWidget->setText("Score actuel : " + QString::number(player->getScore()));
			
		if(drawLetters){
			button->setText("Tirer des lettres");
		}
		
		else if(gameIsFinished){
			button->setText("Réinitialiser");
		}

		if(rack->empty() || !drawLetters){
			discardButton->setEnabled(false);
		}

		cancelButton->setEnabled(true);
	}
}

void ScrabbleMain::discardRack(){
	if(!gameIsFinished && !rack->empty() && !noMoreLetters){
		rack->discard();
		rackWidget->updateLetters({});
		button->setText("Tirer des lettres");
		drawLetters = true;
		discardButton->setEnabled(false);
		cancelButton->setEnabled(false);
	}
}

void ScrabbleMain::cancelLastPlay(){
	auto lastPlay = player->getLastPlay();
	player->cancelLastPlay();
	gridWidget->updatePositions(grid, lastPlay.getPositionsChanged());
	QString info = "Annulation du mot '" + QString::fromStdString(lastPlay.getWord()) + "' qui a rapporté "
		+ QString::number(lastPlay.getScore()) + " points.";
	infoWidget->setText(info);
	rackWidget->updateLetters(rack->getLetters());

	scoreWidget->setText("Score actuel : " + QString::number(player->getScore()));
	
	drawLetters = false;
	gameIsFinished = false;
	cancelButton->setEnabled(false);
	button->setText("Jouer le meilleur coup");
}
