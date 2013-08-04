#ifndef LOG_MSG_HH_
#define LOG_MSG_HH_

#include <iostream>

// strings for messages
extern const char* kUnknownFile;
extern const char* kErrBounds;
extern const char* kErrIncompatible;
extern const char* kErrListLength;

#define LOG_MSG_(severity) \
  LogMsg(LOG_##severity, __FILE__, __LINE__).GetStream()

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

#endif // LOG_MSG_HH_
