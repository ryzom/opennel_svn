#ifndef MODESELECTOR_H_
#define MODESELECTOR_H_

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QApplication>
#include "xvidmodemanager.h"


class ModeSelectorWindow : public QWidget
{
	Q_OBJECT

public:
	ModeSelectorWindow(QApplication *app);
	void setWindowFlags(Qt::WindowFlags flags);
//private slots:
//	void updatePreview();
public slots:
	void selectMode(int index);
	void switchMode();
	
private:
	QPushButton *quitButton, *switchButton;
	QComboBox	*modeComboBox;
	
	XVidModeManager manager;
	GfxMode *selectedMode;
};

#endif /*MODESELECTOR_H_*/
