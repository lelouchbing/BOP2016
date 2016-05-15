#ifndef _NODE_H_
#define _NODE_H_
#include <string>
#include <stdio.h>

using namespace std;

struct Node {
    unsigned long long id;
    std::string type;
    Node(unsigned long long pid, string ptype) {
        id = pid;
        type = ptype;
    }
    string tostring() {
        char res[100];
        sprintf(res, "%s%lld", type.c_str(), id);
        return string(res);
    }
};

#endif
