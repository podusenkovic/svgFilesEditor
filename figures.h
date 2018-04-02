#ifndef FIGURES_H
#define FIGURES_H

#include <QString>
#include <QPoint>
#include <QColor>
#include <QPainter>

class Figure{
	friend class MainWindow;
protected:
	QString type;
	QColor color;
public:
	Figure(QString t, QColor c) : type(t), color(c) {}
	virtual void draw(QPainter*) = 0;
	QString getColor(){
		return QString::number(color.red()) + "," +
				QString::number(color.green()) + "," +
				 QString::number(color.blue());
	}
};


class Circle : public Figure{
	friend class MainWindow;
	QPoint center;
	int rad;
public:
	Circle(QPoint c, int r, QColor col) : Figure("circle", col),
		center(c), rad(r){}
	
	void draw(QPainter *painter){
		painter->setBrush(QBrush(color));
		painter->setPen(QPen(color, 3));
		painter->drawEllipse(center, rad, rad);
	}
};


class Rect : public Figure{
	friend class MainWindow;
	friend bool isInside(QPoint, Rect*);
	QPoint topleft;
	int width, height;
public:
	Rect(QPoint p_1,
		 int w, int h,
		 QColor col) : Figure("rect", col),
		topleft(p_1), width(w), height(h){}

	void draw(QPainter *painter){
		painter->setBrush(QBrush(color));
		painter->setPen(QPen(color, 3));
		painter->drawRect(QRect(topleft, QSize(width, height)));
	}
};

class Line : public Figure{
	friend class MainWindow;
	QPoint p1, p2;
public:
	Line(QPoint p_1, QPoint p_2, QColor col) : Figure("line", col),
		p1(p_1), p2(p_2){}
	
	void draw(QPainter *painter){
		painter->setPen(QPen(color,3));
		painter->drawLine(QLine(p1,p2));
	}
};

#endif // FIGURES_H
