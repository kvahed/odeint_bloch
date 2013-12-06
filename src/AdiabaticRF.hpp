/*
 * AdiabaticRF.hpp
 *
 *  Created on: Dec 2, 2013
 *      Author: kvahed
 */

#ifndef ADIABATICRF_HPP_
#define ADIABATICRF_HPP_

#include "RF.hpp"

#include <math.h>
#include <complex>

template<class T> inline static T
sech (const T x) { return (T)1 / (T)cosh(x); }

template<class T>
class AdiabaticRF : public RF<T> {

public:

	AdiabaticRF (double start = 0., double end = 0., T scale = 1.) :
		RF<T> (start, end, scale) {
		this->_type = ADIABATIC_RF;
	}

	virtual ~AdiabaticRF() {};

	std::complex<T> operator() (double t) const {
		if (this->Active(t) && this->Duration() > 0.) {
			double x = pow(TWOPI * ((t-this->_tpois.front())/this->Duration()-.5),2.0);
			return this->_scale*std::polar(sech(x),x*x);
		}
		return std::complex<T>(0.,0.);
	}

};

#endif /* ADIABATICRF_HPP_ */
