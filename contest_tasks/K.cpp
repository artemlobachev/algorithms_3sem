#include <iostream>
#include <stack>

inline bool is_open_bracket(char bracket)
{
    return (bracket == '{' || 
            bracket == '[' ||
            bracket == '(' ||
            bracket == '<');
}          

inline bool is_close_bracket(char bracket)
{
    return (bracket == '}' || 
            bracket == ']' ||
            bracket == ')' ||
            bracket == '>');
}

inline bool is_different_bracket(char bracket, char top_of_stack)
{
    return ((top_of_stack == '(' && bracket != ')') ||
            (top_of_stack == '[' && bracket != ']') ||
            (top_of_stack == '{' && bracket != '}') ||
            (top_of_stack == '<' && bracket != '>'));
}

int main()
{
    std::stack<char> bracket_sequence;
    bool is_correct = true;

    char bracket = '\0';
    while ((bracket = getchar()) != EOF)
    {
        if (is_open_bracket(bracket))
            bracket_sequence.push(bracket);

        else if (is_close_bracket(bracket))
        {
            if (bracket_sequence.empty() || 
                is_different_bracket(bracket, bracket_sequence.top()))
            {
                is_correct = false;
                break;
            }

            bracket_sequence.pop();
        }
    }

    if (is_correct && bracket_sequence.empty()) std::cout << "YES" << std::endl; 
    else                                        std::cout << "NO"  << std::endl;

    return 0;
}