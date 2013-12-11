#ifndef __ND_DATA_H__
#define __ND_DATA_H__

#include "Container.hpp"

#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <typeinfo>

template<class T>
struct T_minmax {
	T min, max;
};

template<class T> inline std::ostream&
operator<< (std::ostream& os, const T_minmax<T>& mm) {
	return os << "[" << mm.min << ", " << mm.max << "]";
}

template<class T> inline static T
sum (const codeare::container<T>& v) {
	return std::accumulate(v.begin(), v.end(), (T)0);
}

template<class T> inline const T_minmax<T>
minmax (const codeare::container<T>& v) {
	T_minmax<T> ret;
	for (size_t i = 0; i < v.size(); ++i){
		ret.min = std::min(ret.min,v[i]);
		ret.max = std::max(ret.max,v[i]);
	}
	return ret;
}


/**
 * @brief  Simple nd-data structure.
 */
template<class T>
class NDData {


protected:

    codeare::container<size_t> _dims;  /**< dimensions */
    codeare::container<size_t> _dszs;
    codeare::container<T>      _data;  /**< actual data */

    inline void Allocate () {
    	size_t nd = _dims.size();
    	_dszs.resize(nd);
    	_dszs[0]  = 1;
    	for (size_t i = 1; i < nd; ++i)
    		_dszs[i] = _dims[i-1]*_dszs[i-1];
    	_data.resize(prod(_dims));
    }

public:

	/**
	 * @brief Default constructor
	 */
    NDData () {};

    /**
     * @brief 1D-data (convenience constructor)
     *
     * @param n0 Vector length
     */
    NDData (const size_t n0) {
    	_dims.resize(1,n0);
    	Allocate();
    }

    /**
     * @brief 2D-data (convenience constructor)
     *
     * @param n0 First side
     * @param n1 Second side
     */
    explicit NDData (const size_t n0, const size_t n1) {
    	_dims.resize(2);
    	_dims[0] = n0;
    	_dims[1] = n1;
    	Allocate();
    }

    /**
     * @brief 3D-data (convenience constructor)
     *
     * @param n0 First side
     * @param n1 Second side
     * @param n2 Third side
     */
    explicit NDData (const size_t n0, const size_t n1, const size_t n2) {
    	_dims.resize(3);
    	_dims[0] = n0;
    	_dims[1] = n1;
    	_dims[2] = n2;
    	Allocate();
    }

    /**
     * @brief 4D-data (convenience constructor)
	 *
     * @param n0 First side
     * @param n1 Second side
     * @param n2 Third side
     */
    explicit NDData (const size_t n0, const size_t n1, const size_t n2, const size_t n3) {
    	_dims.resize(4);
    	_dims[0] = n0;
    	_dims[1] = n1;
    	_dims[2] = n2;
    	_dims[3] = n3;
    	Allocate();
    }

    /**
     * @brief ND-Data
     *
     * @param dims  Side lengths
     */
	NDData (const codeare::container<size_t>& dims) {
		_dims = dims;
    	Allocate();
	}

    /**
     * @brief ND-Data
     *
     * @param dims  Side lengths
     */
	explicit NDData (const codeare::container<size_t>& dims, const T t) {
		_dims = dims;
    	Allocate();
    	_data = t;
	}

	/**
	 * @brief Copy constructor
	 *
	 * @param data To copy
	 */
	NDData (const NDData& data) {
		*this = data;
	}

	/**
	 * @brief Assignement
	 *
	 * @param data To copy
	 */
	NDData& operator= (const NDData& data) {
		_dims = data._dims;
		_data = data._data;
		_dszs = data._dszs;
		return *this;
	}

	/**
	 * @brief Assignement
	 *
	 * @param data To copy
	 */
	NDData& operator= (const codeare::container<T>& data) {
		_dims.resize(1,data.size());
		_dszs.resize(1,1);
		_data = data;
		return *this;
	}

	/**
	 *
	 */
	inline size_t Dim (const size_t n = 0) const { return _dims[n];	}
	inline const std::type_info& Type () { return typeid(T);}
    
	inline const codeare::container<size_t> Dims () const { return _dims; }
	inline const codeare::container<size_t> Dszs () const { return _dszs; }

	inline size_t Size () const { return _data.size(); }
    
	inline size_t NDim() const { return _dims.size(); }
    
	std::ostream& Print (std::ostream& os) const {
		os << "dims(";
		for (size_t i = 0; i < _dims.size(); ++i)
			os << _dims[i] << " ";
		os << ") range(" << minmax(_data) << ")";
		return os;
	}

	inline bool Empty () const { return _data.empty(); }
    
	inline T& operator[] (const size_t p)       { return _data[p]; }
    
	inline T  operator[] (const size_t p) const { return _data[p]; }

	inline T& operator() (const size_t p)       { return _data[p]; }

	inline T  operator() (const size_t p) const { return _data[p]; }

	inline T& operator() (const size_t n0, const size_t n1)
	                                            { return _data[n1*Dim(0)+n0]; }

	inline T  operator() (const size_t n0, const size_t n1) const
	                                            { return _data[n1*Dim(0)+n0]; }

	inline T& operator() (const size_t n0, const size_t n1, const size_t n2)
								{return _data[n2*Dim(0)*Dim(1)+n1*Dim(0)+n0];}

	inline T  operator() (const size_t n0, const size_t n1, const size_t n2) const
								{return _data[n2*Dim(0)*Dim(1)+n1*Dim(0)+n0];}

	inline bool operator== (const NDData<T>& data) {
		return (data._dims == _dims && data._data == _data);
	}

	inline const T* Ptr (const size_t n = 0) const { return _data.ptr()+n; }

	inline T* Ptr (const size_t n = 0) { return &_data[n]; }

	inline const codeare::container<T> Data () const { return _data; }
	inline codeare::container<T>& Data () {return _data;}

	inline NDData<T> operator- () {
		NDData<T> ret (_dims);
		for (size_t i = 0; i < Size(); ++i)
			ret[i] = -ret[i];
		return ret;
	}

	template<class S> inline const NDData<T>&
	operator/= (const S s) {
		for (size_t i = 0; i < Size(); ++i)
			_data[i] /= s;
		return *this;
	}
	template<class S> inline const NDData<T>
	operator/ (const S s) const {
		NDData<T> ret (*this);
		return ret /= s;
	}
	template<class S> inline const NDData<T>&
	operator*= (const S s) {
		for (size_t i = 0; i < Size(); ++i)
			_data[i] *= s;
		return *this;
	}
	template<class S> inline const NDData<T>
	operator* (const S s) const {
		NDData<T> ret (*this);
		return ret *= s;
	}

	template<class S> inline const NDData<T>& // data += s;
	operator+= (const S s) {
		for (size_t i = 0; i < Size(); ++i)
			_data[i] += s;
		return *this;
	}
	template<class S> inline const friend NDData<T> // data + s
	operator+ (const S s, const NDData<T>& data) {
		return data + s;
	}
	template<class S> inline const NDData<T> // s + data
	operator+ (const S s) const {
		NDData<T> ret (*this);
		return ret += s;
	}
	template<class S> inline const NDData<T>&
	operator^= (const S s) const {
		for (size_t i = 0; i < Size(); ++i)
			_data[i] ^= s;
		return *this;
	}
	template<class S> inline const NDData<T>
	operator^ (const S s) const {
		NDData<T> ret (*this);
		return ret ^= s;
	}


};


template <class T> inline static NDData<T>
cumtrapz (const NDData<T>& data,
		codeare::container<T>& times = codeare::container<T>(),
		codeare::container<size_t>& meta = codeare::container<size_t>()) {

	if (!(times.empty()))
		assert (times.size() == data.Dim(0));
	else
		times = codeare::container<T> (data.Dim(0),1.);

	if (!(meta.empty()))
		assert (meta.size() == data.Dim(0));
	else
		meta = codeare::container<size_t> (data.Dim(0),0);

	NDData<T> ret (data.Dims());
	size_t ncol = ret.Size()/ret.Dim(0);
	size_t csz  = ret.Dim(0);

	for (size_t i = 0, os = i*csz; i < ncol; ++i)
		for (size_t j = 1; j < csz; ++j)
            ret[os+j] = ret[os+j-1] + .5 * (data[os+j] + data[os+j-1]) * (times[os+j] - times[os+j-1]);

	return ret;

}


template <class T> inline static bool
isNd (const NDData<T>& data, const size_t n) {
	return data.Dims().size() == n;
}
template <class T> inline static bool
is2d (const NDData<T>& data) {
	return isNd (data, 2);
}
template <class T> inline static bool
isNDData (const NDData<T>& data) {
	return is2d (data);
}
template <class T> inline static bool
is3d (const NDData<T>& data) {
	return isNd (data, 3);
}
template <class T> inline static bool
isVolume (const NDData<T>& data) {
	return is3d (data);
}

template <class T> inline static NDData<T>
transpose (const NDData<T>& data) {
	assert (isNDData(data));
	NDData<T> ret (data.Dim(1),data.Dim(0));
	for (size_t j = 0; j < data.Dim(1); ++j)
		for (size_t i = 0; i < data.Dim(0); i++)
			ret(j,i) = data(i,j);
	return ret;
}

template <class T> inline static size_t
numel               (const NDData<T>& data) {
	return data.Size();
}

template <class T> inline static const NDData<T>
real (const NDData<std::complex<T> >& data) {
	NDData<T> ret (data.Dims());
	for (size_t i = 0; i < ret.Size(); ++i)
		ret[i] = real(data[i]);
	return ret;
}
template <class T> inline static NDData<T>
imag (const NDData<std::complex<T> >& data) {
	NDData<T> ret (data.Dims());
	for (size_t i = 0; i < ret.Size(); ++i)
		ret[i] = real(data[i]);
	return ret;
}
template <class T> inline static NDData<T>
abs (const NDData<std::complex<T> >& data) {
	NDData<T> ret (data.Dims());
	for (size_t i = 0; i < ret.Size(); ++i)
		ret[i] = abs(data[i]);
	return ret;
}
template <class T> inline static NDData<T>
arg (const NDData<std::complex<T> >& data) {
	NDData<T> ret (data.Dims());
	for (size_t i = 0; i < ret.Size(); ++i)
		ret[i] = arg(data[i]);
	return ret;
}
template <class T> inline static NDData< std::complex<T> >
complex (const NDData<T>& re, const NDData<T>& im) {
	size_t rs = re.Size();
	assert (rs == im.Size());
	NDData<std::complex<T> > ret (re.Dims());
	for (size_t i = 0; i < rs; ++i)
		ret[i] = std::complex<T>(re[i],im[i]);
	return ret;
}
template <class T> inline static NDData< std::complex<T> >
complex2 (const NDData<T>& amp, const NDData<T>& pha) {
	size_t rs = amp.Size();
	assert (rs == pha.Size());
	NDData<std::complex<T> > ret (amp.Dims(),1);
	T one = (T)1;
	for (size_t i = 0; i < rs; ++i)
		ret[i] = std::polar (amp[i],pha[i]);
	return ret;
}
template<class T> inline static T
max (const NDData<T>& data) {
	assert (!data.Empty());
	T ret = data[0];
	for (size_t i = 0; i < data.Size(); ++i)
		ret = (ret < data[i]) ? data[i] : ret;
	return ret;
}
template<class T> inline static T
min (const NDData<T>& data) {
	assert (!data.Empty());
	T ret = data[0];
	for (size_t i = 0; i < data.Size(); ++i)
		ret = (ret > data[i]) ? data[i] : ret;
	return ret;
}

template<class T> inline static bool
isempty (const NDData<T>& data) {
	return data.Empty();
}

template <class T> inline std::ostream&
operator<< (std::ostream& os, NDData<T>& di) { return di.Print(os); }

template<class T> inline static NDData<T>
zeros (const codeare::container<size_t>& dims) { return NDData<T>(dims); }

template<class T> inline static NDData<T>
ones (const codeare::container<size_t>& dims) { return NDData<T>(dims,(T)1); }

template<class T> inline static codeare::container<size_t>
size (const NDData<T>& data) { return data.Dims(); }

template <class T> inline static size_t
size (const NDData<T>& data, const size_t& d) { return data.Dim(d); }

template<class T> inline static size_t
sub2ind (const NDData<T>& data, const codeare::container<size_t>& sub) {
	const codeare::container<size_t>& dszs = data.Dszs();
	size_t ind = 0, nd = dszs.size(), lim = std::min(sub.size(),nd);
	for (size_t i = 0; i < lim; ++i)
		ind += sub[i] * dszs[i];
	return ind;
}

template<class T> inline static size_t
sub2ind (const NDData<T>& data, const size_t i0, const size_t i1 = 0,
		const size_t i2 = 0, const size_t i3 = 0) {
	codeare::container<size_t> sub (4);
	sub[0] = i0; sub[1] = i1; sub[2] = i2; sub[3] = i3;
	return sub2ind(data,sub);
}


template <class T> inline static NDData<T>
squeeze (const NDData<T>& data) {
	codeare::container<size_t> dimm = data.Dims(), dimr;
	for (size_t i = 0; i < dimm.size(); ++i)
		if (dimm[i] > 1)
			dimr.push_back(dimm[i]);
	NDData<T> ret (dimr);
	ret.Data() = data.Data();
	return ret;
}

template <class T> inline static NDData<T>
sqrt (const NDData<T>& data) {
	NDData<T> ret(size(data));
	for (size_t i = 0; i < ret.Size(); ++i)
		ret[i] = sqrt(data[i]);
	return ret;
}

template <class T> inline static NDData<T>
atan2 (const NDData<T>& re, const NDData<T>& im) {
	assert (re.Size() == im.Size());
	NDData<T> ret (size(re));
	for (size_t i = 0; i < re.Size(); ++i)
		ret[i] = atan2 (re[i],im[i]);
	return ret;
}

#endif
