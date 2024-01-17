#include "psql_info.h"

PsqlInfo::PsqlInfo(){
    try{
        Config conf;
        conf.read ("/var/www/private/postgresql/conf");

        conf.get (CONF_DB_DATABASE, this->database_);
        conf.get (CONF_DB_HOSTNAME, this->hostname_);
        conf.get (CONF_DB_PASSWORD, this->password_);
        conf.get (CONF_DB_PORT, this->port_);
        conf.get (CONF_DB_USERNAME, this->username_);
    }catch (const ConfigException &e){
        
    }
}

PsqlInfo::~PsqlInfo(){

}

const std::string &PsqlInfo::getHostName(void) const noexcept{
    // TODO: insert return statement here
    return this->hostname_;
}

const std::string &PsqlInfo::getUserName(void) const noexcept{
    // TODO: insert return statement here
    return this->username_;
}

const std::string &PsqlInfo::getPassword(void) const noexcept{
    // TODO: insert return statement here
    return this->password_;
}

const std::string &PsqlInfo::getDatabase(void) const noexcept{
    // TODO: insert return statement here
    return this->database_;
}

const std::string &PsqlInfo::getPort(void) const noexcept{
    // TODO: insert return statement here
    return this->port_;
}
