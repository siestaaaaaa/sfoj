#include<bits/stdc++.h>
using namespace std;

ostream& operator<<(ostream& out, const vector<int>& v) {
    out << '[';
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i] << ",]"[i == v.size() - 1];
    }
    return out;
}

template<typename...Args>
void print(Args&&...args) {
    ((cout << args << ' '), ...);
    cout << '\n';
}

template<typename T1, typename T2, typename...Args>
bool judge(T1&& res, T2&& ans, Args&&...args) {
    if(res == ans) {
        return true;
    }
    cout << "Wrong Answer!\n";
    cout << "   input:  "; print(args...);
    cout << "   output: " << res << '\n';
    cout << "   expect: " << ans << '\n';
    return false;
}
