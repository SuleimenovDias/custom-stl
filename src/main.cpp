#include <iostream>
#include "mstd/vector.hpp"

int main()
{
    // Create an empty vector of integers
    mstd::vector<int> my_vec(2, 3);

    mstd::vector<int> v2(my_vec);

    std::iostream::sync_with_stdio(false); // Optimize standard streams

    std::cout << "Vector created successfully!\n";
    std::cout << "Initial Size: " << v2.size() << "\n";
    std::cout << "Initial Capacity: " << v2.capacity() << "\n";

    if (v2.empty())
    {
        std::cout << "Vector is currently empty.\n";
    }
    return 0;
}