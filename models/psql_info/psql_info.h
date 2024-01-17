#pragma once
#include <string>
#include <libconfig/config.h>

#include "models/define/define.h"

class PsqlInfo{
    private:
        // データベース情報
        std::string database_;
        // ホスト名
        std::string hostname_;
        // ユーザ名
        std::string username_;
        // パスワード
        std::string password_;
        // ポート番号
        std::string port_;

    public:
        PsqlInfo ();
        ~PsqlInfo ();

    public:
        // ホスト名を取得する
        // r: const std::string& -> ホスト名
        const std::string &getHostName (void) const noexcept;

        // ユーザ名
        // r: const std::string& -> ユーザ名
        const std::string &getUserName (void) const noexcept;

        // パスワード
        // r: const std::string& -> パスワード
        const std::string &getPassword (void) const noexcept;

        // データベース
        // r: const std::string& -> データベース
        const std::string &getDatabase (void) const noexcept;

        // ポート番号
        // r: const std::string& -> ポート番号
        const std::string &getPort (void) const noexcept;

};
