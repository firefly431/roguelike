#ifndef ROGUELIKE_INCLUDE_LOG_LOG
#define ROGUELIKE_INCLUDE_LOG_LOG

#include <iostream>

namespace Log {
    inline std::ostream &output() { return std::cerr; }
    inline void do_output() {}
    template<typename T, typename ...R>
    inline void do_output(T &&x, R &&...rest) {
        output() << std::forward<T>(x);
        do_output(std::forward<R>(rest)...);
    }
}

#define LOG_STRINGIFY2(x) #x
#define LOG_STRINGIFY(x) LOG_STRINGIFY2(x)
#define LOG_LOCATION_STRING __FILE__ ":" LOG_STRINGIFY(__LINE__) ": "

#define LOG_DEBUG(...) ::Log::do_output(LOG_LOCATION_STRING "DEBUG: ", __VA_ARGS__)
#define LOG_INFO(...) ::Log::do_output(LOG_LOCATION_STRING "INFO: ", __VA_ARGS__)
#define LOG_WARN(...) ::Log::do_output(LOG_LOCATION_STRING "WARNING: ", __VA_ARGS__)
#define LOG_ERR(...) ::Log::do_output(LOG_LOCATION_STRING "ERROR: ", __VA_ARGS__)

#endif
