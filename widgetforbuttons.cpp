#include "widgetforbuttons.h"
#include "ui_widgetforbuttons.h"

WidgetForButtons::WidgetForButtons(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetForButtons)
{
	ui->setupUi(this);
	connect(ui->allButtons, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(buttonPressed(QAbstractButton*)));
	connect(ui->button_color, SIGNAL(clicked(bool)),
			this, SLOT(sendColorSignal()));
}

WidgetForButtons::~WidgetForButtons()
{
	delete ui;
}


void WidgetForButtons::buttonPressed(QAbstractButton* but){
	if (but->text() == "Line")
		emit linePressed();
	else if (but->text() == "Rectangle")
		emit rectPressed();
	else if (but->text() == "Circle")
		emit circlePressed();
	else if (but->text() == "Move")
		emit movePressed();
	
}

void WidgetForButtons::sendColorSignal(){
	emit chooseColor();
}
