#pragma once
#include <cstddef>
#include <utility>

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
        constexpr vector(std::size_t new_size, T new_item) noexcept
        {
            while (size() < new_size)
            {
                grow();
            }

            push
        }
        constexpr ~vector()
        {
            std::destroy(m_start, m_last);
            ::operator delete(m_start);
        }

        constexpr std::size_t size() const noexcept
        {
            return m_last - m_start;
        }

        constexpr std::size_t capacity() const noexcept
        {
            return m_end_of_storage - m_start;
        }

        constexpr bool empty()
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