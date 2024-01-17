#pragma once
#include <stdint.h>
#include <map>
#include <vector>
#include <string>

#include "config_exception.h"

class Config{
    private:
        std::map <std::string, std::string> conf_data;
    public:
        Config (void);
        Config (const std::string &file_path);
        ~Config (void);
    public:
        bool read (const std::string &file_path);
        void get (const std::string &key, std::string &value) const noexcept;
        void getVector (const std::string &key, std::vector <std::string> &values) const noexcept;
        void clear (void);
};