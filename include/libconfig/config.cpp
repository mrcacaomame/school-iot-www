#include "config.h"

enum{
    CONFIG_KEY,
    CONFIG_VALUE,
    CONFIG_SINGLE_QUOTATION,
    CONFIG_DOUBLE_QUOTATION,
    CONFIG_BACKSLASH,
    CONFIG_COMMENT, 

    CONFIG_NONE
};

Config::Config(void){

}

Config::Config(const std::string &file_path){
    this->read (file_path);
}

Config::~Config(void){
    std::map <std::string, std::string> ().swap (this->conf_data);
}

bool Config::read(const std::string &file_path){
    FILE *file;
    bool kv;
    char buf;
    uint8_t status;

    std::string stack;
    std::string key;
    std::string value;

    file = fopen (file_path.c_str (), "r");
    if (!file){
        return false;
    }

    kv = false;
    status = CONFIG_KEY;
    stack = "";
    key = "";
    value = "";

    while ((buf = fgetc (file)) != (char)0xff){
        if (status == CONFIG_COMMENT && buf != '\n'){
            continue;
        }
        switch (buf){
            case ('\''):
                if (status == CONFIG_KEY || status == CONFIG_VALUE){
                    status = CONFIG_SINGLE_QUOTATION;
                }else if (status == CONFIG_BACKSLASH){
                    status = CONFIG_SINGLE_QUOTATION;
                    stack.push_back (buf);
                }else if (status == CONFIG_SINGLE_QUOTATION){
                    status = (!kv ? CONFIG_KEY : CONFIG_VALUE);
                }
                break;
            case ('\"'):
                if (status == CONFIG_KEY || status == CONFIG_VALUE){
                    status = CONFIG_DOUBLE_QUOTATION;
                }else if (status == CONFIG_BACKSLASH){
                    status = CONFIG_DOUBLE_QUOTATION;
                    stack.push_back (buf);
                }else if (status == CONFIG_DOUBLE_QUOTATION){
                    status = (!kv ? CONFIG_KEY : CONFIG_VALUE);
                }
                break;
            case ('='):
                if (status == CONFIG_SINGLE_QUOTATION || status == CONFIG_DOUBLE_QUOTATION){
                    stack.push_back (buf);
                    break;
                }
                key = stack;
                kv = true;
                status = CONFIG_VALUE;
                stack = "";
                break;
            case ('\n'):
                value = stack;
                this->conf_data[key] = value;
                kv = false;
                status = CONFIG_KEY;
                key = value = stack = "";
                break;
            case ('\\'):
                if (status == CONFIG_BACKSLASH){
                    status = (!kv ? CONFIG_KEY : CONFIG_VALUE);
                }
                stack.push_back (buf);
                break;
            case (' '):
                if (status == CONFIG_SINGLE_QUOTATION || status == CONFIG_DOUBLE_QUOTATION){
                    stack.push_back (buf);
                }
                break;
            case ('#'):
                if (status == CONFIG_SINGLE_QUOTATION || status == CONFIG_DOUBLE_QUOTATION){
                    break;
                }
                status = CONFIG_COMMENT;
            default:
                if (status == CONFIG_BACKSLASH){
                    status = (!kv ? CONFIG_KEY : CONFIG_VALUE);
                }
                stack.push_back (buf);
        }
    }
    fclose (file);
    if (!key.empty () && !stack.empty ()){
        this->conf_data[key] = stack;
    }
    return true;
}

void Config::get(const std::string &key, std::string &value) const noexcept{
    value.clear ();

    std::map <std::string, std::string>::const_iterator iter;
    if ((iter = this->conf_data.find (key)) == this->conf_data.end ()){
        return;
    }
    value = iter->second;
}

void Config::getVector(const std::string &key, std::vector<std::string> &values) const noexcept{
    values.clear ();

    std::string line;
    this->get (key, line);
    if (line.empty ()){
        return;
    }

    uint8_t status = CONFIG_NONE;
    std::string stack;

    for (const char &buf : line){
        if (status == CONFIG_NONE){
            if (buf == '['){
                status = CONFIG_VALUE;
            }
            continue;
        }
        if (status == CONFIG_VALUE){
            if (buf == ']'){
                values.emplace_back (stack);
                stack = "";
                break;
            }
        }

        switch (buf){
            case ('\''):
                if (status == CONFIG_VALUE){
                    status = CONFIG_SINGLE_QUOTATION;
                }else if (status == CONFIG_BACKSLASH){
                    status = CONFIG_SINGLE_QUOTATION;
                    stack.push_back (buf);
                }else if (status == CONFIG_SINGLE_QUOTATION){
                    status = CONFIG_VALUE;
                }
                break;
            case ('\"'):
                if (status == CONFIG_VALUE){
                    status = CONFIG_DOUBLE_QUOTATION;
                }else if (status == CONFIG_BACKSLASH){
                    status = CONFIG_DOUBLE_QUOTATION;
                    stack.push_back (buf);
                }else if (status == CONFIG_DOUBLE_QUOTATION){
                    status = CONFIG_VALUE;
                }
                break;
            case (','):
                if (status == CONFIG_SINGLE_QUOTATION || status == CONFIG_DOUBLE_QUOTATION){
                    stack.push_back (buf);
                    break;
                }
                values.emplace_back (stack);
                stack = "";
                break;
            case ('\\'):
                if (status == CONFIG_BACKSLASH){
                    status = CONFIG_VALUE;
                }
                stack.push_back (buf);
                break;
            case (' '):
                if (status == CONFIG_SINGLE_QUOTATION || status == CONFIG_DOUBLE_QUOTATION){
                    stack.push_back (buf);
                }
                break;


            default:
                if (status == CONFIG_BACKSLASH){
                    status = CONFIG_VALUE;
                }
                stack.push_back (buf);
        }
    }
}


void Config::clear(void){
    std::map <std::string, std::string> ().swap (this->conf_data);
}
