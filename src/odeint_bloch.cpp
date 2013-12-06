#include "Bloch.hpp"

#include <boost/numeric/odeint.hpp>
#include <iostream>

using namespace boost::numeric::odeint;

void record (const state_type& m, const double t) {
    std::cout << std::setprecision(6) << std::fixed << std::setw(16) << t
         << std::scientific << std::setw(16) << m[0] <<
         std::setw(16) << m[1] << std::setw(16) << m[2] << std::endl;
}

int main (int argc, char **argv) {

	typedef std::complex<double> cdouble;
	typedef boost::tuple<NDData<double>, NDData<cdouble> >  RFData;

	/** RF alternatives **/
	AdiabaticRF<double> rf (0., 10.e-3, 200.0e-6); // Adiabatic hypsec inv 10ms
	//HardRF<double> rf1 (0., 0.0002, std::complex<double>(0.,1.845e-4));   // Hard 90 deg

	/** Simulation world **/
	Spin<double> spin (1., 0., 0., 0., 1., 60.e-3, 0.*TWOPI);
	Bloch<double>::Instance().SetSpin(spin);
	Bloch<double>::Instance().AddEvent(rf);

	/** Integrate IVP **/
	state_type m = { 0., 0., 1. }; // initial magnetisation
	integrate(bloch<double>, m, 0., 5., 1.e-6, record);

	/** Dump pulse **/
	RFData rfd = rf.Dump(1000);
	HDF5File f("out.h5", OUT);
	f.Write (boost::get<0>(rfd), "times");
	f.Write (boost::get<1>(rfd), "data");
	fclose(f);

}
