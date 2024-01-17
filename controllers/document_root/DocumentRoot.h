#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class DocumentRoot : public drogon::HttpSimpleController<DocumentRoot>{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    // PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
    // ドキュメント情報
    PATH_ADD ("/", Get, Post);    
    PATH_LIST_END
  
  private:
    void doGet (const HttpRequestPtr &req, HttpResponsePtr &res) const;

    void doPost (const HttpRequestPtr &req, HttpResponsePtr &res) const;
    
};
