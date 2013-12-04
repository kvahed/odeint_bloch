/*
 * RF.h
 *
 *  Created on: Dec 2, 2013
 *      Author: kvahed
 */

#ifndef RF_H_
#define RF_H_

#include <complex>
#include <vector>

const static double TWOPI = 6.283185307179586476925286766559005768394338798750211641949889185;

enum RFType {HARD, ADIABATIC, SINC};

template<class T> class RF {

public:
	virtual ~RF() {};
	RF (double start = 0., double end = 0., T scale = 1.) : _scale(scale) {
		this->_tpois.push_back(start);
		this->_tpois.push_back(end);
	}

	inline const std::complex<T>
	operator() (const double t) const {
		assert (false);
		return 0.;
	}

	const std::vector<T>&
	TPOIs () const {
		return _tpois;
	}

	double
	Duration () const {
		return _tpois.back()-_tpois.front();
	}

	bool
	Active (const double t) const {
		return (t >= _tpois.front() && t <= _tpois.back());
	}

	inline RFType Type () const {
		return _type;
	}

protected:
	std::vector<double> _tpois;
	T _scale;
	RFType _type;

};

#endif /* RF_H_ */
