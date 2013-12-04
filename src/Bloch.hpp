/*
 * Bloch.hpp
 *
 *  Created on: Dec 2, 2013
 *      Author: kvahed
 */

#ifndef BLOCH_HPP_
#define BLOCH_HPP_

#include "Spin.hpp"
#include "NDData.hpp"
#include "AdiabaticRF.hpp"

#include <boost/array.hpp>

const double GAMMA = 4.2577480e7;


template<class T> class Bloch {

public:

	Bloch (const Spin<T>& spin) : _spin(spin) {}

	inline bool AddEvent (const RF<T>& rf) {
		_rfs.push_back(&rf);
		return true;
	}

	inline std::complex<T> GetRF (const double t) const {
		std::complex<T> rft (0.,0.);
		for (size_t i = 0; i < _rfs.size(); ++i) {
			if (_rfs[i]->Type() == ADIABATIC)
				rft += (*(const AdiabaticRF<T>*)_rfs[i])(t);
		}
		return rft;
	}

	static Bloch& Instance() {
		if (!_instance)
			_instance = new Bloch<T>();
		return *_instance;
	}

	inline void SetSpin (const Spin<T>& spin) {
		_spin = spin;
	}

	inline const Spin<T>& GetSpin () const {
		return _spin;
	}
protected:

	Spin<T> _spin;
	std::vector<const RF<T>*> _rfs;
	static Bloch<T>* _instance;

	Bloch(){};

};

template<> Bloch<double>* Bloch<double>::_instance = 0;
template<> Bloch<float>* Bloch<float>::_instance = 0;



#endif /* BLOCH_HPP_ */
