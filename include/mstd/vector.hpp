#pragma once
#include <cstddef>
#include <utility>
#include <memory>

namespace mstd
{

    template <typename T>
    class vector
    {
    private:
        T *m_start = nullptr;
        T *m_last = nullptr;
        T *m_end_of_storage = nullptr;

        constexpr void grow()
        {
            size_t old_capacity = capacity();
            size_t new_capacity = old_capacity == 0 ? 1 : old_capacity * 2;

            T *new_start = static_cast<T *>(::operator new(new_capacity * sizeof(T)));
            T *new_last = new_start;

            try
            {
                for (T *curr = m_start; curr != m_last; ++curr)
                {
                    std::construct_at(new_last, std::move(*curr));
                    new_last++;
                }
            }
            catch (...)
            {
                for (T *clean = new_start; clean != new_last; ++clean)
                {
                    std::destroy_at(clean);
                }
                ::operator delete(new_start);
                throw;
            }

            for (T *curr = m_start; curr != m_last; ++curr)
            {
                std::destroy_at(curr);
            }

            ::operator delete(m_start);

            m_start = new_start;
            m_last = new_last;
            m_end_of_storage = new_start + new_capacity;
        }

    public:
        vector() = default;

        constexpr vector(std::size_t count, const T &new_item) : m_start(nullptr), m_last(nullptr), m_end_of_storage(nullptr)
        {
            for (std::size_t i = 0; i < count; ++i)
            {
                push_back(new_item);
            }
        }

        constexpr vector &operator=(const vector &other)
        {
            if (this == &other)
            {
                return *this;
            }

            vector copy(other);
            swap(copy);
            return *this;
        }

        constexpr vector(const vector &other)
        {
            std::size_t count = other.m_last - other.m_start;

            if (count == 0)
            {
                m_start = nullptr;
                m_last = nullptr;
                m_end_of_storage = nullptr;
                return;
            }

            m_start = static_cast<T *>(::operator new(count * sizeof(T)));
            m_last = m_start;
            m_end_of_storage = m_start + count;

            try
            {
                for (T *curr = other.m_start; curr != other.m_last; curr++)
                {
                    std::construct_at(m_last, *curr);
                    m_last++;
                }
            }
            catch (...)
            {
                std::destroy(m_start, m_last);
                ::operator delete(m_start);
                throw;
            }
        }

        constexpr vector(vector &&other) noexcept
            : m_start(other.m_start), m_last(other.m_last), m_end(other.m_end)
        {
            other.m_start = nullptr;
            other.m_last = nullptr;
            other.m_end = nullptr;
        }

        constexpr vector &operator=(vector &&other) noexcept
        {
            if (this == &other)
                return *this;

            if (m_start)
            {
                delete[] m_start;
            }

            m_start = other.m_start;
            m_last = other.m_last;
            m_end = other.m_end;

            other.m_start = nullptr;
            other.m_last = nullptr;
            other.m_end = nullptr;

            return *this;
        }

        constexpr ~vector()
        {
            std::destroy(m_start, m_last);
            ::operator delete(m_start);
        }

        constexpr std::size_t size() const noexcept
        {
            if (m_start == nullptr)
            {
                return 0;
            }

            return m_last - m_start;
        }

        constexpr std::size_t capacity() const noexcept
        {
            if (m_start == nullptr)
            {
                return 0;
            }

            return m_end_of_storage - m_start;
        }

        constexpr bool empty() const noexcept
        {
            return m_start == m_last;
        }

        constexpr void push_back(T new_item)
        {
            if (m_last == m_end_of_storage)
            {
                grow();
            }

            std::construct_at(m_last, std::move(new_item));
            m_last++;
        }

        constexpr void swap(vector &other) noexcept
        {
            using std::swap;
            swap(m_start, other.m_start);
            swap(m_last, other.m_last);
            swap(m_end_of_storage, other.m_end_of_storage);
        }

        T &operator[](std::size_t index) noexcept
        {
            return m_start[index];
        }

        const T &operator[](std::size_t index) const noexcept
        {
            return m_start[index];
        }
    };
}