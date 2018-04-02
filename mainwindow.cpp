#include "mainwindow.h"
#include "ui_mainwindow.h"

#define STARTWITH "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg version = \"1.1\"\n     baseProfile=\"full\"\n     xmlns = \"http://www.w3.org/2000/svg\"\n     xmlns:xlink = \"http://www.w3.org/1999/xlink\"\n     xmlns:ev = \"http://www.w3.org/2001/xml-events\"\n     height = \"%1px\"  width = \"%2px\">\n"

void swapFigures(Figure* a, Figure* b){
	Figure *temp = a;
	a = b;
	b = temp;	
}

double Distance(QPoint a, QPoint b){
	double x = a.x() - b.x();
	double y = a.y() - b.y();
	return sqrt(x*x + y*y);
}

bool isInside(QPoint a, Rect *rect){
	if (rect->topleft.x() < a.x() &&
		rect->topleft.y() < a.y() &&
		rect->topleft.x() + rect->width > a.x() &&
		rect->topleft.y() + rect->height > a.y())
		return true;
	return false;
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	movingIndex = -1;
	startWith = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
				"<svg version = \"1.1\"\n     "
				"baseProfile=\"full\"\n     "
				"xmlns = \"http://www.w3.org/2000/svg\"\n     "
				"xmlns:xlink = \"http://www.w3.org/1999/xlink\"\n     "
				"xmlns:ev = \"http://www.w3.org/2001/xml-events\"\n     "
				"height = \"%1px\"  width = \"%2px\">\n";
	makingLine = 0;
	ui->setupUi(this);
	buttons = new WidgetForButtons();
	
	mainColor = QColor(0,0,0);
	
	QTimer::singleShot(15,buttons, SLOT(show()));
	
	connect(buttons, SIGNAL(linePressed()),
			this, SLOT(startDrawLine()));
	connect(buttons, SIGNAL(rectPressed()),
			this, SLOT(startDrawRect()));
	connect(buttons, SIGNAL(circlePressed()),
			this, SLOT(startDrawCircle()));
	connect(buttons, SIGNAL(movePressed()), 
			this, SLOT(startMovingAction()));
	
	connect(buttons, SIGNAL(chooseColor()),
			this, SLOT(chooseColor()));
	
	connect(ui->actionExit, SIGNAL(triggered(bool)),
			this, SLOT(closeAll()));
	connect(ui->actionSave, SIGNAL(triggered(bool)),
			this, SLOT(savePictureToFile()));
	connect(ui->actionClear, SIGNAL(triggered(bool)),
			this, SLOT(clearScreen()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
	if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down){
		if (movingAnything == 2){
			if (movingIndex != 0 && event->key() == Qt::Key_Down){
				figures.move(movingIndex, movingIndex - 1);
				movingIndex--;
			}
			if (movingIndex != figures.size() - 1 && event->key() == Qt::Key_Up){
				figures.move(movingIndex, movingIndex + 1);
				movingIndex++;
			}
		}
	}
	this->update();
}


void MainWindow::mouseMoveEvent(QMouseEvent *event){
	if (movingAnything == 2){
		if (figures[movingIndex]->type == "circle"){
			((Circle*)figures[movingIndex])->center = event->pos() + diff;
		}
		else if (figures[movingIndex]->type == "rect"){
			((Rect*)figures[movingIndex])->topleft = event->pos() + diff;
		}
	}
	this->update();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
	//-------------LINES-----------------------
	if (makingLine == 1){
		startPoint = event->pos();
		makingLine = 2;
	}
	else if (makingLine == 2){
		figures.push_back(new Line(startPoint, event->pos(), mainColor));
		makingLine = 0;
		buttons->show();
		this->releaseMouse();
	}
	//-----------------------------------------
	//-------------RECTANGLES------------------
	if (makingRect == 1){
		startPoint = event->pos();
		makingRect = 2;
	}
	else if (makingRect == 2){
		QPoint topleft = startPoint;
		QSize size = QSize(event->pos().x() - startPoint.x(), 
						   event->pos().y() - startPoint.y());
		if (size.width() < 0){
			topleft.setX(topleft.x() + size.width());
			size.setWidth(-size.width());
		}
		if (size.height() < 0){
			topleft.setY(topleft.y() + size.height());
			size.setHeight(-size.height());
		}
		figures.push_back(new Rect(topleft, 
								   size.width(), 
								   size.height(), 
								   mainColor));
		makingRect = 0;
		buttons->show();
		this->releaseMouse();
	}
	//-----------------------------------------
	//-------------CIRCLES---------------------
	if (makingCircle == 1){
		startPoint = event->pos();
		makingCircle = 2;
	}
	else if (makingCircle == 2){
		int radius = Distance(startPoint, event->pos());
		//circlesColor.push_back(mainColor);
		figures.push_back(new Circle(startPoint, radius, mainColor));
		makingCircle = 0;
		buttons->show();
		this->releaseMouse();
	}
	//-----------------------------------------
	//--------------MOVING---------------------
	if (movingAnything == 1){
		for (int i = figures.size() - 1; i >= 0; i--){
			if (figures[i]->type == "circle")
				if (Distance(((Circle*)figures[i])->center, event->pos()) < ((Circle*)figures[i])->rad){
					movingIndex = i;
					diff = ((Circle*)figures[i])->center - event->pos();
					movingAnything = 2;
					qDebug() << "pressed on circle";
					break;
				}
			if (figures[i]->type == "rect")
				if (isInside(event->pos(), (Rect*)figures[i])){
					movingIndex = i;
					diff = ((Rect*)figures[i])->topleft - event->pos();
					movingAnything = 2;
					qDebug() << "pressed on rectangle";
					break;
				}
			//if (figures[i]->type == "line")
		}
	}else if (movingAnything == 2){
		((Circle*)figures[movingIndex])->center = event->pos() + diff;
		movingAnything = 0;
		movingIndex = -1;
		buttons->show();
		this->releaseMouse();
	}
}

void MainWindow::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPoint pos = this->mapFromGlobal(QCursor::pos());
	
	for (int i = 0; i < figures.size(); i++){
		figures[i]->draw(&painter);
	}
	
	painter.setPen(QPen(mainColor, 3));
	painter.setBrush(QBrush(mainColor));
	//-------------LINES-----------------------
	if (makingLine == 1){
		painter.drawEllipse(pos,3,3);
	}
	else if (makingLine == 2){
		painter.drawLine(startPoint, pos);
	}
	//-----------------------------------------
	//-------------RECTANGLES------------------
	if (makingRect == 1){
		painter.drawEllipse(pos,3,3);
	}
	else if (makingRect == 2){
		painter.drawRect(startPoint.x(), startPoint.y(), 
						 pos.x() - startPoint.x(), pos.y() - startPoint.y());
	}
	//-----------------------------------------
	//-------------CIRCLES---------------------
	if (makingCircle == 1){
		painter.drawEllipse(pos,3,3);
	}
	else if (makingCircle == 2){
		int radius = Distance(startPoint, pos);
		painter.drawEllipse(startPoint, radius, radius);
	}
	//-----------------------------------------
}


void MainWindow::startDrawLine(){
	buttons->hide();
	this->grabMouse();
	this->setMouseTracking(true);
	makingLine = 1;
}


void MainWindow::startDrawRect(){
	buttons->hide();
	this->grabMouse();
	this->setMouseTracking(true);
	makingRect = 1;
}

void MainWindow::startDrawCircle(){
	buttons->hide();
	this->grabMouse();
	this->setMouseTracking(true);
	makingCircle = 1;
}

void MainWindow::startMovingAction(){
	buttons->hide();
	this->grabMouse();
	this->setMouseTracking(true);
	movingAnything = 1;
}

void MainWindow::closeAll(){
	buttons->close();
	this->close();
}

void MainWindow::savePictureToFile(){
	QFile file("picture.svg");
	file.open(QIODevice::WriteOnly);
	QTextStream fileStream(&file);
	fileStream << startWith.arg(this->size().width()).arg(this->size().height()) + "\n";
	for (int i = 0; i < figures.size(); i++){
		if (figures[i]->type == "line")
			fileStream << QString("\n <line x1=\"%1\" y1=\"%2\" "
								  "x2=\"%3\" y2=\"%4\" stroke-width=\"1\" "
								  "stroke=\"rgb(%5)\" fill = \"rgb(%6)\"/>\n")
						  .arg(((Line*)figures[i])->p1.x())
						  .arg(((Line*)figures[i])->p1.y())
						  .arg(((Line*)figures[i])->p2.x())
						  .arg(((Line*)figures[i])->p2.y())
						  .arg(figures[i]->getColor())
						  .arg(figures[i]->getColor());
		else if (figures[i]->type == "rect")
			fileStream << QString("\n <rect x=\"%1\" y=\"%2\" "
								  "width=\"%3\" height=\"%4\" stroke-width=\"1\" "
								  "stroke=\"rgb(%5)\" fill = \"rgb(%6)\"/>\n")
						  .arg(((Rect*)figures[i])->topleft.x())
						  .arg(((Rect*)figures[i])->topleft.y())
						  .arg(((Rect*)figures[i])->width)
						  .arg(((Rect*)figures[i])->height)
						  .arg(figures[i]->getColor())
						  .arg(figures[i]->getColor());
		else if (figures[i]->type == "circle")
			fileStream << QString("\n <circle cx=\"%1\" cy=\"%2\" " 
								  "r=\"%3\" stroke-width=\"1\" "
								  "stroke=\"rgb(%4)\" fill = \"rgb(%5)\"/>\n")
						  .arg(((Circle*)figures[i])->center.x())
						  .arg(((Circle*)figures[i])->center.y())
						  .arg(((Circle*)figures[i])->rad)
						  .arg(figures[i]->getColor())
						  .arg(figures[i]->getColor());
	}
	fileStream << "\n</svg>";
}


void MainWindow::chooseColor(){
	colorsWin = new ChooseColor();
	colorsWin->show();
	connect(colorsWin, SIGNAL(readyToDraw()),
			this, SLOT(setColor()));
}

void MainWindow::setColor(){
	colorsWin->hide();
	mainColor = colorsWin->getColor();
	qDebug() << "got new color";
}

void MainWindow::clearScreen(){
	figures.erase(figures.begin(), figures.end());
	this->update();
}



