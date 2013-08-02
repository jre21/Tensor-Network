#ifndef LOG_MSG_HH_
#define LOG_MSG_HH_

#include <iostream>

const char* kUnknownFile = "unknown file";

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
  static const char* FormatFileLocation(const char* file, int line);

 private:
  const LogSeverity severity_;
};


#endif
