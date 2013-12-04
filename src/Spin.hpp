/*
 * Spin.hpp
 *
 *  Created on: Dec 2, 2013
 *      Author: kvahed
 */

#ifndef SPIN_HPP_
#define SPIN_HPP_

enum PV {PD,RX,RY,RZ,T1,T2,CS};

template<class T>
class Spin {

public:

	Spin () : _pd(1.), _rx(0.), _ry(0.), _rz(0.), _t1(1.), _t2(.1), _cs(0.) {};

	explicit Spin (const T pd, const T rx, const T ry, const T rz, const T t1, const T t2,
			const T cs) : _pd(pd), _rx(rx), _ry(ry), _rz(rz), _t1(t1), _t2(t2), _cs(cs) {}

	virtual ~Spin() {};

	inline T pd() const {return _pd;}
	inline T rx() const {return _rx;}
	inline T ry() const {return _ry;}
	inline T rz() const {return _rz;}
	inline T t1() const {return _t1;}
	inline T t2() const {return _t2;}
	inline T t3() const {return _cs;}

private:
	T _pd, _rx, _ry, _rz, _t1, _t2, _cs;

};

#endif /* SPIN_HPP_ */
