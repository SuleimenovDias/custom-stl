#include <string_view>

namespace mstd
{
    class trie
    {
    private:
        struct TrieNode
        {
            TrieNode *children[26]{nullptr};
            bool is_end_of_word{false};

            ~TrieNode()
            {
                for (TrieNode *child : children)
                {
                    delete child;
                }
            }
        };

        TrieNode *m_root;

        constexpr bool erase_helper(TrieNode *current, std::string_view word, std::size_t depth)
        {
            if (current == nullptr)
                return false;

            if (depth == word.size())
            {
                if (!current->is_end_of_word)
                    return false;

                current->is_end_of_word = false;

                return has_no_children(current);
            }

            int index = word[depth] - 'a';
            if (erase_helper(current->children[index], word, depth + 1))
            {
                delete current->children[index];
                current->children[index] = nullptr;

                return !current->is_end_of_word && has_no_children(current);
            }

            return false;
        }

        constexpr bool has_no_children(TrieNode *node) const
        {
            for (TrieNode *child : node->children)
            {
                if (child != nullptr)
                    return false;
            }
            return true;
        }

    public:
        trie() : m_root(new TrieNode()) {}

        constexpr trie(trie &&other) noexcept : m_root(other.m_root)
        {
            other.m_root = nullptr;
        }

        constexpr trie &operator=(trie &&other) noexcept
        {
            if (this == &other)
                return *this;

            delete m_root;
            m_root = other.m_root;
            other.m_root = nullptr;

            return *this;
        }

        ~trie()
        {
            delete m_root;
        }
        trie(const trie &) = delete;
        trie &operator=(const trie &) = delete;

        constexpr void insert(std::string_view word)
        {
            TrieNode *current = m_root;

            for (char c : word)
            {
                int index = c - 'a';

                if (current->children[index] == nullptr)
                {
                    current->children[index] = new TrieNode();
                }
                current = current->children[index];
            }

            current->is_end_of_word = true;
        }

        constexpr bool search(std::string_view word) const
        {
            TrieNode *current = m_root;

            for (char c : word)
            {
                int index = c - 'a';
                if (current->children[index] == nullptr)
                {
                    return false;
                }
                current = current->children[index];
            }

            return current->is_end_of_word;
        }

        constexpr bool starts_with(std::string_view prefix) const
        {
            TrieNode *current = m_root;

            for (char c : prefix)
            {
                int index = c - 'a';
                if (current->children[index] == nullptr)
                {
                    return false;
                }
                current = current->children[index];
            }

            return true;
        }

        constexpr void erase(std::string_view word)
        {
            erase_helper(m_root, word, 0);
        }
    };
}