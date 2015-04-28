#include <string>
#ifndef ERROR_H
#define ERROR_H


class NetworkException : public std::exception
{
	private:
		int error_number;
		std::string error_message;

	public:
		NetworkException(const std::string message, int number=0)
		{
			error_message = message;
			error_number = number;
		} 
		virtual const char* what() const throw()
		{
			return error_message.c_str();
		}
};

#endif