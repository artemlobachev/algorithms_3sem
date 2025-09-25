#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

static constexpr int IMPOSSIBLE = INT_MAX;

struct SolveInfo 
{
    int  min_exact_coins   = IMPOSSIBLE; 
    bool can_overpay       = false;          
    int  min_overpay_coins = IMPOSSIBLE;
};

void recursion_cost(long remaining_cost, const std::vector<long> &coins, 
                   size_t coin_index, int curr_coins, SolveInfo& solve_info)
{
    if (remaining_cost == 0) 
    {
        solve_info.min_exact_coins = std::min(solve_info.min_exact_coins, curr_coins);
        return;
    }
    
    if (remaining_cost < 0) 
    {
        solve_info.can_overpay = true;
        solve_info.min_overpay_coins = std::min(solve_info.min_overpay_coins, curr_coins);
        return;
    }
    
    if (coin_index >= coins.size()) 
        return;
    
    long coin_value = coins[coin_index];

    // node1: don`t take any coin
    recursion_cost(remaining_cost, coins, 
        coin_index + 1, curr_coins, solve_info);
    
    // node2: take only one coin
    recursion_cost(remaining_cost - coin_value, coins, 
                       coin_index + 1, curr_coins + 1, solve_info);
    
    // node3: take two coins
    recursion_cost(remaining_cost - 2 * coin_value, coins, 
                       coin_index + 1, curr_coins + 2, solve_info);
}

int main()
{
    long cost = 0;
    size_t amount_coins = 0;
    std::cin >> cost >> amount_coins;
    
    std::vector<long> coins(amount_coins); 
    for (size_t i = 0; i < amount_coins; i++)
        std::cin >> coins[i];
    
    // check on possibility
    long total_money = 0;
    for (long coin : coins)
        total_money += 2 * coin;
    
    if (total_money < cost) 
    {
        std::cout << "Impossible" << std::endl;
        return 0;
    }

    SolveInfo solve_info = {};
    // for solve this problem i imagine recursion tree
    // that split on 3 node: 
    //      first:  is call without taking coin
    //      second: is call with only coin
    //      third:  is call with two coins
    recursion_cost(cost, coins, 0, 0, solve_info);

    if (solve_info.min_exact_coins != IMPOSSIBLE) std::cout << solve_info.min_exact_coins << std::endl;
    else if (solve_info.can_overpay)              std::cout <<          "Change"          << std::endl;
    else                                          std::cout <<        "Impossible"        << std::endl;
    
    return 0;
}