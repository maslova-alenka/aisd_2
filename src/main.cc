
#include<cmath>
#include <iostream>
#include "../include/List.h"

int main() {
    CyclicList<int> list;

    list.push_tail(1);
    list.push_tail(2);
    list.push_tail(3);

    std::cout << "Original list: ";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;

    list.reverse();

    std::cout << "Reversed list: ";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
