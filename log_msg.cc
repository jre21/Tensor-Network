// Copyright 2013 Jacob Emmert-Aronson
// This file is part of Tensor Network.
//
// Tensor Network is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Tensor Network is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tensor Network.  If not, see
// <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include "log_msg.hh"

using std::endl;

const char* kUnknownFile = "unknown file";
const char* kErrBounds = "argument out of bounds: ";
const char* kErrIncompatible = "incompatible objects: ";
const char* kErrListLength = "list has illegal length: ";

LogMsg::LogMsg(LogSeverity severity, const char* file, int line)
    : severity_(severity) {
  const char* marker =
    severity == LOG_DEBUG ?   "[ DEBUG ]" :
    severity == LOG_INFO ?    "[  INFO ]" :
    severity == LOG_WARNING ? "[WARNING]" :
    severity == LOG_ERROR ?   "[ ERROR ]" : "[ FATAL ]";
  if(file == nullptr) file = kUnknownFile;
  if(line < 0)
    GetStream() << endl << marker << " "
		<< file << ": ";
  else
    GetStream() << endl << marker << " "
		<< file << ":" << line << ": ";
}

// Flushes the buffers and, if severity is LOG_FATAL, aborts the program.
LogMsg::~LogMsg() {
  GetStream() << endl;
  if (severity_ == LOG_FATAL) {
    fflush(stderr);
    abort();
  }
}
