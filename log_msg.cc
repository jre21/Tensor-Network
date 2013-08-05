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
