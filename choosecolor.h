#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include <QWidget>
#include <QPainter>
#include <time.h>

namespace Ui {
	class ChooseColor;
}

class ChooseColor : public QWidget
{
	Q_OBJECT
	
public:
	explicit ChooseColor(QWidget *parent = 0);
	~ChooseColor();
	
	QColor getColor();

signals:
	void readyToDraw();
	
private slots:
	void sendReadyToDraw();
	void randomSliders();
	
	void on_slider_blue_sliderMoved(int position);
	
	void on_slider_green_sliderMoved(int position);
	
	void on_slider_red_sliderMoved(int position);
	
	void paintEvent(QPaintEvent *event);
private:
	Ui::ChooseColor *ui;
};

#endif // CHOOSECOLOR_H
