#pragma once

#define _AXIS_TO_STR_IMPL(x) #x
#define AXIS_TO_STR(x) _AXIS_TO_STR_IMPL(x)

#define _AXIS_CONCAT_IMPL(x, y) x##y
#define AXIS_CONCAT(x, y) _AXIS_CONCAT_IMPL(x, y)

// Should also think about modular build, where counter is no longer unique
#ifdef __COUNTER__
#define AXIS_ANONYMOUS(name) AXIS_CONCAT(name, __COUNTER__)
#else
#define AXIS_ANONYMOUS(name) AXIS_CONCAT(name, __LINE__)
#endif