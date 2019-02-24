#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace dlvm {

uint32_t get_page_size() { return sysconf(_SC_PAGE_SIZE); }

void *allocate_memory(uint32_t pages) {
  int fd = shm_open("/dev/zero", O_RDWR, S_IRUSR | S_IWUSR);
  void *p = mmap(0, pages * get_page_size(), PROT_READ | PROT_WRITE,
                 MAP_PRIVATE, fd, 0);
  close(fd);

  return p;
}

void deallocate_memory(void *p, uint32_t pages) {
  munmap(p, pages * get_page_size());
}
}  // namespace dlvm