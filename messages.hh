#include <cstdio>
#include <cstdlib>

#define MESSAGE_AT_(file, line, message, description) \
  printf("%s:%s: %s %s", file, line, message, description)

#define MESSAGE_(message, description) \
  MESSAGE_AT_(__FILE__, __LINE__, message, description

#define ERR_MSG(message, description) do { \
  MESSAGE(message, description); \
  exit(2); \
  } while(0)

#define WARN_MSG(message, description) \
  MESSAGE_(message, description)

#ifdef DEBUG
#define DBG_MSG(message, description) \
  MESSAGE_(message, description)
#else
#define DBG_MSG(message, description)
#endif
