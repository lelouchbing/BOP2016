#include "http_util.h"
#include <stdio.h>
#include <unistd.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace RestClient;

const char* query_temp = "expr=%s=%lld&model=latest&count=%d&offset=0&attributes=Id,F.FId,J.JId,C.CId,AA.AuId,AA.AfId,RId&subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6";
const char* query_temp_com = "expr=Composite(%s=%lld)&model=latest&count=%d&offset=0&attributes=Id,F.FId,J.JId,C.CId,AA.AuId,AA.AfId,RId&subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6";

const char* match_temp = "expr=And(%s,%s)&model=latest&count=%d&offset=0&attributes=Id,F.FId,J.JId,C.CId,AA.AuId,AA.AfId,RId&subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6";
HttpUtil::HttpUtil() {
    RestClient::init();
    conn = new Connection("oxfordhk.azure-api.net/academic/v1.0/evaluate?");
    connh = new Connection("oxfordhk.azure-api.net/academic/v1.0/calchistogram?");
    headers = new HeaderFields();
	(*headers)["Content-Type"] = "application/json";
	conn->SetHeaders(*headers);
}

HttpUtil::~HttpUtil() {
    delete conn;
    delete headers;
    RestClient::disable();
}

bool HttpUtil::Match(Node node1, Node node2, int count) {
    char query1[50];
    char query2[50];
    if (node1.type.find('.') != string::npos) {
        sprintf(query1, "Composite(%s=%lld)", node1.type.c_str(), node1.id);
    } else {
        sprintf(query1, "%s=%lld", node1.type.c_str(), node1.id);
    }
    if (node2.type.find('.') != string::npos) {
        sprintf(query2, "Composite(%s=%lld)", node2.type.c_str(), node2.id);
    } else {
        sprintf(query2, "%s=%lld", node2.type.c_str(), node2.id);
    }
    char query[200];
    sprintf(query, match_temp, query1, query2, count);
    Response r = connh->get(string(query));
    Document d;
    
    if (d.Parse(r.body.c_str()).HasParseError()) {
        cout << "error: invalid json " + r.body << endl;
        return false;
    }
   

    if (r.code == 200) {
        if (!d.HasMember("entities"))
            return false;
        if (d["entities"].Capacity() > 0 && d["entities"][0].HasMember("AA")) {
            return true;
        } else {
            return false;
        }
    } else {
        cout << r.body <<endl;
        return false;
    }
}

Response HttpUtil::evaluate(const string& type, unsigned long long id, int count) {
    char query[2048];
    if (type.find('.') != string::npos) { 
        sprintf(query, query_temp_com, type.c_str(), id, count);
    } else {
        sprintf(query, query_temp, type.c_str(), id, count);
    }
    Node temp(id, type);
    Response r;
    if (cache.find(temp.tostring()) != cache.end()) {
        r = cache[temp.tostring()];
    } else {
        r = conn->get(string(query));
        cache[temp.tostring()] = r;
    }
    if (r.code != 200) {
            cout << string(query) <<endl;
            cout << r.code <<endl;
            cout << r.body <<endl;
    }
    return r; 
}


string HttpUtil::GetIdType(unsigned long long id) {
    Response r = evaluate("AA.AuId", id); 
    Document d;
    string ret;
    if (d.Parse(r.body.c_str()).HasParseError()) {
        cout << "error: invalid json " + r.body << endl;
        return ret;
    }

    if (r.code == 200) {
        if (!d.HasMember("entities"))
                return "Id";
        if (d["entities"].Capacity() > 0 && d["entities"][0].HasMember("AA")) {
            return "AA.AuId";
        } else {
            return "Id";
        }
    } else {
        cout << "Invalid id:" << id <<endl;
        return ret;
    }
}

vector<Node> GetSuc(Response r, string type) {
    vector<Node> ids;
    Document d;
    if (d.Parse(r.body.c_str()).HasParseError()) {
        cout << "error: invalid json " + r.body << endl;
        return ids;
    }
    if (!d.HasMember("entities")) {
        cout << "error: no entity" << endl;
        return ids;
    }

    Value& entities = d["entities"];
    for (int i = 0; i < entities.Size(); ++i) {

        Value& id = entities[i]["Id"];
        if (type == "F.FId") {
            ids.push_back(Node(id.GetUint(),"Id"));
        }
        if (type == "C.CId") {
            ids.push_back(Node(id.GetUint(),"Id"));
        }
        if (type == "J.JId") {
            ids.push_back(Node(id.GetUint(),"Id")); 
        }
        if (type == "AA.AuId") {
            ids.push_back(Node(id.GetUint(),"Id"));
        }
        
        if (entities[i].HasMember("J")) {
            Value& jo = entities[i]["J"];
            Value& jid = jo["JId"];
            if (type == "Id") {
                ids.push_back(Node(jid.GetUint(),"J.JId"));
            }
        }

        if (entities[i].HasMember("RId")) {
            Value& rid = entities[i]["RId"];
        
            for (int j = 0; j < rid.Capacity(); ++j) {
                if (type == "Id") {
                   ids.push_back(Node(rid[j].GetUint(), "Id"));
                }
            }
        } 

        if (entities[i].HasMember("AA")) {
            Value& au = entities[i]["AA"];
            for (int j = 0; j < au.Capacity(); ++j) {
                if (au[j].HasMember("Auid")) {
                    Value& auid = au[j]["AuId"];
                    if (type == "AA.AfId") {
                     ids.push_back(Node(auid.GetUint(),"AA.AuId"));
                    }
                    if (type == "Id") {
                        ids.push_back(Node(auid.GetUint(),"AA.AuId"));
                    }
                }
                if (au[j].HasMember("AfId")) {
                    Value& afid = au[j]["AfId"];
                    if (type == "AA.AuId") {
                        ids.push_back(Node(afid.GetUint(),"AA.AfId"));
                    }
                }
            }
        }

        if (entities[i].HasMember("F")) {
            Value& f = entities[i]["F"];
            for (int j = 0; j < f.Capacity(); ++j) {
                Value& fid = f[j]["FId"];
                if (type == "Id") {
                    ids.push_back(Node(fid.GetUint(),"F.FId"));
                }
            }
        }

        if (entities[i].HasMember("C")) {
            Value& c = entities[i]["C"];
            Value& cid = c["CId"];
            if (type == "Id") {
                ids.push_back(Node(cid.GetUint(),"C.CId"));
            }
        }

    }


    return ids;
}


vector<Node> HttpUtil::GetSuccessor(const Node& node) {
    vector<Node> ret;
    Response r = evaluate(node.type, node.id);
    ret = GetSuc(r, node.type);  
    return ret;
}
