#include <stdlib.h>
#include <string>
#include <sstream>
#include "log_msg.hh"

using std::endl;
using std::string;
using std::stringstream;

extern const char* kUnknownFile;

LogMsg::LogMsg(LogSeverity severity, const char* file, int line)
    : severity_(severity) {
  const char* const marker =
    severity == LOG_DEBUG ?     "[ DEBUG ]" :
    severity == LOG_INFO ?    "[  INFO ]" :
    severity == LOG_WARNING ? "[WARNING]" :
    severity == LOG_ERROR ?   "[ ERROR ]" : "[ FATAL ]";
  GetStream() << endl << marker << " "
              << FormatFileLocation(file, line) << ": ";
}

// Flushes the buffers and, if severity is LOG_FATAL, aborts the program.
LogMsg::~LogMsg() {
  GetStream() << endl;
  if (severity_ == LOG_FATAL) {
    fflush(stderr);
    abort();
  }
}

const char* FormatFileLocation(const char* file, int line) {
  string file_name(file == NULL ? kUnknownFile : file);

  if (line < 0) {
    return (file_name + ":").c_str();
  }
  stringstream ss;
  string s;
  ss << line;
  ss >> s;
  return (file_name + ":" + s + ":").c_str();
}
