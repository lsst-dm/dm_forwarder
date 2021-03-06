/*
 * This file is part of dm_forwarder
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <core/Exceptions.h>
#include <core/SimpleLogger.h>
#include <forwarder/Formatter.h>

namespace fs = boost::filesystem;

FitsOpener::FitsOpener(const fs::path& filepath, int mode) : _status(0) {
    bool file_exist;
    try {
        file_exist = exists(filepath);
    }
    catch (boost::filesystem::filesystem_error& e) {
        std::ostringstream err;
        err << "Error in checking file existence for " << filepath.string()
            << " because " << e.code().message();
        LOG_CRT << err.str();
        throw L1::CfitsioError(err.str());
    }

    if (mode == FILE_MODE::WRITE_ONLY && !file_exist) {
        fits_create_file(&_fptr, filepath.c_str(), &_status);
    }
    else if (mode == READWRITE || mode == READONLY && file_exist) {
        fits_open_file(&_fptr, filepath.c_str(), mode, &_status);
    }
    else if (mode == FILE_MODE::WRITE_ONLY && file_exist){
        // Removing is safe for synchronous application. If there is
        // asynchronous access to file. remove should be carefully
        // handled.
        std::ostringstream wrn;
        wrn << "Going to overwrite existing fitsfile " << filepath.string();
        LOG_WRN << wrn.str();
        remove(filepath.c_str());
        fits_create_file(&_fptr, filepath.c_str(), &_status);
    }
    else if (mode == READWRITE || mode == READONLY && !file_exist) {
        std::ostringstream err;
        err << "File at " << filepath.string() << " does not exist to read.";
        LOG_CRT << err.str();
        throw L1::CfitsioError(err.str());
    }

    if (_status) {
        char err[FLEN_ERRMSG];
        fits_read_errmsg(err);
        LOG_CRT << err;
        throw L1::CfitsioError(std::string(err));
    }
}

FitsOpener::~FitsOpener() {
    fits_close_file(_fptr, &_status);
}

fitsfile* FitsOpener::get() {
    return _fptr;
}

int FitsOpener::num_hdus() {
    int num_hdus = -1;
    fits_get_num_hdus(_fptr, &num_hdus, &_status);
    return num_hdus;
}
