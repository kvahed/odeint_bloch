/*
 * Signal.hpp
 *
 *  Created on: Dec 9, 2013
 *      Author: kvahed
 */

#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include "NDData.hpp"

template<class T>
class Signal {

public:

	Signal (size_t pools, size_t length) {
		_signals = NDData<T> (length,pools);
	}

	~Signal () {};

protected:
	NDData<T> _signals;

};

#endif /* SIGNAL_HPP_ */
