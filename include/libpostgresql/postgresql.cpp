#include "postgresql.h"

Postgresql::Postgresql(void){
    this->pg_conn_          = nullptr;
    this->debug_option_     = nullptr;
    this->debug_filepath_   = nullptr;
    this->port_             = DEFAULT_POSTGRESQL_PORT;
}

Postgresql::~Postgresql(void){
    Postgresql::clear ();
}

void Postgresql::setPort(const uint16_t port) noexcept{
    this->port_ = port;
}

void Postgresql::setHostName(const std::string &host_name) noexcept{
    this->host_name_ = host_name;
}

void Postgresql::setUserName(const std::string &user_name) noexcept{
    this->user_name_ = user_name;
}

void Postgresql::setPassword(const std::string &password) noexcept{
    this->password_ = password;
}

void Postgresql::setDatabase(const std::string &database) noexcept{
    this->database_ = database;
}

void Postgresql::setDebugOption(const char *debug_option) noexcept{
    this->debug_option_ = debug_option;
}

void Postgresql::setDebugFilepath(const char *debug_filepath) noexcept{
    this->debug_filepath_ = debug_filepath;
}

void Postgresql::connect(void){
    ConnStatusType res;

    // データベースへの接続を試みる
    this->pg_conn_ = PQsetdbLogin (
        // ホスト名
        this->host_name_.c_str (),
        // ポート番号
        std::to_string (this->port_).c_str (),
        // デバッグオプション
        this->debug_option_,
        // デバッグ出力のためのファイル、またはtty名
        this->debug_filepath_,
        // データベース名
        this->database_.c_str (),
        // ユーザ名
        this->user_name_.c_str (),
        // パスワード
        this->password_.c_str ()
    );

    // 接続が成功したか調べる
    res = PQstatus (this->pg_conn_);
    if (res == CONNECTION_BAD){
        // 接続に失敗しました
        std::string msg = std::string("void Postgresql::connect(void);\n>> ")
                        + std::string (PQerrorMessage (this->pg_conn_));

        throw PostgresqlException (msg);
    }
}

bool Postgresql::exec(const std::string &sql) const{
    bool ret;
    ExecStatusType res;
    PGresult *pg_result;

    if (!this->pg_conn_){
        // データベースへの接続処理を行っていません
        std::string msg = std::string ("bool Postgresql::exec(const std::string &sql) const;\n>> ")
                        + std::string ("Failed to connected the database.");

        throw PostgresqlException (msg);
    }

    // SQL を実行させる
    pg_result = PQexec (this->pg_conn_, sql.c_str ());
    
    res = PQresultStatus (pg_result);
    if (res != PGRES_COMMAND_OK){
        ret = false;
    }else{
        ret = true;
    }

    PQclear (pg_result);
    return ret;
}

bool Postgresql::execQuery(const std::string &sql, PGresult **pg_result) const{
    bool ret;
    ExecStatusType res;

    if (!this->pg_conn_){
        // データベースへの接続処理を行っていません
        std::string msg = std::string ("bool Postgresql::exec(const std::string &sql) const;\n>> ")
                        + std::string ("Failed to connected the database.");

        throw PostgresqlException (msg);
    }

    // SQL を実行させる
    *pg_result = PQexec (this->pg_conn_, sql.c_str ());
    
    res = PQresultStatus (*pg_result);
    if (res != PGRES_TUPLES_OK){
        ret = false;
    }else{
        ret = true;
    }

    return ret;
}

bool Postgresql::execValues(const std::string &sql, Postgresql::Values &values) const{
    bool ret;
    int32_t rows;
    int32_t cols;
    PGresult *pg_result;

    pg_result = nullptr;

    try{
        ret = Postgresql::execQuery (sql, &pg_result);

        if (ret){
            rows = PQntuples (pg_result);
            cols = PQnfields (pg_result);
            
            values.resize (rows);
            for (int32_t j = 0; j < rows; j ++){
                values.resize (cols);
                for (int32_t k = 0; k < cols; k ++){
                    values[j][k] = PQgetvalue (pg_result, j, k);
                }
            }
        }
    }catch (const PostgresqlException &e){
        if (pg_result){
            PQclear (pg_result);
            pg_result = nullptr;
        }
        throw e;
    }

    return ret;
}

bool Postgresql::execValue(const std::string &sql, Value &value) const{
    bool ret;
    int32_t rows;
    int32_t cols;
    PGresult *pg_result;

    pg_result = nullptr;

    try{
        ret = Postgresql::execQuery (sql, &pg_result);

        if (ret){
            rows = PQntuples (pg_result);
            cols = PQnfields (pg_result);
            
            value.resize (cols);
            for (int32_t j = 0; j < cols; j ++){
                value[j] = PQgetvalue (pg_result, 0x0, j);
            }
        }
    }catch (const PostgresqlException &e){
        if (pg_result){
            PQclear (pg_result);
            pg_result = nullptr;
        }
        throw e;
    }

    return ret;
}

bool Postgresql::execString(const std::string &sql, std::string &response) const{
    bool ret;
    int32_t rows;
    int32_t cols;
    PGresult *pg_result;

    pg_result = nullptr;

    try{
        #if POSTGRESQL_DEBUG
        std::cout << sql << std::endl;
        #endif
        
        ret = Postgresql::execQuery (sql, &pg_result);

        if (ret){
            rows = PQntuples (pg_result);
            cols = PQnfields (pg_result);
            
            response = PQgetvalue (pg_result, 0x0, 0x0);
        }
    }catch (const PostgresqlException &e){
        if (pg_result){
            PQclear (pg_result);
            pg_result = nullptr;
        }
        throw e;
    }

    return ret;
}

void Postgresql::disconnect(void) noexcept{
    if (this->pg_conn_){
        PQfinish (this->pg_conn_);
        this->pg_conn_ = nullptr;
    }
}

void Postgresql::clear(void) noexcept{
    this->port_ = DEFAULT_POSTGRESQL_PORT;
    this->host_name_.clear ();
    this->user_name_.clear ();
    this->password_.clear ();
    this->database_.clear ();

    Postgresql::disconnect ();
}
