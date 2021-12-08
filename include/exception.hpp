#include <exception>

class ejscppException:public std::exception {

public:
        
    explicit ejscppException(int line,int column,std::string_view _msg)
        :line{line},column{column}
    {
        msg = std::string(_msg) + " at line: "+ std::to_string(line)
            + " column: " + std::to_string(column) ;
    }

    virtual char const*
    what() const throw() 
    { 
        return msg.c_str();
    }
private:
    std::string msg;
    int line,column;
};
