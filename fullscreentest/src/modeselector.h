#ifndef MODESELECTOR_H_
#define MODESELECTOR_H_

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QApplication>
#include "xvidmodeextwrapper.h"
#include "xineramaextwrapper.h"
#include "xvidmodemanager.h"
#include "xrandrmodemanager.h"

/**
 * \brief Length of the modename buffer in bytes.
 */
#define MODE_NAME_LENGTH 32

class ModeSelectorWindow : public QWidget {
Q_OBJECT

public:
	ModeSelectorWindow(QApplication *app);
	virtual ~ModeSelectorWindow();
	void setWindowFlags(Qt::WindowFlags flags);
	//private slots:
	//	void updatePreview();
public slots:
	void selectMode(int index);
	void switchMode();

private:
	QPushButton *quitButton, *switchButton;
	QComboBox *modeComboBox;

	XVidModeExtensionWrapper *xvidwrapper;
	XineramaExtensionWrapper *xinwrapper;
	XRandRExtensionWrapper *xrandrwrapper;
	ExtensionWrapperFactory wrapperFactory;
	//XVidModeManager *manager;
	XRandrModeManager *manager;
	GfxMode *selectedMode;
};

#endif /*MODESELECTOR_H_*/
