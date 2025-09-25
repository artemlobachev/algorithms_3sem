#include <iostream>
#include <cstdint>

int main()
{
    constexpr int64_t five_coin   = 5;

    uint64_t number_purch = 0;
    std::cin >> number_purch;
    
    int64_t five_change     = 0;
    int64_t five_change_min = INT64_MAX;

    for (uint64_t i = 0; i < number_purch; i++)
    {
        int64_t input_money = 0;
        std::cin >> input_money;
        if (input_money == five_coin)
            five_change++;

        else
        {
            five_change -= (input_money - five_coin) / five_coin;
            if (five_change < five_change_min)
                five_change_min = five_change;
        }
    } 

    if (five_change_min < 0) std::cout << -five_change_min << std::endl;
    else                     std::cout << 0 << std::endl;
}