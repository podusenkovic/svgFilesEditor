#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <iterator>

#include "figures.h"
#include "widgetforbuttons.h"
#include "choosecolor.h"


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	
private slots:
	void startDrawLine();
	void startDrawRect();
	void startDrawCircle();
	void startMovingAction();
	
	void chooseColor();
	void closeAll();
	void savePictureToFile();
	void clearScreen();
	
	void setColor();
private:
	Ui::MainWindow *ui;
	WidgetForButtons *buttons;
	ChooseColor* colorsWin;
	
	short makingLine;
	short makingRect;
	short makingCircle;
	short movingAnything;
	
	int movingIndex;
	
	QColor mainColor;
	
	QPoint diff;
	
	QPoint startPoint;
	
	QVector<Figure*> figures;
	
	QString startWith;
};

#endif // MAINWINDOW_H
