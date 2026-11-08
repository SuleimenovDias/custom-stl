#include <iostream>
#include <mstd/vector.hpp>

int main()
{
    mstd::vector<int> v;

    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i * 10);

        std::cout << "size=" << v.size()
                  << ", capacity=" << v.capacity()
                  << '\n';
    }

    for (std::size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << ' ';
    }

    std::cout << '\n';

    v.pop_back();
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << ' ';
    }

    v.clear();
    std::cout << v.capacity() << std::endl;
    std::cout << v.size() << std::endl;
    
}