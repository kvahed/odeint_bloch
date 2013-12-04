/** @file HDF5File.h
 *  @brief Implementation of JEMRIS Sample
 */

/*
 *  JEMRIS Copyright (C) 2007-2010  Tony Stoecker, Kaveh Vahedipour
 *                                  Forschungszentrum Juelich, Germany
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef HDF5_FILE_H_
#define HDF5_FILE_H_

//#define VERBOSE

#include "NDData.hpp"
#include "File.hpp"

#include <H5Cpp.h>
#include <algorithm>
#include <string.h>

namespace codeare {
namespace io {

template <class T> struct H5Traits;
template<> struct H5Traits<double> {
	static const bool Complex = false;
	static const H5::PredType H5Type () { return H5::PredType::NATIVE_DOUBLE; }
};
template<> struct H5Traits<std::complex<double> > {
	static const bool Complex = true;
	static const H5::PredType H5Type () { return H5::PredType::NATIVE_DOUBLE; }
};
template<> struct H5Traits<float> {
	static const bool Complex = false;
	static const H5::PredType H5Type () { return H5::PredType::NATIVE_FLOAT; }
};
template<> struct H5Traits<std::complex<float> > {
	static const bool Complex = true;
	static const H5::PredType H5Type () { return H5::PredType::NATIVE_FLOAT; }
};

const static std::string SLASH = "/";
const static std::string DSLASH = "//";

inline static std::string
URI (const std::string& URL, const std::string& URN) {
    std::string uri (SLASH + URL + SLASH + URN);
    size_t pos = uri.find(DSLASH);
    while (pos != std::string::npos) {
        uri.replace (pos, 2, SLASH);
        pos = uri.find(DSLASH);
    }
    return uri;
}


/**
 * @brief HDF5 IO interface
 */
class HDF5File : public FileHandle {

public:
	
	/**
	 * @brief Contructor
	 *
	 * @brief Filename
	 * @brief Mode (IN, OUT)
	 */
	HDF5File     (const std::string& fname, const IOMode mode = IN);
	
	HDF5File ();

	/**
	 * @brief Destructor
	 */
	virtual ~HDF5File ();

	virtual const IOStatus Close ();

	/**
	 * @brief     Write data from container to file
	 *
	 * @param  dc Data container
	 */
	template<class T> IOStatus
	Write (const NDData<T>& data, const std::string& urn, const std::string& url = "/") {

		try {

#ifndef VERBOSE
			H5::Exception::dontPrint();
#endif

			H5::Group group;
			bool Complex = H5Traits<T>::Complex;

			codeare::container<hsize_t> dims ((codeare::container<hsize_t>) data.Dims());
			if (Complex)
				dims.insert(dims.begin(), 2);
			std::reverse (dims.begin(),dims.end());

			try {
				group = this->_file.openGroup(url);
#ifdef VERBOSE
				printf ("Group %s opened for writing\n", url.c_str()) ;
#endif
			} catch (const H5::Exception& e) {
				group = this->CreateGroup (url);
			}

			H5::DataSpace dspace (data.NDim() + ((Complex) ? 1 : 0), dims.ptr());
			H5::FloatType dtype  (H5Traits<T>::H5Type());
			H5::DataSet   dset = group.createDataSet(urn, dtype, dspace);

			dset.write(data.Ptr(), dtype);
			dset.close();
			dspace.close();
			group.close();

		} catch (const H5::FileIException&      e) {
			return ReportException (e, HDF5_FILE_I_EXCEPTION);
		} catch (const H5::GroupIException&     e) {
			return ReportException (e, HDF5_FILE_I_EXCEPTION);
		} catch (const H5::DataSetIException&   e) {
			return ReportException (e, HDF5_DATASET_I_EXCEPTION);
		} catch (const H5::DataSpaceIException& e) {
			return ReportException (e, HDF5_DATASPACE_I_EXCEPTION);
		} catch (const H5::DataTypeIException&  e) {
			return ReportException (e, HDF5_DATATYPE_I_EXCEPTION);
		}

		return OK;

	}
	template<class T> IOStatus
	Write (const NDData<T>& data, const char* urn, const char* url = "/") {
		return Write(data, std::string(urn), std::string(url));
	}


	template<class T> IOStatus
	Read (NDData<T>& data, const std::string& urn, const std::string& url = "/") {

		try {

#ifndef VERBOSE
			H5::Exception::dontPrint();
#endif
			H5::DataSet   dset   = this->_file.openDataSet(URI(url,urn));
			H5::FloatType dtype  (H5Traits<T>::H5Type());
			H5::DataSpace dspace = dset.getSpace();
			bool Complex = H5Traits<T>::Complex;
			size_t ndims = dspace.getSimpleExtentNdims();

			codeare::container<hsize_t> dims (ndims);
			size_t ndim  = dspace.getSimpleExtentDims(&dims[0], NULL);
			if (Complex)
				dims.pop_back();
			std::reverse (dims.begin(),dims.end());
			data         = NDData<T> ((codeare::container<size_t>)dims);

			dset.read(data.Ptr(), dtype);
			dspace.close();
			dset.close();

		} catch (const H5::FileIException&      e) {
			return ReportException (e, HDF5_FILE_I_EXCEPTION);
		} catch (const H5::DataSetIException&   e) {
			return ReportException (e, HDF5_DATASET_I_EXCEPTION);
		} catch (const H5::DataSpaceIException& e) {
			return ReportException (e, HDF5_DATASPACE_I_EXCEPTION);
		} catch (const H5::DataTypeIException&  e) {
			return ReportException (e, HDF5_DATATYPE_I_EXCEPTION);
		}

		return OK;

	}
	template<class T> IOStatus
	Read (NDData<T>& data, const char* urn, const char* url = "/") {
		return Read(data, std::string(urn), std::string(url));
	}
    template<class T> NDData<T>
    Read (const std::string& urn, const std::string& url = "/") {
        NDData<T> ret;
        Read (ret, urn, url);
        return ret;
    }
    template<class T> NDData<T>
    Read (const char* urn, const char* url = "/") {
        return Read<T> (std::string(urn), std::string(url));
    }

	const IOStatus
	FileAccess    ();

	HDF5File& operator= (const HDF5File& h5io) ;


private:

	H5::Group CreateGroup (const std::string& url);


	/**
	 * 	@brief Handle HDF5 exceptions
	 *
	 * 	@param e     Exception
	 * 	@param ios   Status
	 */
	const IOStatus ReportException (const H5::Exception& e, const IOStatus ios);

	H5::H5File _file;
	IOStatus _status;

};

}}

using namespace codeare::io;

static HDF5File
fopen (std::string fname, IOMode mode = IN) {
    return HDF5File (fname, mode);
}
#define fwrite(X,Y) _fwrite (X,Y,#Y)
template<class T> static IOStatus
_fwrite (HDF5File& h5io, const NDData<T>& data, const char* dname) {
    return h5io.Write (data, dname, "/");
}
template<class T> static IOStatus
fread (HDF5File& h5io, NDData<T>& data, const char* dname) {
    return h5io.Read (data, dname, "/");
}
template<class T> static NDData<T> 
fread (HDF5File& h5io, const char* dname) {
    return h5io.Read<T> (dname, "/");
}
static IOStatus fclose (HDF5File& h5io) {
    return h5io.Close();
}

#endif
