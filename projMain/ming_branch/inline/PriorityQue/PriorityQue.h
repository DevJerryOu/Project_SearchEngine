#include <vector>
#include <string>

using std::string;
using std::vector;
using std::pair;

using type = pair<string,int>;

class Priority
{
public:
    Priority(vector<type> &str);
    ~Priority();

    string sort();
    // vector<type> Cncort();

private:
    struct cmp{
        bool operator() (pair<string,int>&a,pair<string,int>&b)
        {
            return a.second < b.second;  //大的优先级在前面
        }
    };

private:
    vector<type> _str;
};
