#include <string>
#include "node.h"
#include "http_util.h"

using namespace std;


vector<vector<Node> > res;
HttpUtil http;

/*
void dfs(const vector<Node>& q, const Node& end, vector<Node> path, int dep) {
    for (int i = 0; i < q.size(); ++i) {
        vector<Node> temp = path;
        if (q[i].id == end.id) {
            temp.push_back(end);
            res.push_back(path);
        }
        temp = path;
        temp.push_back(q[i]);
        vector<Node> suc = http.GetSuccessor(q[i]);
        if (dep < 3)
            dfs(suc, end, temp, dep+1);
    } 
    return;
}
*/

void match(Node s, Node e) {
    vector<Node> q1 = http.GetSuccessor(s);
    vector<Node> q2 = http.GetSuccessor(e);

    for (int i = 0; i < q1.size(); ++i) {
        if (q1[i].id == e.id) {
           vector<Node> path;
           path.push_back(s);
           path.push_back(e);
           res.push_back(path);
           break;
        }
    }

    for (int i = 0; i < q1.size(); ++i) {
        for (int j = 0; j < q2.size(); ++j) {
            if (q1[i].id == q2[j].id) {
                vector<Node> path;
                
                path.push_back(s);
                path.push_back(q1[i]);
                path.push_back(e);
                res.push_back(path);
            }
        }
    }
    for (int i = 0; i < q1.size(); ++i) {
        for (int j = 0; j < q2.size(); ++j) {
            bool flag = http.Match(q1[i], q2[j]);
            if (flag) {
                vector<Node> path;
                
                path.push_back(s);
                path.push_back(q1[i]);
                path.push_back(q2[j]);
                path.push_back(e);
                res.push_back(path);
                cout << 123 <<endl;
            }
        }
    }
}

int main() {
    unsigned long long start = 57898110;
    unsigned long long end = 2014261844;
    start = 189831743;
    end = 2147152072;
    string type1 = http.GetIdType(start);
    string type2 = http.GetIdType(end);
    cout << type1 <<endl;
    cout << type2 <<endl;
   
    Node s(start, type1);
    Node e(end, type2);

    /*
    vector<Node> que = http.GetSuccessor(s);
    vector<Node> path;
    path.push_back(s);
    // first
    dfs(que, e, path, 1);    
*/
    // check
    match(s, e);
    for (int i = 0; i < res.size(); ++i) {
        for (int j = 0; j < res[i].size(); ++j) {
            cout << res[i][j].id << " ";
        }
        cout << endl;
    }
    return 0;
}
