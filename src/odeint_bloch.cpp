#include "Bloch.hpp"

#include <iostream>

#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;

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


void record (const state_type& x, const double t) {
    cout << setprecision(6) << fixed << setw(16) << t
         << scientific << setw(16) << x[0] << setw(16) << x[1] << setw(16) << x[2] << endl;
}

int main (int argc, char **argv) {

	/** RF alternatives **/
	AdiabaticRF<double> rf (0., 10.e-3, 200.0e-6); // Adiabatic hypsec inv 10ms
	//HardRF<double> rf1 (0., 0.0002, std::complex<double>(0.,1.845e-4));   // Hard 90 deg
	//HardRF<double> rf2;   // Hard 90 deg

	/** Simulation world **/
	Spin<double> spin (1., 0., 0., 0., 1., 60.e-3, 0*TWOPI);
	Bloch<double>::Instance().SetSpin(spin);
	Bloch<double>::Instance().AddEvent(rf);
	//Bloch<double>::Instance().AddEvent(rf2);

	/** Integrate IVP **/
	state_type x = { 0., 0., 1. }; // initial magnetisation
	integrate(bloch<double>, x, 0., 5., 1.e-6, record);

}
