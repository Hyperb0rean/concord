#pragma once

#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
        #define __SANITIZE_ADDRESS__
    #endif
#endif