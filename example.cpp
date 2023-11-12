#include "templatesort.h"
#include <iostream>
#include <array>

int main(){

    std::array<size_t, 5> arr{2, 1, 3, 5, 4};
    dg::sort::template_sort_arr(&arr.front(), std::integral_constant<size_t, 5>{});

    for (const auto& e: arr){
        std::cout << e << std::endl;
    }

}