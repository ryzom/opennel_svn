#include <QVBoxLayout>
#include <QString>
#include <vector>
#include <iostream>
#include <stdint.h>
#include "modeselector.h"
#include "gfxmode.h"

ModeSelectorWindow::ModeSelectorWindow(QApplication *app)
{
	char modename[32]; // that should be enough space for now
	selectedMode = NULL;
	modeComboBox = new QComboBox();
	connect(modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMode(int)));

	if (manager.initLibraries()) {
		manager.initModes();
		const std::vector<GfxMode*> &list = manager.getModes();
		GfxMode * const *data = list.data();
		for (unsigned int i = 0; i < list.size(); i++) {
			GfxMode *mode = data[i];
			snprintf(modename, 30, "%dx%d_%d", mode->Width, mode->Height, (int)mode->Frequency);
			modeComboBox->addItem(QString(modename), QVariant::fromValue((void*)mode));
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

void ModeSelectorWindow::setWindowFlags(Qt::WindowFlags flags)
{
	QWidget::setWindowFlags(flags);
}

void ModeSelectorWindow::selectMode(int index)
{
	if (modeComboBox == 0)
		return;
	QVariant variantData = modeComboBox->itemData(index);
	selectedMode = (GfxMode*)variantData.value<void*>();
	std::cout << "selected mode: " << selectedMode->Width << "x" << selectedMode->Height << "_" <<
		((int)selectedMode->Frequency) << " @(" << selectedMode->OriginX << "," << selectedMode->OriginY <<
		")" << std::endl;
}

void ModeSelectorWindow::switchMode()
{
	if (selectedMode != NULL && selectedMode->Manager != 0) {
		selectedMode->Manager->setMode(selectedMode);
		setGeometry(selectedMode->OriginX, selectedMode->OriginY, selectedMode->Width, selectedMode->Height);
	}
}
