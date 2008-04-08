#ifndef FULLSCREENTEST_H_
#define FULLSCREENTEST_H_

#include <QApplication>
#include <X11/Xlib.h>

class FullscreenTest : public QApplication {
Q_OBJECT

public:
	FullscreenTest(int argc, char **argv) : QApplication(argc, argv) {}
//	virtual bool x11EventFilter(XEvent *event);
};

#endif /*FULLSCREENTEST_H_*/
