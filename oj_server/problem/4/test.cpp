bool test1()
{
    vector<int> arr{3, 4, 1, 2};
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test2()
{
    vector<int> arr;
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test3()
{
    vector<int> arr(1000000);
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

mt19937 gen;
uniform_int_distribution dis(numeric_limits<int>::min(), numeric_limits<int>::max());
bool test4()
{
    vector<int> arr;
    for(int i = 0; i < 100; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test5()
{
    vector<int> arr;
    for(int i = 0; i < 100; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test6()
{
    vector<int> arr;
    for(int i = 0; i < 1000; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test7()
{
    vector<int> arr;
    for(int i = 0; i < 1000; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test8()
{
    vector<int> arr;
    for(int i = 0; i < 10000; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test9()
{
    vector<int> arr;
    for(int i = 0; i < 10000; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

bool test10()
{
    vector<int> arr;
    for(int i = 0; i < 100000; i++) arr.push_back(dis(gen));
    auto ans = arr;
    sort(begin(ans), end(ans));
    Solution().sortArray(arr);
    return judge(arr, ans, arr);
}

int main()
{
    if(!test1()) exit(0);
    if(!test2()) exit(0);
    if(!test3()) exit(0);
    if(!test4()) exit(0);
    if(!test5()) exit(0);
    if(!test6()) exit(0);
    if(!test7()) exit(0);
    if(!test8()) exit(0);
    if(!test9()) exit(0);
    if(!test10()) exit(0);
    cout << "Accepted!\n";
}