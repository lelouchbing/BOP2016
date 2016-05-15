#ifndef _HTTP_UTIL_H_
#define _HTTP_UTIL_H_

#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include <iostream>
#include <vector>
#include <map>
#include "node.h"


using namespace std;

class HttpUtil {
 public:
    string GetIdType(unsigned long long id);
    vector<Node> GetSuccessor(const Node& node);
    HttpUtil();
    bool Match(Node n1, Node n2, int count = 1);
    virtual ~HttpUtil();
 private:
    
    RestClient::Connection* conn; 
    RestClient::Connection* connh; 
    RestClient::HeaderFields* headers;
    map<string, RestClient::Response> cache; 
    RestClient::Response evaluate(const string& type, unsigned long long id, int count = 100);
};

#endif
