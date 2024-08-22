bool test1()
{
    auto res = Solution().fourSum(3, 4, 5, 6);
    auto ans = 3 + 4 + 5 + 6;
    return judge(res, ans, 3, 4, 5, 6);
}

bool test2()
{
    auto res = Solution().fourSum(0, 0, 0, 0);
    auto ans = 0;
    return judge(res, ans, 0, 0, 0, 0);
}

bool test3()
{
    auto res = Solution().fourSum(10000, 10000, 10000, 10000);
    auto ans = 40000;
    return judge(res, ans, 10000, 10000, 10000, 10000);
}

bool test4()
{
    auto res = Solution().fourSum(10000000, 0, 10000000, 10000000);
    auto ans = 30000000;
    return judge(res, ans, 10000000, 0, 10000000, 10000000);
}

bool test5()
{
    auto res = Solution().fourSum(10000000, 0, 0, 1);
    auto ans = 10000001;
    return judge(res, ans, 10000000, 0, 0, 1);
}

bool test6()
{
    auto res = Solution().fourSum(0, 0, 1000000, 0);
    auto ans = 1000000;
    return judge(res, ans, 0, 0, 1000000, 0);
}

bool test7()
{
    auto res = Solution().fourSum(0, 10000, 10000, 10000);
    auto ans = 30000;
    return judge(res, ans, 0, 10000, 10000, 10000);
}

bool test8()
{
    auto res = Solution().fourSum(500, 500, 500, 500);
    auto ans = 2000;
    return judge(res, ans, 500, 500, 500, 500);
}

bool test9()
{
    auto res = Solution().fourSum(99, 66, 33, 11);
    auto ans = 99 + 66 + 33 + 11;
    return judge(res, ans, 99, 66, 33, 11);
}

bool test10()
{
    auto res = Solution().fourSum(123, 456, 789, 101112);
    auto ans = 123 + 456 + 789 + 101112;
    return judge(res, ans, 123, 456, 789, 101112);
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