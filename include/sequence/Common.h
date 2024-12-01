#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

#include <string>

using namespace std;

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif

//Исключения

class IndexOutOfRange : public std::exception {
    std::string message_;

public:
    explicit IndexOutOfRange(const std::string &message) : message_(message) {}

    explicit IndexOutOfRange(const char *message) : message_(message) {}

    const char *what() const noexcept override {
        return message_.c_str();
    }
};

#endif
