/*
 * Record.hpp
 *
 *  Created on: Dec 11, 2013
 *      Author: kvahed
 */

#ifndef RECORD_HPP_
#define RECORD_HPP_

#include "Bloch.hpp"
#include <boost/numeric/odeint.hpp>

#include <iostream>

enum RMedium {STDOUT, SAVE};

template<RMedium R> class Recorder {

public:
	Recorder () {}; // If file open
	virtual ~Recorder () {}; // If file close
	void operator () (const state_type& m, double t);

protected:
	codeare::container<double> _times;
	codeare::container<state_type> _states;

};

template<> inline void Recorder<STDOUT>::operator() (const state_type& m, double t) {
	std::cout << std::setprecision(6) << std::fixed << std::setw(16) << t
	         << std::scientific << std::setw(16) << m[0] <<
	         std::setw(16) << m[1] << std::setw(16) << m[2] << std::endl;
}

template<> inline void Recorder<SAVE>::operator() (const state_type& m, double t) {
	_states.push_back(m);
	_times.push_back(t);
}

template<> Recorder<SAVE>::~Recorder () {
	size_t n_samples = _states.size();
	if (n_samples == 0)
		return;
	HDF5File f ("simout.h5", OUT);
	NDData<double> times (n_samples);
	NDData<double> data (n_samples,3);
	for (size_t i = 0; i < n_samples; ++i) {
		times [i] = _times[i];
		data (i,0) = _states[i][0];
		data (i,1) = _states[i][1];
		data (i,2) = _states[i][2];
	}
	f.Write (times, "times");
	f.Write (data, "data");
	fclose (f);
}


#endif /* RECORD_HPP_ */
