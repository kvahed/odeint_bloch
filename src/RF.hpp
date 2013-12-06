/*
 * RF.h
 *
 *  Created on: Dec 2, 2013
 *      Author: kvahed
 */

#ifndef RF_H_
#define RF_H_

#include "Event.hpp"

#include <complex>
#include <vector>

const static double TWOPI = 6.283185307179586476925286766559005768394338798750211641949889185;

enum RFType {NONE, HARD, ADIABATIC, SINC};

template<class T> class RF : public Event<T> {

public:

	virtual ~RF() {};

	RF (double start = 0., double end = 0., std::complex<T> scale = 1.) :
		Event<T>(start, end), _scale(scale), _type(NONE) {
		this->_tpois.push_back(start);
		this->_tpois.push_back(end);
	}

	std::complex<T> operator() (double t) const {
		assert (false);
		return 0.;
	}

	inline RFType Type () const {
		return _type;
	}

protected:
	T _phase_offset;
	std::complex<T> _scale;
	RFType _type;

};

#endif /* RF_H_ */
