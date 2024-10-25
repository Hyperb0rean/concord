

#include "mmap.hpp"

#ifdef LINUX
    #include "platform/posix.hpp"
#elif APPLE
    #include "platform/posix.hpp"
#elif WIN32
    #include "platform/windows.hpp"
#endif

namespace concord::syscall {}