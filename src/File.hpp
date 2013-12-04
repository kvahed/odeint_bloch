#ifndef __FILE_HANDLE_HPP__
#define __FILE_HANDLE_HPP__

#include "NDData.hpp"

#include <fstream>

namespace codeare {
namespace io {

/**
 * Binary io stati
 */
enum IOStatus {
	OK,                          /**< OK                                          */
	EMPTY_FILE_NAME,             /**< Error: Empty file name                      */
	FILE_NOT_FOUND,              /**< Error: FileHandle not found for reading           */
	INSUFFICIENT_PRIVILEGES,     /**< Error: Insufficient privileges or disk full */
	HDF5_FILE_I_EXCEPTION,       /**< Error: HDF5 general                         */
	HDF5_DATASET_I_EXCEPTION,    /**< Error: HDF5 dataset                         */
	HDF5_DATASPACE_I_EXCEPTION,  /**< Error: HDF5 dataspace                       */
	HDF5_DATATYPE_I_EXCEPTION,    /**< Error: HDF5 datatype                        */
	UNMATCHED_DIMENSIONS,
	GENERAL_FAULT,
	LAST_STATUS_ENTRY
};

static std::string StatusMessage[LAST_STATUS_ENTRY]  = {
	std::string("IO notice: OK."),
	std::string("IO error: Empty file name."),
	std::string("IO error: FileHandle not found."),
	std::string("IO error: Insufficient privileges."),
	std::string("IO error: HDF5, general file excption."),
	std::string("IO error: HDF5, dataset exception."),
	std::string("IO error: HDF5, dataspace exception."),
	std::string("IO error: HDF5, datatype exception."),
	std::string("IO error: Dimensions don't match."),
	std::string("IO error: General fault.")
};

	enum DType {RLFL, RLDB, CXFL, CXDB, LONG, SHRT};
	enum IOMode { IN = 0, OUT, APPEND };


    /**
     * brief          Does a FileHandle exist?
     * 
     * @param  fname  FileHandle name
     * @return        Does it exist?
     */
    inline static bool
    fexists (const char* fname) {
    	assert (fname[0] != '\0');
        std::ifstream fs (fname);
        return (fs);
    }

    /**
     * brief          Does a FileHandle exist?
     *
     * @param  fname  FileHandle name
     * @return        Does it exist?
     */
    inline static bool
    fexists (const std::string& fname) {
        return (fexists(fname.c_str()));
    }

	/**
	 * @brief     Base class for FileHandle IO
	 */
	class FileHandle {

	public:


		/**
		 * @brief Construct with FileHandle
		 *
		 * @param  fname    FileHandlename
		 * @param  mode     IO mode (READ/WRITE)
		 * @param  params   Optional parameters
		 * @param  verbose  Verbose output? (default: true)
		 */
		FileHandle (const std::string& fname, const IOMode& mode = IN,
				const bool& verbose = true) :
			_fname(fname),
			_status(OK),
			_fopen(false),
			_verb(verbose),
			_alloc(false),
			_initialised (false),
			_mode (mode) {}

		/**
		 * @brief  Default destructor
		 */
		virtual ~FileHandle () {
			CleanUp();
		}


		/**
		 * @brief Clean up
		 */
		virtual bool
		CleanUp () {
			return OK;
		}


		/**
		 * @brief  FileHandle name
		 *
		 * @return  FileHandle name
		 */
		inline std::string
		FileHandleName() const {
			return _fname;
		}


		/**
		 * @brief  Verbosity
		 *
		 * @return Verbosity
		 */
		inline bool
		Verbosity() const {
			return _verb;
		}


		/**
		 * @brief  Status
		 *
		 * @return Status
		 */
		inline IOStatus
		Status () const {
			return _status;
		}


		/**
		 * @brief  Memory allocated?
		 *
		 * @return Memory allocated?
		 */
		inline bool
		Allocated () const {
			return _alloc;
		}


		/**
		 * @brief  Holding write lock?
		 *
		 * @return Write lock?
		 */
		inline bool
		Locked () const {
			return (_mode == OUT);
		}


		/**
		 * @brief Read a particular data set from FileHandle
		 *
		 * @return  Success
		 */
		template<class T> NDData<T>
		Read (const std::string& uri) const;


		/**
		 * @brief  Write data to FileHandle
		 *
		 * @return  Success
		 */
		template<class T> bool
		Write (const NDData<T>& M, const std::string& uri);


	protected:

		std::string _fname; /**< @brief  FileHandle name */

		IOStatus    _status; /**< @brief Status */

		IOMode      _mode;   /**< @brief IO mode */

		bool        _fopen;  /**< @brief FileHandle open? */
		bool        _verb;   /**< @brief Verbosity */
		bool        _alloc;  /**< @brief Memory allocated? */
		bool        _initialised; /**< @brief Initialised */


		/**
		 * @brief Default constructor
		 */
		FileHandle () :
			_fname(""),	_status(OK), _fopen(false),	_verb(true),
			_alloc(false), _initialised (false), _mode (IN) {}

		/**
		 * @brief Copy constructor
		 *
		 * @param  iob  To copy
		 */
		FileHandle (const FileHandle& iob):
			_fname(iob.FileHandleName()),	_status(iob.Status()), _verb (iob.Verbosity()),
			_alloc (iob.Allocated()), _fopen (false), _initialised (false),
			_mode (IN) {}

	};

} // namespace io
} // namespace codeare

#endif
