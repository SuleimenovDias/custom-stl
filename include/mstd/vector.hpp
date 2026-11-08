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
        std::allocator<T> allocator_;
        T *data_;
        std::size_t size_;
        std::size_t capacity_;

    public:
        vector() : data_(nullptr), size_(0), capacity_(0);
        ~vector()
        {
            if (capacity_ > 0)
            {
                for (std::size_t i = 0; i < size_; ++i)
                {
                    std::destroy_at(data_ + i);
                }
                allocator_.deallocate(data_, capacity_);
            }
        };

        vector(const vector &) = delete;
        vector &operator=(const vector &) = delete;

        void push_back(const T &value)
        {
            if (size_ == capacity_)
            {
                std::size_t new_capacity_;
                if (capacity_ == 0)
                {
                    new_capacity_ = 1;
                }
                else
                {
                    new_capacity_ = capacity_ * 2;
                }

                T *new_data_ = allocator_.allocate(new_capacity_);

                // counter to count how many objects constructed -> for exception handlings
                std::size_t constructed = 0;

                // try to construct
                try
                {
                    for (std::size_t i = 0; i < size_; ++i)
                    {
                        std::construct_at(new_data_ + i, data_[i]);
                        ++constructed;
                    }

                    std::construct_at(new_data_ + size_, value);
                    ++constructed;
                }
                // if error appears we destroy new constructed vector and returns to initial version
                catch (...)
                {
                    for (std::size_t i = 0; i < constructed; ++i)
                    {
                        std::destroy_at(new_data_ + i);
                    }

                    allocator_.deallocate(new_data_, new_capacity_);

                    throw;
                }

                for (std::size_t i = 0; i < size_; ++i)
                {
                    std::destroy_at(data_ + i);
                }

                if (capacity_ > 0)
                {
                    allocator_.deallocate(data_, capacity_);
                }

                data_ = new_data_;
                ++size_;
                capacity_ = new_capacity_;
            }

            else
            {
                std::construct_at(data_ + size_, value);
                ++size_;
            }
        };

        int &operator[](std::size_t index);
        const int &operator[](std::size_t index) const;

        std::size_t size() const noexcept;
        // std::size_t capacity() const noexcept;
    };
}