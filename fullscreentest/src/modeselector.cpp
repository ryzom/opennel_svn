#include <QVBoxLayout>
#include <QString>
#include <vector>
#include <iostream>
#include <stdint.h>
#include "modeselector.h"
#include "gfxmode.h"
#include "propdefs.h"

const char *blubb[] = { "1000x500-0-0", "800x600+200+100", "1680x1050+1600+0" };

ModeSelectorWindow::ModeSelectorWindow(QApplication *app) {
	char modename[MODE_NAME_LENGTH]; // that should be enough space for now
	selectedMode = NULL;
	modeComboBox = new QComboBox();
	connect(modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMode(int)));
	
	// initialize Xext wrapper objects and dump them into the factory
	xvidwrapper = new XVidModeExtensionWrapper();
	wrapperFactory.addWrapper(xvidwrapper);
	xinwrapper = new XineramaExtensionWrapper();
	wrapperFactory.addWrapper(xinwrapper);
	xrandrwrapper = new XRandRExtensionWrapper();
	wrapperFactory.addWrapper(xrandrwrapper);

	// TODO how do we select automagically between them? Do we
	// even want this?
	//manager = new XVidModeManager(&wrapperFactory);
	//manager = new XRandrModeManager(&wrapperFactory);
	manager = new UserModeManager(&wrapperFactory);
	
	(*manager)[P_USE_GLX_TEST] = false;
	(*manager)[P_USER_MODES] = blubb;
	(*manager)[P_USE_XINERAMA] = true; //false;
	if (manager->initLibraries()) {
		manager->initModes();
		const std::vector<GfxMode*> &list = manager->getModes();
		GfxMode * const *data = list.data();
		for (unsigned int i = 0; i < list.size(); i++) {
			GfxMode *mode = data[i];
			snprintf(modename, MODE_NAME_LENGTH-1, "%dx%d_%d /%d", mode->Width, mode->Height,
					(int)mode->Frequency, (int)mode->CRTC);
			modeComboBox->addItem(QString(modename),
					QVariant::fromValue((void*)mode));
		}
	}
	switchButton = new QPushButton(tr("&Switch"));
	connect(switchButton, SIGNAL(clicked()), this, SLOT(switchMode()));
	quitButton = new QPushButton(tr("&Quit"));
	connect(quitButton, SIGNAL(clicked()), app, SLOT(quit()));
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(modeComboBox);
	mainLayout->addWidget(switchButton);
	mainLayout->addWidget(quitButton);
	setLayout(mainLayout);
	setWindowTitle(tr("Don't spy on me, I'm a trojan!"));
}

ModeSelectorWindow::~ModeSelectorWindow() {
	if (manager != 0)
		delete manager;
	if (xinwrapper != 0)
		delete xinwrapper;
	if (xvidwrapper != 0)
		delete xvidwrapper;
	if (xrandrwrapper != 0)
		delete xrandrwrapper;
}

void ModeSelectorWindow::setWindowFlags(Qt::WindowFlags flags) {
	QWidget::setWindowFlags(flags);
}

void ModeSelectorWindow::selectMode(int index) {
	if (modeComboBox == 0)
		return;
	QVariant variantData = modeComboBox->itemData(index);
	if (variantData == QVariant::Invalid)
		return;
	selectedMode = (GfxMode*)variantData.value<void*>();
	std::cout << "selected mode: " << selectedMode->Width << "x"
			<< selectedMode->Height << "_" << ((int)selectedMode->Frequency)
			<< " @(" << selectedMode->OriginX << "," << selectedMode->OriginY
			<< ")" << std::endl;
}

void ModeSelectorWindow::switchMode() {
	if (selectedMode != NULL && selectedMode->Manager != 0) {
		if (selectedMode->Manager->setMode(selectedMode))
			setGeometry(selectedMode->OriginX, selectedMode->OriginY,
					selectedMode->Width, selectedMode->Height);
		else
			std::cout << "failed to set mode" << std::endl;
	}
}
