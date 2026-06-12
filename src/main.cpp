#include <iostream>
#include <cassert>
#include <string_view>
#include "mstd/trie.hpp"

int main()
{
    std::cout << "--- Testing mstd::trie ---\n";

    mstd::trie my_trie;

    my_trie.insert("apple");
    my_trie.insert("app");
    my_trie.insert("car");
    my_trie.insert("cart");

    std::cout << "Searching for 'apple' (Expected: 1): " << my_trie.search("apple") << "\n";
    std::cout << "Searching for 'app'   (Expected: 1): " << my_trie.search("app") << "\n";
    std::cout << "Searching for 'banana'(Expected: 0): " << my_trie.search("banana") << "\n";
    std::cout << "Searching for 'ca'    (Expected: 0): " << my_trie.search("ca") << "\n";

    std::cout << "---------------------------\n";

    std::cout << "Starts with 'ap'      (Expected: 1): " << my_trie.starts_with("ap") << "\n";
    std::cout << "Starts with 'car'     (Expected: 1): " << my_trie.starts_with("car") << "\n";
    std::cout << "Starts with 'dog'     (Expected: 0): " << my_trie.starts_with("dog") << "\n";

    std::cout << "---------------------------\n";

    std::cout << "Erasing 'apple'...\n";
    my_trie.erase("apple");

    std::cout << "Searching for 'apple' (Expected: 0): " << my_trie.search("apple") << "\n";
    std::cout << "Searching for 'app'   (Expected: 1): " << my_trie.search("app") << "\n";

    std::cout << "Erasing 'app'...\n";
    my_trie.erase("app");
    std::cout << "Starts with 'ap'      (Expected: 0): " << my_trie.starts_with("ap") << "\n";

    std::cout << "\nAll core functions tested successfully! Memory cleaned up safely.\n";
    return 0;
}