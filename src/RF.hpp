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

enum RFType {NONE_RF = -1, HARD_RF, ADIABATIC_RF, SINC_RF};

template<class T> class RF : public Event<T> {

	typedef std::complex<T> CT;

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

	boost::tuple<NDData<double>, NDData<CT> >Dump (size_t n_samples) const {
		assert (n_samples > 0);
		NDData<double> times (n_samples);
		NDData<CT> data (n_samples);
		double dt = this->Duration()/n_samples;
		for (size_t i = 0; i < n_samples; ++i) {
			double t = dt * i;
			times[i] = t;
			data[i] = (*this)(t);
		}
		return boost::make_tuple(times, data);
	}


protected:
	T _phase_offset;
	std::complex<T> _scale;
	RFType _type;

};

#endif /* RF_H_ */
