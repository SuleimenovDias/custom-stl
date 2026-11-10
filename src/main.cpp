#include <iostream>
#include <mstd/vector.hpp>

int main()
{
    mstd::vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    v.resize(5);

    for (std::size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }

    std::cout << "\nSize: " << v.size();
    std::cout << "\nCapacity: " << v.capacity();

    v.resize(2);

    std::cout << "\nAfter shrinking:";
    std::cout << "\nSize: " << v.size();
    std::cout << "\nCapacity: " << v.capacity();
}