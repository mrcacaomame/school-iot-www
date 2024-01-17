#include "GetGraphic.h"

void GetGraphic::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    // write your application logic here
    HttpResponsePtr res = HttpResponse::newHttpResponse ();

    switch (req->method ()){
        case (Get):
            doGet (req, res);

            break;

    }

    callback (res);
}

void GetGraphic::doGet(const HttpRequestPtr & req, HttpResponsePtr & res){
    try{
        bool flag;
        PsqlInfo p_info;
        Postgresql psql;
        std::string sql;
        Postgresql::Value value;
        Json::Value json;
        
        psql.setHostName (p_info.getHostName ());
        psql.setPort (std::stoi (p_info.getPort ()));
        psql.setUserName (p_info.getUserName ());
        psql.setPassword (p_info.getPassword ());
        psql.setDatabase (p_info.getDatabase ());

        // データベース接続を行う
        psql.connect ();

        sql = "SELECT info.id, info.hum, info.tem FROM info ORDER BY id DESC LIMIT 1;";

        flag = psql.execValue (sql, value);

        if (!flag || value.empty ()){
            // 取得できませんでした
            printf ("値を取得することができませんでした\n");
            return;
        }

        json["id"] = value[0];
        json["hum"] = value[1];
        json["tem"] = value[2];

        res = HttpResponse::newHttpJsonResponse (json);
    }catch (const PostgresqlException &e){
        fprintf (stderr, "%s\n", e.what ());
    }
}
