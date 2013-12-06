/*
 * HardRF.hpp
 *
 *  Created on: Dec 6, 2013
 *      Author: kvahed
 */

#ifndef HARDRF_HPP_
#define HARDRF_HPP_


template<class T>
class HardRF : public RF<T> {

	typedef std::complex<T> CT;

public:

	HardRF (double start = 0., double end = 0., CT scale = 1.) :
		RF<T> (start, end, scale) {
		this->_type = HARD_RF;
	}

	virtual ~HardRF() {};

	CT operator() (double t) const {
		return (this->Active(t) && this->Duration() > 0.) ?
				this->_scale : CT(0., 0.);
	}

};


#endif /* HARDRF_HPP_ */
