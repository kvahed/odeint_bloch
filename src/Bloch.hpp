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
#include "HardRF.hpp"

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
		for (size_t i = 0; i < _rfs.size(); ++i)
			rft += (*_rfs[i])(t);
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

typedef boost::array<double, 3> state_type;

template<class T> void multiply (const state_type &m,
		codeare::container<T>B, state_type& dm) {

	dm[0] = B[0]*m[0] + B[3]*m[1] + B[6]*m[2];
	dm[1] = B[1]*m[0] + B[4]*m[1] + B[7]*m[2];
	dm[2] = B[2]*m[0] + B[5]*m[1] + B[8]*m[2];

}

template<class T> void bloch (const state_type &m,
		state_type& dm, double t) {

	const Spin<T>& spin = Bloch<T>::Instance().GetSpin();
	const std::complex<T> rf = Bloch<T>::Instance().GetRF(t);

	double bx = GAMMA*real(rf), by = GAMMA*imag(rf),
			bz = 0. + spin.cs(), r1=1./spin.t1(), r2=1./spin.t2();

	codeare::container<T> B (9,0.);
	B[0] = -r2; B[3] =  bz; B[6] = -by;
	B[1] = -bz; B[4] = -r2; B[7] =  bx;
	B[2] =  by; B[5] = -bx; B[8] = -r1;

	//std::cerr << t << " " << bx << " " << by  << " " << bz << std::endl;

	multiply (m, B, dm);
	dm[2] += spin.pd()*r1;

}




#endif /* BLOCH_HPP_ */
