#include "choosecolor.h"
#include "ui_choosecolor.h"

ChooseColor::ChooseColor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChooseColor)
{
	srand(time(NULL));
	ui->setupUi(this);
	connect(ui->button_random, SIGNAL(clicked(bool)),
			this, SLOT(randomSliders()));
	connect(ui->button_done, SIGNAL(clicked(bool)),
			this, SLOT(sendReadyToDraw()));
}

ChooseColor::~ChooseColor()
{
	delete ui;
}

void ChooseColor::on_slider_blue_sliderMoved(int position)
{
    this->update();
}

void ChooseColor::on_slider_green_sliderMoved(int position)
{
    this->update();
}

void ChooseColor::on_slider_red_sliderMoved(int position)
{
	this->update();
}

void ChooseColor::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	painter.setBrush(QColor(ui->slider_red->value(),
							ui->slider_green->value(),
							ui->slider_blue->value()));
	painter.drawRect(210, 30, 170,170);
}

void ChooseColor::randomSliders(){
	ui->slider_red->setValue(rand()%256);
	ui->slider_green->setValue(rand()%256);
	ui->slider_blue->setValue(rand()%256);
	this->update();
}


QColor ChooseColor::getColor(){
	return QColor(ui->slider_red->value(),
			 	  ui->slider_green->value(),
			 	  ui->slider_blue->value());
}


void ChooseColor::sendReadyToDraw(){
	emit readyToDraw();
}
