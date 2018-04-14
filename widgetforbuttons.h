#ifndef WIDGETFORBUTTONS_H
#define WIDGETFORBUTTONS_H

#include <QWidget>
#include <QAbstractButton>
#include <QDebug>
#include <QTimer>

namespace Ui {
	class WidgetForButtons;
}

class WidgetForButtons : public QWidget
{
	Q_OBJECT
	
signals:
	void linePressed();
	void rectPressed();
	void circlePressed();
	void movePressed();
	void polyPressed();
	void chooseColor();

private slots:
	void buttonPressed(QAbstractButton*);
	
	void sendColorSignal();
public:
	explicit WidgetForButtons(QWidget *parent = 0);
	~WidgetForButtons();
	
private:
	Ui::WidgetForButtons *ui;
};

#endif // WIDGETFORBUTTONS_H
