#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>

#include <postgresql/libpq-fe.h>

#include "postgresql_exception.h"

#define DEFAULT_POSTGRESQL_PORT 5432

#define POSTGRESQL_DEBUG true

#if POSTGRESQL_DEBUG
#include <iostream>
#endif

class Postgresql{
    public:
        typedef std::vector <std::string> Value;
        typedef std::vector <Value> Values;

    private:
        // ポート
        uint16_t port_;
        // ホスト名
        std::string host_name_;
        // ユーザ名
        std::string user_name_;
        // パスワード
        std::string password_;
        // データベース名
        std::string database_;
        
        // デバッグオプション
        const char *debug_option_;
        // デバッグ出力のためのファイルパス、またはtty名
        const char *debug_filepath_;

        // データベース接続情報
        PGconn *pg_conn_;

    public:
        Postgresql (void);
        ~Postgresql (void);
    
    public:
        // ポート番号を設定する
        // 1: const std::string& -> 接続したいデータベースのポート番号
        void setPort (const uint16_t port) noexcept;

        // ホスト名を設定する
        // 1: const std::string& -> 接続したいデータベースのホスト名
        void setHostName (const std::string &host_name) noexcept;
        
        // ユーザ名を設定する
        // 1: const std::string& -> 接続したいデータベースのユーザ名
        void setUserName (const std::string &user_name) noexcept;

        // パスワードを設定する
        // 1: const std::string& -> 接続したいデータベースのパスワード
        void setPassword (const std::string &password) noexcept;

        // データベース名を設定する
        // 1: const std::string& -> 接続したいデータベース名
        void setDatabase (const std::string &database) noexcept;

        // デバッグオプションを設定する
        // 1: const char* -> デバッグオプション
        void setDebugOption (const char *debug_option) noexcept;

        // デバッグ出力のためのファイルパス、またはtty名を設定する
        // 1: const char* -> 記録したいファイルパス
        void setDebugFilepath (const char *debug_filepath) noexcept;

        // データベース接続を行う
        void connect (void);

        // SQL文を送信する
        // 1: const std::string& -> SQL文
        // r: true -> 処理が成功しました
        // r: false -> 処理に失敗しました
        bool exec (const std::string &sql) const;

        // SQL文を送信してクエリを受け取る
        // 1: const std::string& -> SQL文
        // 2: PGresult* -> クエリ
        // r: true -> 処理が成功しました
        // r: false -> 処理に失敗しました
        bool execQuery (const std::string &sql, PGresult **pg_result) const;

        // SQL文を送信してValuesを受け取る
        // 1: const std::string& -> SQL文
        // 2: Values& -> クエリ
        // r: true -> 処理が成功しました
        // r: false -> 処理に失敗しました
        bool execValues (const std::string &sql, Values &values) const;

        // SQL文を送信してValueを受け取る
        // 1: const std::string& -> SQL文
        // 2: Value& -> クエリ
        // r: true -> 処理が成功しました
        // r: false -> 処理に失敗しました
        bool execValue (const std::string &sql, Value &value) const;

        // SQL文を送信してstd::stringを受け取る
        // 1: const std::string& -> SQL文
        // 2: std::string& -> クエリ
        // r: true -> 処理が成功しました
        // r: false -> 処理に失敗しました
        bool execString (const std::string &sql, std::string &response) const;

        // データベース接続を切る
        void disconnect (void) noexcept;

        // 設定した情報を空にする
        void clear (void) noexcept;
    
};