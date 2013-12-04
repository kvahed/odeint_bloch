/*
 * Container.hpp
 *
 *  Created on: May 28, 2013
 *      Author: kvahed
 */

#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include <iostream>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <vector>

#if defined (_MSC_VER) && _MSC_VER<1300
#    define VECTOR_TYPE(A) std::vector<A>
#    define VECTOR_CONSTR(A,B) std::vector<A>(B)
#    define VECTOR_CONSTR_VAL(A,B,C) std::vector<A>(B,C)
#else
#    if defined USE_VALARRAY
#        include <valarray>
#        define VECTOR_TYPE(A) std::valarray<A>
#        define VECTOR_CONSTR(A,B) std::valarray<A>(B)
#    else
#        include "Allocator.hpp"
#        if defined __AVX__
#            define ALIGNEMENT 32
//#            warning "AVX"
#        elif defined __SSE2__
#            define ALIGNEMENT 16
//#            warning "SSE2"
#        endif
#        define VECTOR_TYPE(A) std::vector<A,AlignmentAllocator<A,ALIGNEMENT> >
#        define VECTOR_CONSTR(A,B) std::vector<A,AlignmentAllocator<A,ALIGNEMENT> >(B)
#        define VECTOR_CONSTR_VAL(A,B,C) std::vector<A,AlignmentAllocator<A,ALIGNEMENT> >(B,C)
#    endif
#endif

typedef unsigned char cbool;

/**
 * @brief   Memory paradigm (share, opencl or message passing)
 */
enum    paradigm {

  SHM, /**< @brief Shared memory (Local RAM) */
  OCL, /**< @brief Open CL GPU RAM */
  MPI  /**< @brief Distributed memory */

};

namespace codeare {

template <class T, paradigm P=SHM>
class container {
public:
	typedef typename VECTOR_TYPE(T)::iterator iterator;
	typedef typename VECTOR_TYPE(T)::const_iterator const_iterator;
	inline container () { _data = VECTOR_CONSTR (T,0); }
	inline container (const size_t n) { assert(n>0); _data = VECTOR_CONSTR (T,n); }
	inline container (const size_t n, const T t) { assert(n>0); _data = VECTOR_CONSTR_VAL (T,n,t); }
	inline T& operator[] (const size_t n) { return _data[n]; }
	inline T operator[] (const size_t n) const { return _data[n]; }
	inline void operator= (const T t) {
		for (size_t i = 0; i < _data.size(); ++i)
			_data[i] = t;
	}
	inline T& back () { return _data.back(); }
	inline T back () const { return _data.back; }
	inline T& front () { return _data.front(); }
	inline T front () const { return _data.front; }
	inline T& at (const size_t n) { return _data.at(n); }
	inline T at (const size_t n) const { return _data.at(n); }
	inline const T* ptr (const size_t n = 0) const { return &_data[n]; }
	inline T* ptr (const size_t n = 0)  { return &_data[n]; }
	inline VECTOR_TYPE(T) data() const { return _data; }
	inline VECTOR_TYPE(T)& data() { return _data; }
	inline size_t size() const { return _data.size(); }
	inline ~container () {}
	inline iterator begin() { return _data.begin(); }
	inline iterator end() { return _data.end(); }
	inline const_iterator begin() const { return _data.begin(); }
	inline const_iterator end() const { return _data.end(); }
	inline void resize (const size_t n, const T val = T()) { assert(n>0); _data.resize(n,val); }
	inline void push_back (const T& t) { _data.push_back(t);}
	inline void pop_back () {_data.pop_back(); }
	inline bool empty () const { return _data.empty(); }
	inline iterator insert (const iterator pos, const T val) { return _data.insert (pos, val); };
	inline iterator erase (const iterator pos) {return _data.erase(pos); }
	template <class S> container<S> operator() () {
		container<S> cs (size());
		std::copy(_data.begin(), _data.end(), cs.begin());
	}
	template<class S> container (const container<S>& cs) {
		_data.resize(cs.size());
		std::copy(cs.begin(), cs.end(), _data.begin());
	}
private:
	VECTOR_TYPE(T) _data;
};

}


template<class T> inline T ct_real (const std::complex<T> ct) {return ct.real();}
template<class T> inline T ct_imag (const std::complex<T> ct) {return ct.imag();}
template<class T> inline static codeare::container<T>
real (const codeare::container<std::complex<T> >& c) {
	codeare::container<T> res (c.size());
	std::transform (c.begin(), c.end(), res.begin(), ct_real<T>);
	return res;
}
template<class T> inline static codeare::container<T>
imag (const codeare::container<std::complex<T> >& c) {
	codeare::container<T> res (c.size());
	std::transform (c.begin(), c.end(), res.begin(), ct_imag<T>);
	return res;
}

template<class T> inline std::ostream&
operator<< (std::ostream& os, const codeare::container<T>& ct) {
    for (typename codeare::container<T>::const_iterator it = ct.begin(); it != ct.end(); ++it)
        os << *it << " ";
    return os;
}

template<class T> inline static T multiply (const T a, const T b) {
    return (a*b);
}
template<class T> inline static T prod (const codeare::container<T>& ct) {
	return std::accumulate(ct.begin(), ct.end(), (T)1, multiply<T>);
}


#endif /* CONTAINER_HPP_ */
