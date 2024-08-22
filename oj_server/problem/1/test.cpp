bool test1()
{
    auto res = Solution().twoSum(3, 4);
    auto ans = 7;
    return judge(res, ans, 3, 4);
}

bool test2()
{
    auto res = Solution().twoSum(0, 0);
    auto ans = 0;
    return judge(res, ans, 0, 0);
}

bool test3()
{
    auto res = Solution().twoSum(10000, 10000);
    auto ans = 20000;
    return judge(res, ans, 10000, 10000);
}

bool test4()
{
    auto res = Solution().twoSum(100000000, 0);
    auto ans = 100000000;
    return judge(res, ans, 100000000, 0);
}

bool test5()
{
    auto res = Solution().twoSum(10000000, 0);
    auto ans = 10000000;
    return judge(res, ans, 10000000, 0);
}

bool test6()
{
    auto res = Solution().twoSum(1000000, 0);
    auto ans = 1000000;
    return judge(res, ans, 1000000, 0);
}

bool test7()
{
    auto res = Solution().twoSum(0, 10000);
    auto ans = 10000;
    return judge(res, ans, 0, 10000);
}

bool test8()
{
    auto res = Solution().twoSum(500, 500);
    auto ans = 500 + 500;
    return judge(res, ans, 500, 500);
}

bool test9()
{
    auto res = Solution().twoSum(99, 66);
    auto ans = 99 + 66;
    return judge(res, ans, 99, 66);
}

bool test10()
{
    auto res = Solution().twoSum(123, 456);
    auto ans = 123 + 456;
    return judge(res, ans, 123, 456);
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