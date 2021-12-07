#include <exception>

class ejscppException:public std::exception {

    virtual char const*
    what() const throw() 
    { return ""; }
};
