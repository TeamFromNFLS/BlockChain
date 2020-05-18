#ifndef LOG_H
#define LOG_H
#define LOGOUT             \
    _time_ = std::time(0); \
    _s = ctime(&_time_);   \
    cout << dec << _s.substr(11, 8) << ' '
#include <string>
#include <ctime>

extern time_t _time_;
extern std::string _s;
#endif // LOG_H