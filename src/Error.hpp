#include <string>
#include <errno.h>
#include <sstream>

#ifndef ERROR_H
#define ERROR_H

/** \brief Exception representing network errors.
  *
  * Encapsulates the error number returned by the 
  * errno.h.
  */
class NetworkException : public std::exception
{
    private:
        int _errno;
        std::string msg;
        std::string strDetailed;

    public:
        /** errno should be the value after the variable of the system
          * errno after the system call that resulted in an error. As a remainder,
          * the value of errno should be captured immediatly after the system call.
          * A call for printf, for example, will change the current value of errno.
          * If not applicable, it should be left as zero.
          */
        NetworkException(const std::string msg, int _errno):
            _errno(_errno),
            msg(msg)
        {
            std::ostringstream oss;
            if (_errno != 0)
                oss << msg << ":" << strerror(_errno) << " (errno " << _errno << ")";
            else
                oss << msg;
            strDetailed = oss.str();
        }

        virtual const char* what() const throw()
        {
            return strDetailed.c_str();
        }
};

#endif