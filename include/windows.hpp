#include <stdint.h>
#include <windows.h>

namespace dlvm {

uint32_t get_page_size() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwPageSize;
}
}