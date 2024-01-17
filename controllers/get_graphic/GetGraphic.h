#pragma once

#include <drogon/HttpSimpleController.h>

#include <libpostgresql/postgresql.h>
#include <libconfig/config.h>

#include "models/psql_info/psql_info.h"

using namespace drogon;

class GetGraphic : public drogon::HttpSimpleController<GetGraphic>{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    // PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
    PATH_ADD ("/get-graphic", Get);
    PATH_LIST_END
  
  private:
    void doGet (const HttpRequestPtr &req, HttpResponsePtr &res);

};
