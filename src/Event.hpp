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

template<class T>
class Event {

public:
	virtual ~Event() {};
	Event (double start = 0., double end = 0.) {
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


};
#endif /* EVENT_HPP_ */
