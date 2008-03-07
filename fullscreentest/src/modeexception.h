#ifndef MODEEXCEPTION_H_
#define MODEEXCEPTION_H_

#include <exception>

class ModeException
{
public:
	ModeException(const char *cause) {
		causeText = cause;
	}
	
	virtual const char* what() {
		return causeText;
	}
	
private:
	const char *causeText;

};

#endif /*MODEEXCEPTION_H_*/
