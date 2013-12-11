#include "Bloch.hpp"
#include "Recorder.hpp"

#include <boost/numeric/odeint.hpp>

using namespace boost::numeric::odeint;

int main (int argc, char **argv) {

	typedef std::complex<double> cdouble;
	typedef boost::tuple<NDData<double>, NDData<cdouble> >  RFData;
	Bloch<double>& Env = Bloch<double>::Instance();
	Recorder<SAVE> recorder;

	/** RF alternatives **/
	AdiabaticRF<double> rf (0., 10.e-3, 200.0e-6); // Adiabatic hypsec inv 10ms
	//HardRF<double> rf1 (0., 200.e-6, std::complex<double>(0.,1.845e-4)); // Hard 90 deg 200us

	/** Simulation world **/
	Spin<double> spin (1., 0., 0., 0., 1., 60.e-3, 0.*TWOPI); // Spin
	Env.SetSpin(spin);
	Env.AddEvent(rf);

	/** Integrate IVP **/
	state_type m = { 0., 0., 1. }; // initial magnetisation
	integrate(bloch<double>, m, 0., 5., 1.e-8, recorder);

	/** Dump pulse **/
	RFData rfd = rf.Dump(1000);
	HDF5File f("rf.h5", OUT);
	f.Write (boost::get<0>(rfd), "times");
	f.Write (boost::get<1>(rfd), "data");
	fclose(f);

}
