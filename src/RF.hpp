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

template<class T> class HardRF;
template<class T> class AdiabaticRF;

const static double TWOPI = 6.283185307179586476925286766559005768394338798750211641949889185;

enum RFType {NONE_RF = -1, HARD_RF, ADIABATIC_RF, SINC_RF};

template<class T> class RF : public Event<T> {

public:

	virtual ~RF() {};

	RF (double start = 0., double end = 0., std::complex<T> scale = 1.) :
		Event<T>(start, end), _scale(scale), _type(NONE_RF) {
		this->_etype = RF_E;
	}

	std::complex<T> operator() (double t) const {
		switch (this->Type()) {
			case ADIABATIC_RF: return (*(const AdiabaticRF<T>*)this)(t);
			case HARD_RF:	   return (*(const HardRF<T>*)this)(t);
			default:           return std::complex<T>(0.,0.);
		}
	}

	inline RFType Type () const {
		return _type;
	}

	void Dump () {}

protected:
	T _phase_offset;
	std::complex<T> _scale;
	RFType _type;

};

#endif /* RF_H_ */
