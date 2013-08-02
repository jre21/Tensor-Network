#ifndef MESSAGES_HH_
#define MESSAGES_HH_

#include <cstdio>
#include <cstdlib>

// strings for messages
extern const char *kErrListLength;
extern const char *kErrBounds;

#define MESSAGE_AT_(file, line, message, description) \
  printf("%s:%d: %s: %s\n", file, line, message, description)

#define MESSAGE_(message, description) \
  MESSAGE_AT_(__FILE__, __LINE__, message, description)

#define ERROR(message, description) do { \
  MESSAGE_(message, description); \
  exit(-1); \
  } while(0)

#define WARNING(message, description) \
  MESSAGE_(message, description)

#ifdef DEBUG
#define DEBUG_MSG(message, description) \
  MESSAGE_(message, description)
#else
#define DEBUG_MSG(message, description)
#endif

#endif // MESSAGES_HH_
