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
		_stack.push_back(spin);
		_untouched.push_back(_stack.size()-1);
	}

	inline std::vector<Spin<T> > Spins () const {
		return _stack;
	}

	inline Spin& GetNext () {
		size_t n = _untouched.back();
		_untouched.pop_back();
		return _stack[n];
	}

	inline void TurnIn (size_t n) {
		assert (n < _stack.size());
		_done.push_back(n);
	}

	inline void RandomOrder () {

	}

protected:

	std::vector<Spin<T> > _stack; // spins
	std::vector<size_t> _untouched; // processing states
	std::vector<size_t> _done;

};

#endif /* SAMPLE_HPP_ */
