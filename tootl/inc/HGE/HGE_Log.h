#ifndef HGE_LOG_H
#define HGE_LOG_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void hgeAddToLogFile(const char* message);

#if __linux__
#define TERMINAL_COLORCODE_BLUE() "\033[1;34m"
#define TERMINAL_COLORCODE_RED() "\033[1;31m"
#define TERMINAL_COLORCODE_YELLOW() "\033[01;33m"
#define TERMINAL_COLORCODE_GREEN() "\033[1;32m"
#define TERMINAL_COLORCODE_DEFAULT() "\033[0m"
#else
#define TERMINAL_COLORCODE_BLUE() ""
#define TERMINAL_COLORCODE_RED() ""
#define TERMINAL_COLORCODE_YELLOW() ""
#define TERMINAL_COLORCODE_GREEN() ""
#define TERMINAL_COLORCODE_DEFAULT() ""
#endif

#define HGE_LOGGING 1
#if HGE_LOGGING
#define HGE_LOG(str)  ({ \
  char message_buffer[255]; \
  snprintf(message_buffer, sizeof(message_buffer), "[HGE LOG] %s:%d: %s", __FILE__, __LINE__, str); \
  printf("%s%s%s\n", TERMINAL_COLORCODE_BLUE(), message_buffer, TERMINAL_COLORCODE_DEFAULT()); \
  hgeAddToLogFile(message_buffer); \
})
#define HGE_ERROR(str)  ({ \
  char message_buffer[255]; \
  snprintf(message_buffer, sizeof(message_buffer), "[HGE ERROR] %s:%d: %s", __FILE__, __LINE__, str); \
  printf("%s%s%s\n", TERMINAL_COLORCODE_RED(), message_buffer, TERMINAL_COLORCODE_DEFAULT()); \
  hgeAddToLogFile(message_buffer); \
})
#define HGE_WARNING(str)  ({ \
  char message_buffer[255]; \
  snprintf(message_buffer, sizeof(message_buffer), "[HGE WARNING] %s:%d: %s", __FILE__, __LINE__, str); \
  printf("%s%s%s\n", TERMINAL_COLORCODE_YELLOW(), message_buffer, TERMINAL_COLORCODE_DEFAULT()); \
  hgeAddToLogFile(message_buffer); \
})
#define HGE_SUCCESS(str)  ({ \
  char message_buffer[255]; \
  snprintf(message_buffer, sizeof(message_buffer), "[HGE SUCCESS] %s:%d: %s", __FILE__, __LINE__, str); \
  printf("%s%s%s\n", TERMINAL_COLORCODE_GREEN(), message_buffer, TERMINAL_COLORCODE_DEFAULT()); \
  hgeAddToLogFile(message_buffer); \
})
#else
#define HGE_LOG(str)
#define HGE_ERROR(str)
#define HGE_WARNING(str)
#define HGE_SUCCESS(str)
#endif

#define PROFILE_FUNCTION() SET_PRINT_COLOR_BLUE(); printf("Profiling: %s\n", __FUNCTION__); SET_PRINT_COLOR_DEFAULT();

#include <stdlib.h>
#include <string.h>

char* concat(const char *str1, const char *str2);

#endif
