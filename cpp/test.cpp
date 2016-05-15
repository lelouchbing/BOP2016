#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include <iostream>

using namespace std;

int main() {
	RestClient::Connection* conn = new RestClient::Connection("https://api.projectoxford.ai/academic/v1.0/evaluate?");
	RestClient::HeaderFields headers;
	headers["Content-Type"] = "application/json";
	headers["Ocp-Apim-Subscription-Key"] = "71ec020289c74a5ca9ff1093851bbf3c";
	conn->SetHeaders(headers);
	RestClient::Response r = conn->get("expr=Id=2030985472&model=latest&count=10&offset=0&attributes=Id,F.FId,J.JId,C.CId,AA.AuId,AA.AfId");
	RestClient::disable();

	cout << r.code << endl;
	cout << r.body << endl;
}	
