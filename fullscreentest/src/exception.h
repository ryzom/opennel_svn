#ifndef MODEEXCEPTION_H_
#define MODEEXCEPTION_H_

#include <exception>

class Exception : public std::exception {
private:
	const char *cause;
public:
	Exception(const char *cause) {
		this->cause = cause;
	}
	virtual ~Exception() throw() {
	}
	virtual const char *what() const throw() {
		return cause;
	}
};

class ModeException : Exception {
public:
	ModeException(const char *msg) : Exception(msg) {};
};

#endif /*MODEEXCEPTION_H_*/
