#ifndef NANOQUANT_LOG_H
#define NANOQUANT_LOG_H

#include <iostream>

#define LOG_( ... ) std::cout << __FUNCTION__ << ": " << __LINE__ << "| " << __VA_ARGS__
#define LOG( ... ) std::cout << __FUNCTION__ << ":" << __LINE__ << "| " << __VA_ARGS__ << std::endl
#define LOG_VAR( varname ) LOG( #varname << " := " << varname )


#endif //NANOQUANT_LOG_H
