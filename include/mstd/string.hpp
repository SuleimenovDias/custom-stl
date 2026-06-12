#pragma once
#include <cstddef>
#include <cstring>
#include <utility>
#include <algorithm>

namespace mstd
{
    class string
    {
    private:
        static constexpr std::size_t SSO_CAPACITY = 15;

        struct LongLayout
        {
            char *start;
            std::size_t size;
            std::size_t capacity;
        };

        struct ShortLayout
        {
            char buffer[SSO_CAPACITY];
            unsigned char shifting_or_length_flag;
        };

        union
        {
            LongLayout m_long;
            ShortLayout m_short;
        };

        bool m_is_long = false;

    public:
        string() = default;

        string(const string &other)
        {
            m_is_long = other.m_is_long;

            if (!m_is_long)
            {
                std::memcpy(m_short.buffer, other.m_short.buffer, SSO_CAPACITY);
                m_short.shifting_or_length_flag = other.m_short.shifting_or_length_flag;
                return;
            }

            m_long.start = static_cast<char *>(::operator new(other.m_long.size + 1));
            std::memcpy(m_long.start, other.m_long.start, other.m_long.size);
            m_long.start[other.m_long.size] = '\0';

            m_long.size = other.m_long.size;
            m_long.capacity = other.m_long.size;
        }

        ~string()
        {
            if (m_is_long)
            {
                ::operator delete(m_long.start);
            }
        }

        constexpr std::size_t size() const noexcept
        {
            if (m_is_long)
                return m_long.size;
            return m_short.shifting_or_length_flag;
        }

        constexpr const char *c_str() const
        {
            if (m_is_long)
                return m_long.start;
            return m_short.buffer;
        }

        char &operator[](std::size_t index)
        {
            if (m_is_long)
                return m_long.start[index];
            return m_short.buffer[index];
        }

        constexpr void swap(string &other)
        {
            using std::swap;
            swap(m_long, other.m_long);
            swap(m_is_long, other.m_is_long);
        }

        string &operator=(const string &other)
        {
            if (this == &other)
            {
                return *this;
            }

            string copy(other);
            swap(copy);

            return *this;
        }

        constexpr string &append(const char *str, std::size_t count)
        {
            if (count == 0 || str == nullptr)
                return *this;

            const std::size_t current_size = size();
            const std::size_t new_size = current_size + count;

            if (!m_is_long)
            {
                if (new_size <= SSO_CAPACITY)
                {
                    std::copy(str, str + count, m_short.buffer + current_size);
                    m_short.buffer[new_size] = '\0';
                    m_short.shifting_or_length_flag = static_cast<unsigned char>(new_size);
                    return *this;
                }

                std::size_t new_cap = (new_size * 2 < 32) ? 32 : new_size * 2;
                char *new_start = new char[new_cap + 1];

                std::copy(m_short.buffer, m_short.buffer + current_size, new_start);
                std::copy(str, str + count, new_start + current_size);
                new_start[new_size] = '\0';

                m_is_long = true;
                m_long.start = new_start;
                m_long.size = new_size;
                m_long.capacity = new_cap;
                return *this;
            }

            if (new_size <= m_long.capacity)
            {
                std::copy(str, str + count, m_long.start + current_size);
                m_long.start[new_size] = '\0';
                m_long.size = new_size;
                return *this;
            }

            std::size_t new_cap = m_long.capacity * 2;
            if (new_cap < new_size)
            {
                new_cap = new_size;
            }

            char *new_start = new char[new_cap + 1];

            std::copy(m_long.start, m_long.start + current_size, new_start);
            std::copy(str, str + count, new_start + current_size);
            new_start[new_size] = '\0';

            delete[] m_long.start;
            m_long.start = new_start;
            m_long.size = new_size;
            m_long.capacity = new_cap;

            return *this;
        }

        constexpr string &operator+=(const string &other)
        {
            return append(other.c_str(), other.size());
        }

        constexpr string &operator+=(const char *str)
        {
            if (str == nullptr)
                return *this;
            return append(str, std::strlen(str));
        }

        constexpr string &operator+=(char c)
        {
            return append(&c, 1);
        }

        friend string operator+(string lhs, const string &rhs)
        {
            lhs += rhs;
            return lhs;
        }

        friend string operator+(string lhs, const char *rhs)
        {
            lhs += rhs;
            return lhs;
        }

        friend string operator+(const char *lhs, string rhs)
        {
            string result(lhs);
            result += rhs;
            return result;
        }

        // void operator+=(const string &other)
        // {
        //     std::size_t new_size = size() + other.size();

        //     if (new_size > SSO_CAPACITY)
        //     {
        //         m_is_long = true;
        //         if (new_size > m_long.capacity)
        //         {
        //             m_long.start = static_cast<char *>(::operator new(new_size + 1));

        //         }
        //     }
        // }
    };
}