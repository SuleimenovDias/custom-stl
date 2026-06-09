#include <iostream>
#include "mstd/vector.hpp"

int main()
{
    // Create an empty vector of integers
    mstd::vector<int> my_vec = {1, 2, 3, 4, 5, 6};

    std::iostream::sync_with_stdio(false); // Optimize standard streams

    std::cout << "Vector created successfully!\n";
    std::cout << "Initial Size: " << my_vec.size() << "\n";
    std::cout << "Initial Capacity: " << my_vec.capacity() << "\n";

    if (my_vec.empty())
    {
        std::cout << "Vector is currently empty.\n";
    }
    return 0;
}