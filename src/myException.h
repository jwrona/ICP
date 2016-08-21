#include <exception>

struct bad_position: public virtual std::exception
{
    const char* what() const throw()
    {
        return "Position out of desk";
    }
};
