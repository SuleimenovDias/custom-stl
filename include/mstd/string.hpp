#pragma once
#include <cstddef>
#include <cstring>
#include <utility>
#include <algorithm>
#include <memory>

namespace mstd
{
    class string
    {
    private:
        std::allocator<char> allocator_;
        char *data_;
        std::size_t size_;
        std::size_t capacity_;

    public:
        string() : data_(allocator_.allocate(1)), size_(0), capacity_(0)
        {
            std::construct_at(data_, '\0');
        }

        explicit string(const char *text) : data_(nullptr), size_(0), capacity_(0)
        {
            std::size_t n = strlen(text);

            data_ = allocator_.allocate(n + 1);

            for (std::size_t i = 0; i < n; ++i)
            {
                std::construct_at(data_ + i, text[i]);
            }

            std::construct_at(data_ + n, '\0');

            size_ = n;
            capacity_ = n;
        }

        ~string()
        {
            if (data_ != nullptr)
            {
                allocator_.deallocate(data_, capacity_ + 1);
            }
        }

        string(const string &other)
        {
            data_ = allocator_.allocate(other.capacity_ + 1);

            std::memcpy(data_, other.data_, other.size_ + 1);

            size_ = other.size_;
            capacity_ = other.capacity_;
        }

        string &operator=(const string &other)
        {
            if (this == &other)
                return *this;

            char *new_data = allocator_.allocate(other.capacity_ + 1);

            std::memcpy(new_data, other.data_, other.size_ + 1);

            if (data_ != nullptr)
                allocator_.deallocate(data_, capacity_ + 1);

            data_ = new_data;
            size_ = other.size_;
            capacity_ = other.capacity_;

            return *this;
        }

        string(string &&other)
        {
            char *empty = allocator_.allocate(1);
            std::construct_at(empty, '\0');

            data_ = other.data_;
            capacity_ = other.capacity_;
            size_ = other.size_;

            other.data_ = empty;
            other.capacity_ = 0;
            other.size_ = 0;
        }

        string &operator=(string &&other) noexcept
        {
            if (this == &other)
                return *this;

            char *empty = allocator_.allocate(1);
            std::construct_at(empty, '\0');

            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);

            other.data_[0] = '\0';
            other.size_ = 0;

            return *this;
        }

        std::size_t size() const noexcept
        {
            return size_;
        };

        const char *c_str() const noexcept
        {
            return data_;
        };

        std::size_t capacity() const noexcept
        {
            return capacity_;
        }

        std::size_t length() const noexcept
        {
            return size_;
        }

        char &operator[](std::size_t index)
        {
            return *(data_ + index);
        }

        const char &operator[](std::size_t index) const
        {
            return *(data_ + index);
        }

        void reserve(std::size_t new_capacity)
        {
            if (new_capacity <= capacity_)
                return;

            char *new_data_ = allocator_.allocate(new_capacity + 1);

            std::memcpy(new_data_, data_, size_ + 1);

            allocator_.deallocate(data_, capacity_ + 1);

            data_ = new_data_;
            capacity_ = new_capacity;
        }

        void push_back(char letter)
        {
            if (size_ == capacity_)
            {
                std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
                reserve(new_capacity);
            }

            data_[size_] = letter;
            ++size_;
            data_[size_] = '\0';
        }

        string &operator+=(char letter)
        {
            push_back(letter);
            return *this;
        }

        string &operator+=(const string &other)
        {
            if (other.length() == 0)
                return *this;

            std::size_t original_length = other.length();

            for (std::size_t i = 0; i < original_length; ++i)
            {
                push_back(other[i]);
            }

            return *this;
        }
    };
}