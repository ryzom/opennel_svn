#ifndef VARIANT_H_
#define VARIANT_H_

/**
 * Simple variant data type. It only allows to use basic types
 * like integers, bools and such, as well as pointers. Complex
 * datatypes such as other classes (pointers to classes are ok)
 * are potentially risky to use.
 * 
 * Use the biggest datatype to be stored in the variant as the
 * class Z, i.e. for pointers one could pick uintptr_t, for
 * example.
 */
template<class Z> class CVariant {
public:
	CVariant() : data(0) {}
	template<class T> CVariant(T newData);
	
	template<class T> T getData();
	template<class T> void setData(T newData);
	
	template<class T> CVariant<Z>& operator=(T newData);
	template<class T> operator T();

private:
	Z data;
};

template<class Z> template<class T> CVariant<Z>::CVariant(T newData) {
	setData<T>(newData);
}

template<class Z> template<class T> T CVariant<Z>::getData() {
	return *reinterpret_cast<T*>(&data);
}

template<class Z> template<class T> void CVariant<Z>::setData(T newData) {
	data = *reinterpret_cast<Z*>(&newData);
}

template<class Z> template<class T> CVariant<Z>& CVariant<Z>::operator=(T newData) {
	setData(newData);
	return *this;
}

template<class Z> template<class T> CVariant<Z>::operator T() {
	return getData<T>();
}

#endif /*VARIANT_H_*/
