#pragma once
#include <string>
#include <exception>

class ConfigException : public std::exception{
    private:
        std::string msg_;
    public:
        explicit ConfigException (const char *message):msg_(("ConfigException: "+(std::string)message)){}
        explicit ConfigException (const std::string &message):msg_("Configexception: "+message){}
        virtual ~ConfigException (void)noexcept{}
        virtual const char *what () const noexcept {
            return (msg_).c_str ();
        }
};
