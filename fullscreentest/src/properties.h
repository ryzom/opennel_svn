#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>
#include <tr1/unordered_map>
#define CHashMap std::tr1::unordered_map
#include "variant.h"


/**
 * Properties map names to mixed data, in this case the
 * CVariant type, which can hold a set of simple data types
 * like integers and pointers of all sorts.
 * 
 * It offers just one method/operator, the array index operator,
 * which is relayed to the underlying map.
 * 
 * Usage examples:
 * 	 CProperties<uintptr_t> props;
 *   CVariant<uintptr_t> var = 300;
 *   props["highlife"] = var;
 *   props["highlife"].setData(500);
 *   props["highlife"] = true;
 *   bool b = props["highlife"];
 * 
 */
template<class Z> class CProperties {
public:
	CProperties() {}
	
	CVariant<Z>& operator[](std::string& key) { return properties[key]; }
	CVariant<Z>& operator[](const char *key) { return properties[key]; }
	
private:
	CHashMap<std::string, CVariant<Z> > properties;
};


#endif /*PROPERTIES_H_*/
