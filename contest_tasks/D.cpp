#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct GrayCode
{
    char *string;
    size_t size;
};

void print_gray_code(GrayCode *gray_code, size_t curr_gray_size, bool is_reverse)
{
    if (curr_gray_size == gray_code->size)
    {
        gray_code->string[gray_code->size] = '\0';
        puts(gray_code->string);
        return;
    }

    if (!is_reverse)
    {
        gray_code->string[curr_gray_size] = '0';
        print_gray_code(gray_code, curr_gray_size + 1, false);

        gray_code->string[curr_gray_size] = '1';
        print_gray_code(gray_code, curr_gray_size + 1, true);
        
    }
    else
    {
        gray_code->string[curr_gray_size] = '1';
        print_gray_code(gray_code, curr_gray_size + 1, false);

        gray_code->string[curr_gray_size] = '0';
        print_gray_code(gray_code, curr_gray_size + 1, true);
    }
}

/*
                    Gray(3)
                   /        \
            0+Gray(2)      1+Gray(2,reverse)
           /        \          /          \
    0+Gray(1)  1+Gray(1,r) 1+Gray(1,r)  1+Gray(1)
   /    \      /        \   /        \    /      \
 000   001   011      010 110      111  101    100


 */
int main()
{
    size_t len_gray_code = 0;
    int warn_fix = scanf("%zu", &len_gray_code);

    GrayCode gray_code = { .size = len_gray_code };
    gray_code.string = (char *) calloc(len_gray_code + 1, sizeof(char));
    if (!gray_code.string)   return EXIT_FAILURE;


    const size_t START_GRAY_SIZE = 0;
    print_gray_code(&gray_code, START_GRAY_SIZE, false);

    free(gray_code.string);


    warn_fix = EXIT_SUCCESS;
    return warn_fix;
}