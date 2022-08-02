#include "PriorityQue.h"
#include <unordered_map>
#include <queue>
#include <iostream>

using std::unordered_map;
using std::priority_queue;
using std::cout;
using std::endl;

Priority::Priority(vector<type> &str)
: _str(str) { }

Priority::~Priority() {}

string Priority::sort()
{
    priority_queue<type,vector<type>,cmp> que;
    for(auto &[key,value] : _str)
    {
        que.push({key,value});
    }
    type s;
    while(!que.empty()){
        s = que.top();
    }
    return s.first;
}

