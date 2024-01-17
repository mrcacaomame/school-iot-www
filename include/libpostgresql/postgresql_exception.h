#pragma once
#include <string>
#include <exception>

class PostgresqlException : public std::exception{
    private:
        std::string msg;

    public:
        PostgresqlException (const std::string &msg):msg (msg){}
        const char *what (void) const noexcept override{
            return this->msg.c_str ();
        }
};
