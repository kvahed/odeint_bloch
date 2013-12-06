/*
 * Event.hpp
 *
 *  Created on: Dec 4, 2013
 *      Author: kvahed
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include "HDF5File.hpp"
#include "NDData.hpp"

#include <boost/tuple/tuple.hpp>

enum EventType {NONE_E = -1, RF_E, GRADIENT_E};

const static double TWOPI = 6.283185307179586476925286766559005768394338798750211641949889185;

template<class T> class RF;
template<class T> class Gradient;

template<class T>
class Event {

	typedef std::complex<T> CT;

public:
	virtual ~Event() {};
	Event (double start = 0., double end = 0.) : _etype(NONE_E) {
		this->_tpois.push_back(start);
		this->_tpois.push_back(end);
	}

	const std::vector<T>& TPOIs () const {
		return _tpois;
	}

	double Duration () const {
		return _tpois.back()-_tpois.front();
	}

	bool Active (const double t) const {
		return (t >= _tpois.front() && t <= _tpois.back());
	}

protected:

	std::vector<double> _tpois;
	EventType _etype;

};
#endif /* EVENT_HPP_ */
