#include "HDF5File.hpp"

using namespace codeare::io;

HDF5File::HDF5File (const std::string& fname, const IOMode mode) :
		_status(OK), FileHandle(fname, mode) {
	this->FileAccess();
}

HDF5File::HDF5File () : _status(OK) {}

HDF5File::~HDF5File() {
	this->Close();
}

const IOStatus HDF5File::Close () {
	_file.close();
	return _status;
}


const IOStatus HDF5File::ReportException (const H5::Exception& e, const IOStatus ios) {
	e.printError();
	_status = ios;
	std::cout << StatusMessage[_status] << std::endl;
	return _status;
}

const IOStatus HDF5File::FileAccess () {

	if (_status != OK)
		return _status;

	if (_mode == IN) {
		_file = H5::H5File (_fname, H5F_ACC_RDONLY);
#ifdef VERBOSE
		printf ("\nFile %s opened for RO\n", _fname.c_str());
#endif
	} else if (_mode == OUT) {
		try {
			_file = H5::H5File  (_fname, H5F_ACC_TRUNC);
#ifdef VERBOSE
			printf ("\nFile %s opened for RW\n", _fname.c_str());
#endif
		} catch (const H5::Exception& e) {
			_file = H5::H5File  (_fname, H5F_ACC_RDWR);
#ifdef VERBOSE
			printf ("\nFile %s created for RW\n", _fname.c_str());
#endif
		}
	} else if (_mode == APPEND) {
		try  {
			_file = H5::H5File  (_fname, H5F_ACC_RDWR);
#ifdef VERBOSE
			printf ("\nFile %s created for RW\n", _fname.c_str());
#endif
		} catch (const H5::Exception& e) {}

	}

	return _status;

}


HDF5File& HDF5File::operator= (const HDF5File& h5io) {

	_file   = h5io._file;
	_fname  = h5io._fname;
	_mode   = h5io._mode;
	_status = h5io._status;
	this->FileAccess();
	return *this;

}

H5::Group HDF5File::CreateGroup (const std::string& url) {
    
	size_t depth = 0,
        snip = url.find_first_of(SLASH,    0) ? 0 : 1,
        snap = url.find_first_of(SLASH, snip);
    
	std::string subpath = url.substr(snip, snap);
    
	H5::Group* tmp, group;
    
	while (!subpath.empty()) {
        
		try {
			group = depth ? tmp->openGroup(subpath)   : _file.openGroup(subpath);
		} catch (const H5::Exception& e) {
			group = depth ? tmp->createGroup(subpath) : _file.createGroup(subpath);
		}
        
		snip = ++snap;
		snap = url.find_first_of(SLASH,snip);
		subpath = url.substr(snip,snap);
        
		tmp = &group;
		depth++;
        
	}
    
	return *tmp;
    
}
