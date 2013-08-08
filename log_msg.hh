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

#pragma once

#include <iostream>

// strings for messages
extern const char* kUnknownFile;
extern const char* kErrBounds;
extern const char* kErrIncompatible;
extern const char* kErrListLength;

#define LOG_MSG_(severity) \
  LogMsg(LOG_##severity, __FILE__, __LINE__).GetStream()

// prints a debugging message if DEBUG is defined
#ifdef DEBUG
#define LOG_DEBUG_MSG(message) \
  LOG_MSG_(DEBUG) << message
#else
#define LOG_DEBUG_MSG(message)
#endif

enum LogSeverity {
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_FATAL
};

// Formats log entry severity, provides a stream object for streaming the
// log message, and terminates the message with a newline when going out of
// scope.
class LogMsg {
 public:
  LogMsg(LogSeverity severity, const char* file, int line);
  LogMsg(const LogMsg&) = delete;
  LogMsg& operator=(const LogMsg&) = delete;

  // Flushes the buffers and, if severity is LOG_FATAL, aborts the program.
  ~LogMsg();

  ::std::ostream& GetStream() { return ::std::cerr; }

 private:
  const LogSeverity severity_;
};
