#include <iostream>
#include <map>
#include <utility>
#include "container.h"
#include "allocator.h"
#include "lib.h"

int main()
{
    std::map<int, size_t> firstMap;
    for (int i = 0; i < 10; ++i) {
        firstMap[i] = fact(i);
    }
    using myAlloc = customAlloc<std::pair<int, size_t>, 10>;
    auto secondMap = std::map<int, size_t, std::less<>, myAlloc>{};
    for (int i = 0; i < 10; ++i) {
        secondMap[i] = fact(i);
    }
    for (auto a : secondMap) {
        std::cout << a.first << " " << a.second << std::endl;
    }
    //containerList<int> firstList;

}