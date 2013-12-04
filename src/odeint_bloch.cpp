#include "Bloch.hpp"

#include <iostream>

#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;

typedef boost::array<double, 3> state_type;

template<class T> void
multiply (const state_type &m,  codeare::container<T>B, state_type& dm) {

	dm[0] = B[0]*m[0] + B[3]*m[1] + B[6]*m[2];
	dm[1] = B[1]*m[0] + B[4]*m[1] + B[7]*m[2];
	dm[2] = B[2]*m[0] + B[5]*m[1] + B[8]*m[2];

}

template<class T>
void bloch (const state_type &m, state_type& dm, double t) {

	const Spin<T>& spin = Bloch<T>::Instance().GetSpin();
	const std::complex<T> rf = Bloch<T>::Instance().GetRF(t);

	double bx = GAMMA*real(rf), by = GAMMA*imag(rf),
			bz, r1=1./spin.t1(), r2=1./spin.t2();

	codeare::container<T> B (9,0.);
	B[0] = -r2; B[3] =  bz; B[6] = -by;
	B[1] = -bz; B[4] = -r2; B[7] =  bx;
	B[2] =  by; B[5] = -bx; B[8] = -r1;

	multiply (m, B, dm);
	dm[2] += spin.pd()*r1;

}


void record (const state_type& x, const double t) {
    cout << setprecision(6) << fixed << setw(16) << t
         << scientific << setw(16) << x[0] << setw(16) << x[1] << setw(16) << x[2] << endl;
}

int main(int argc, char **argv) {

	Spin<double> spin (1., 0., 0., 0., 1., .1, 0.);
	state_type x = { 0., 0., 1. }; // initial magnetisation
	// Keep the RF in a variable during integration, otherwise its destructor will be called
	AdiabaticRF<double> rf (0., 10e-3, 2.e-3);
	Bloch<double>::Instance().SetSpin(spin);
	Bloch<double>::Instance().AddEvent((RF<double>)rf);

	integrate(bloch<double>, x, 0., 5., 0.1e-3, record);

}