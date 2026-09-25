#pragma once
#include <cstddef>
#include <utility>
#include <memory>

namespace mstd
{
    template <typename T>
    class list
    {
    private:
        struct Node
        {
            T data;
            Node *next{nullptr};
            Node *prev{nullptr};

            constexpr Node(const T &value) : data(value) {}
        };

        Node *m_head{nullptr};
        Node *m_tail{nullptr};
        std::size_t m_size{0};

    public:
        constexpr list() noexcept = default;
        constexpr list(const list &other)
        {
            Node *current = other.m_head;
            while (current != nullptr)
            {
                push_back(current->data);
                current = current->next;
            }
        }

        constexpr void swap(list &other) noexcept
        {
            using std::swap;
            swap(m_head, other.m_head);
            swap(m_tail, other.m_tail);
            swap(m_size, other.m_size);
        }

        constexpr list &operator=(const list &other)
        {
            if (this == &other)
                return *this;
            list copy(other);
            swap(copy);
            return *this;
        }

        constexpr list(list &&other) noexcept
            : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size)
        {
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }

        constexpr list &operator=(list &&other) noexcept
        {
            if (this == &other)
                return *this;
            clear();

            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;

            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
            return *this;
        }

        constexpr ~list() noexcept
        {
            clear();
        }

        constexpr void push_back(const T &value)
        {
            Node *new_node = new Node(value);

            if (m_head == nullptr)
            {
                m_head = new_node;
                m_tail = new_node;
                m_size++;
                return;
            }

            m_tail->next = new_node;
            new_node->prev = m_tail;
            m_tail = new_node;
            m_size++;
        }

        constexpr void push_front(const T &value)
        {
            Node *new_node = new Node(value);

            if (m_head == nullptr)
            {
                m_head = new_node;
                m_tail = new_node;
                m_size++;
                return;
            }

            new_node->next = m_head;
            m_head->prev = new_node;
            m_head = new_node;
            m_size++;
        }

        constexpr void clear() noexcept
        {
            Node *current = m_head;
            while (current != nullptr)
            {
                Node *next_node = current->next;
                delete current;
                current = next_node;
            }
            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
        }

        constexpr std::size_t size() const noexcept { return m_size; }
        constexpr bool empty() const noexcept { return m_size == 0; }

        constexpr void pop_back()
        {
            if (m_tail == nullptr)
                return;

            Node *node_to_delete = m_tail;

            if (m_head == m_tail)
            {
                m_head = nullptr;
                m_tail = nullptr;
            }

            else
            {
                m_tail = m_tail->prev;
                m_tail->next = nullptr;
            }

            delete node_to_delete;
            m_size--;
        }

        constexpr void pop_front()
        {
            if (m_head == nullptr)
                return;

            Node *node_to_delete = m_head;

            if (m_head == m_tail)
            {
                m_head = nullptr;
                m_tail = nullptr;
            }

            else
            {
                m_head = m_head->next;
                m_head->prev = nullptr;
            }

            delete node_to_delete;
            m_size--;
        }

        constexpr T &front() { return m_head->data; }
        constexpr const T &front() const { return m_head->data; }

        constexpr T &back() { return m_tail->data; }
        constexpr const T &back() const { return m_tail->data; }
    };
}
