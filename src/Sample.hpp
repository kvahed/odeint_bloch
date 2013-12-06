/*
 * Sample.hpp
 *
 *  Created on: Dec 6, 2013
 *      Author: kvahed
 */

#ifndef SAMPLE_HPP_
#define SAMPLE_HPP_

#include "Spin.hpp"

#include <vector>

template<class T> class Sample {

public:

	Sample();

	Sample(size_t n);

	virtual ~Sample();

	inline void PushBack (const Spin<T>& spin) {
		_spins.push_back(spin);
	}

	inline std::vector<Spin<T> > Spins () const {
		return _spins;
	}

protected:

	std::vector<Spin<T> > _spins;

};

#endif /* SAMPLE_HPP_ */
