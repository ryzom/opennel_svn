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

/*
configuration:
- How? CConfigFile is not really suited for the purpose; ModeManagers basicly
  need a key-value-mapping and shouldn't get swamped with file loading or
  other such functionality.
- We will need different data types, like bool, int, string; maybe making use
  of CVar for this?



*/
