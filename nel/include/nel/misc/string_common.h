/** \file misc/string_common.h
 * common algorithms on string like toString() and fromString()
 *
 * $Id$
 */

/* Copyright, 2003 Nevrax Ltd.
 *
 * This file is part of NEVRAX NEL.
 * NEVRAX NEL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX NEL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX NEL; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef	NL_STRING_COMMON_H
#define	NL_STRING_COMMON_H

#include "types_nl.h"

#include <cstdio>
#include <cstdarg>

namespace NLMISC
{

// get a string and add \r before \n if necessary
std::string addSlashR (std::string str);
std::string removeSlashR (std::string str);

/**
 * \def MaxCStringSize
 *
 * The maximum size allowed for C string (zero terminated string) buffer.
 * This value is used when we have to create a standard C string buffer and we don't know exactly the final size of the string.
 */
const int MaxCStringSize = 2048;


/**
 * \def NLMISC_CONVERT_VARGS(dest,format)
 *
 * This macro converts variable arguments into C string (zero terminated string).
 * This function takes care to avoid buffer overflow.
 *
 * Example:
 *\code
	void MyFunction(const char *format, ...)
	{
		string str;
		NLMISC_CONVERT_VARGS (str, format, NLMISC::MaxCStringSize);
		// str contains the result of the conversion
	}
 *\endcode
 *
 * \param _dest \c string or \c char* that contains the result of the convertion
 * \param _format format of the string, it must be the last argument before the \c '...'
 * \param _size size of the buffer that will contain the C string
 */
#define NLMISC_CONVERT_VARGS(_dest,_format,_size) \
char _cstring[_size]; \
va_list _args; \
va_start (_args, _format); \
int _res = vsnprintf (_cstring, _size-1, _format, _args); \
if (_res == -1 || _res == _size-1) \
{ \
	_cstring[_size-1] = '\0'; \
} \
va_end (_args); \
_dest = _cstring



/** sMart sprintf function. This function do a sprintf and add a zero at the end of the buffer
 * if there no enough room in the buffer.
 *
 * \param buffer a C string
 * \param count Size of the buffer
 * \param format of the string, it must be the last argument before the \c '...'
 */
sint smprintf( char *buffer, size_t count, const char *format, ... );


#ifdef NL_OS_WINDOWS

//
// These functions are needed by template system to failed the compilation if you pass bad type on nlinfo...
//

inline void _check(int a) { }
inline void _check(unsigned int a) { }
inline void _check(char a) { }
inline void _check(unsigned char a) { }
inline void _check(long a) { }
inline void _check(unsigned long a) { }

#ifdef NL_COMP_VC6
	inline void _check(uint8 a) { }
#endif // NL_COMP_VC6

inline void _check(sint8 a) { }
inline void _check(uint16 a) { }
inline void _check(sint16 a) { }

#ifdef NL_COMP_VC6
	inline void _check(uint32 a) { }
	inline void _check(sint32 a) { }
#endif // NL_COMP_VC6

inline void _check(uint64 a) { }
inline void _check(sint64 a) { }
inline void _check(float a) { }
inline void _check(double a) { }
inline void _check(const char *a) { }
inline void _check(const void *a) { }

#define CHECK_TYPES(__a,__b) \
inline __a(const char *fmt) { __b(fmt); } \
template<class A> __a(const char *fmt, A a) { _check(a); __b(fmt, a); } \
template<class A, class B> __a(const char *fmt, A a, B b) { _check(a); _check(b); __b(fmt, a, b); } \
template<class A, class B, class C> __a(const char *fmt, A a, B b, C c) { _check(a); _check(b); _check(c); __b(fmt, a, b, c); } \
template<class A, class B, class C, class D> __a(const char *fmt, A a, B b, C c, D d) { _check(a); _check(b); _check(c); _check(d); __b(fmt, a, b, c, d); } \
template<class A, class B, class C, class D, class E> __a(const char *fmt, A a, B b, C c, D d, E e) { _check(a); _check(b); _check(c); _check(d); _check(e); __b(fmt, a, b, c, d, e); } \
template<class A, class B, class C, class D, class E, class F> __a(const char *fmt, A a, B b, C c, D d, E e, F f) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); __b(fmt, a, b, c, d, e, f); } \
template<class A, class B, class C, class D, class E, class F, class G> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); __b(fmt, a, b, c, d, e, f, g); } \
template<class A, class B, class C, class D, class E, class F, class G, class H> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); __b(fmt, a, b, c, d, e, f, g, h); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); __b(fmt, a, b, c, d, e, f, g, h, i); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); __b(fmt, a, b, c, d, e, f, g, h, i, j); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); __b(fmt, a, b, c, d, e, f, g, h, i, j, k); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T, class U> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t, U u) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); _check(u); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T, class U, class V> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t, U u, V v) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); _check(u); _check(v); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T, class U, class V, class W> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t, U u, V v, W w) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); _check(u); _check(v); _check(w); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T, class U, class V, class W, class X> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t, U u, V v, W w, X x) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); _check(u); _check(v); _check(w); _check(x); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T, class U, class V, class W, class X, class Y> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t, U u, V v, W w, X x, Y y) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); _check(u); _check(v); _check(w); _check(x); _check(y); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y); } \
template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K, class L, class M, class N, class O, class P, class Q, class R, class S, class T, class U, class V, class W, class X, class Y, class Z> __a(const char *fmt, A a, B b, C c, D d, E e, F f, G g, H h, I i, J j, K k, L l, M m, N n, O o, P p, Q q, R r, S s, T t, U u, V v, W w, X x, Y y, Z z) { _check(a); _check(b); _check(c); _check(d); _check(e); _check(f); _check(g); _check(h); _check(i); _check(j); _check(k); _check(l); _check(m); _check(n); _check(o); _check(p); _check(q); _check(r); _check(s); _check(t); _check(u); _check(v); _check(w); _check(x); _check(y); _check(z); __b(fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z); }

#endif
	
#ifdef NL_OS_WINDOWS
	inline std::string _toString(const char *format, ...)
#else
	inline std::string toString(const char *format, ...)
#endif
{
	std::string Result;
	NLMISC_CONVERT_VARGS(Result, format, NLMISC::MaxCStringSize);
	return Result;
}

#ifdef NL_OS_WINDOWS
	CHECK_TYPES(std::string toString, return _toString)
#endif // NL_OS_WINDOWS

template<class T> std::string toStringPtr(const T *val) { return toString("%p", val); }

template<class T> std::string toStringEnum(const T &val) { return toString("%u", (uint32)val); }

/**
 * Template Object toString.
 * \param obj any object providing a "std::string toString()" method. The object doesn't have to derive from anything.
 * 
 * the VC++ error "error C2228: left of '.toString' must have class/struct/union type" means you don't provide
 * a toString() method to your object.
 */
template<class T> std::string toString(const T &obj)
{
	return obj.toString();
}

//inline std::string toString(const char *val) { return toString("%s", val); }
inline std::string toString(const uint8 &val) { return toString("%hu", (uint16)val); }
inline std::string toString(const sint8 &val) { return toString("%hd", (sint16)val); }
inline std::string toString(const uint16 &val) { return toString("%hu", val); }
inline std::string toString(const sint16 &val) { return toString("%hd", val); }
inline std::string toString(const uint32 &val) { return toString("%u", val); }
inline std::string toString(const sint32 &val) { return toString("%d", val); }
inline std::string toString(const uint64 &val) { return toString("%"NL_I64"u", val); }
inline std::string toString(const sint64 &val) { return toString("%"NL_I64"d", val); }
inline std::string toString(const float &val) { return toString("%f", val); }
inline std::string toString(const double &val) { return toString("%lf", val); }
inline std::string toString(const bool &val) { return toString("%u", val?1:0); }
inline std::string toString(const std::string &val) { return val; }

// stl vectors of bool use bit reference and not real bools, so define the operator for bit reference

// Debug : Sept 01 2006
#if _STLPORT_VERSION >= 0x510
	inline std::string toString(const std::priv::_Bit_reference &val) { return toString( bool(val)); }
#else
	inline std::string toString(const std::_Bit_reference &val) { return toString( bool(val)); }
#endif // _STLPORT_VERSION

#ifdef NL_COMP_VC6
inline std::string toString(const uint &val) { return toString("%u", val); }
inline std::string toString(const sint &val) { return toString("%d", val); }
#endif // NL_COMP_VC6

template<class T> 
void fromString(const std::string &str, T &obj)
{
	obj.fromString(str);
}

inline void fromString(const std::string &str, uint32 &val) { sscanf(str.c_str(), "%u", &val); }
inline void fromString(const std::string &str, sint32 &val) { sscanf(str.c_str(), "%d", &val); }
inline void fromString(const std::string &str, uint8 &val) { uint32 v; fromString(str, v); val = (uint8)v; }
inline void fromString(const std::string &str, sint8 &val) { sint32 v; fromString(str, v); val = (sint8)v; }
inline void fromString(const std::string &str, uint16 &val) { uint32 v; fromString(str, v); val = (uint16)v; }
inline void fromString(const std::string &str, sint16 &val) { uint32 v; fromString(str, v); val = (sint16)v; }
inline void fromString(const std::string &str, uint64 &val) { sscanf(str.c_str(), "%"NL_I64"u", &val); }
inline void fromString(const std::string &str, sint64 &val) { sscanf(str.c_str(), "%"NL_I64"d", &val); }
inline void fromString(const std::string &str, float &val) { sscanf(str.c_str(), "%f", &val); }
inline void fromString(const std::string &str, double &val) { sscanf(str.c_str(), "%lf", &val); }
inline void fromString(const std::string &str, bool &val) { uint32 v; fromString(str, v); val = (v==1); }
inline void fromString(const std::string &str, std::string &val) { val = str; }

// stl vectors of bool use bit reference and not real bools, so define the operator for bit reference

// Debug : Sept 01 2006
#if _STLPORT_VERSION >= 0x510
	inline void fromString(const std::string &str, std::priv::_Bit_reference &val) { uint32 v; fromString(str, v); val = (v==1); }
#else
	inline void fromString(const std::string &str, std::_Bit_reference &val) { uint32 v; fromString(str, v); val = (v==1); }
#endif // _STLPORT_VERSION

#ifdef NL_COMP_VC6
inline void fromString(const std::string &str, uint &val) { sscanf(str.c_str(), "%u", &val); }
inline void fromString(const std::string &str, sint &val) { sscanf(str.c_str(), "%d", &val); }
#endif // NL_COMP_VC6


} // NLMISC

#endif	// NL_STRING_COMMON_H
