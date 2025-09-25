#include <cmath>
#include <iostream>


static constexpr char alphabet[27] = 
   {
    '\0','a','b','c','d','e','f','g','h','i','j','k','l','m',
            'n','o','p','q','r','s','t','u','v','w','x','y','z'
   };

inline constexpr size_t len(size_t string_order)
{
    return (1ULL << string_order) - 1;
}

// in contest we use -O2 
// so recursion depth didnt reach (cuz TCO)
// proof(godbolt: https://godbolt.org/z/ov9EqYWPs )
// return: char s_order[i - 1]
char get_char(size_t pos, size_t string_order)
{
    if (string_order == 1) return alphabet[1];
    if (pos == 1)          return alphabet[string_order];
    
    size_t len_prev = len(string_order - 1);
    
// S_i = [aplhabet[i]] + string_i-1 + string_i-1
// if len_prev + 1(alphabet[i]) > pos
//      | go left to string_i-1      |
//      | *** with pos = pos - 1 *** |
// else : 
//      |          go left to string_i-1          |
//      | *** with pos = pos - (len_prev + 1) *** |   
    
    if (pos <= len_prev + 1)
        return get_char(pos - 1, string_order - 1); 
    else
        return get_char(pos - (len_prev + 1), string_order - 1);

}

int main()
{
    size_t string_order = 0;
    size_t start_from   = 0;
    size_t end_in       = 0;

    std::cin >> string_order >> start_from >> end_in;

// Form of S_i - string in order i:
// S_i = alphabet[i] + S_(i - 1) + S(i - 1)
// For len of string in order i, we have:
// len(s_i) = 2 * len(s_(i - 1)) + 1, i > 1 and len(s_1) = 1
// Solve of this recurent is:
// len(s_i) = 2^i - 1

    for (size_t pos = start_from; pos <= end_in; pos++)
        std::cout << get_char(pos, string_order);

    std::cout << std::endl;
}