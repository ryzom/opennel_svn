#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>
#include <map>
#include "variant.h"

template<class Z> class CProperties {
public:
	CProperties() {}
	
	CVariant<Z>& operator[](std::string& key) { return properties[key]; }
	CVariant<Z>& operator[](const char *key) { return properties[key]; }
	
private:
	std::map<std::string, CVariant<Z> > properties;
};


#endif /*PROPERTIES_H_*/
