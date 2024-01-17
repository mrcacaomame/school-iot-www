#include "DocumentRoot.h"

void DocumentRoot::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    // write your application logic here
    HttpResponsePtr res = HttpResponse::newHttpResponse ();
    printf ("Documents root ...\n");
    
    switch (req->method ()){
        case (Get):
            doGet (req, res);

            break;

        case (Post):
            doPost (req, res);

            break;
    }

    callback (res);
}

void DocumentRoot::doGet(const HttpRequestPtr &req, HttpResponsePtr &res) const{
    
    // CSPファイルの情報を読み取る
    res = HttpResponse::newHttpViewResponse ("DocumentRootView.csp");
}

void DocumentRoot::doPost(const HttpRequestPtr &req, HttpResponsePtr &res) const{

    // CSPファイルの情報を読み取る
    res = HttpResponse::newHttpViewResponse ("DocumentRootView.csp");
}
