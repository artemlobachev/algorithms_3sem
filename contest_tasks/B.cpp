#include <iostream>
#include <cstdint>


int main()
{
    uint64_t number_count = 0;
    std::cin >> number_count;

    uint64_t min_cost_of_item_can_buy     = 0;
    for (uint64_t i = 0, input_number = 0; i < number_count; i++)
    {   
        std::cin >> input_number;
        if (min_cost_of_item_can_buy + 1 < input_number) 
            break;

        min_cost_of_item_can_buy += input_number;
    }

    std::cout << min_cost_of_item_can_buy + 1 << std::endl;

    return 0;
}