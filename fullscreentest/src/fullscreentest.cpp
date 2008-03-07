#include <QApplication>
#include <QWidget>
#include "modeselector.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ModeSelectorWindow selectorWindow(&app);
    selectorWindow.setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    selectorWindow.resize(300, 300);
    selectorWindow.show();
	return app.exec();
}
