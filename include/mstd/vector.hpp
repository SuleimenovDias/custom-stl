#pragma once
#include <cstddef>
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
        vector() : data_(nullptr), size_(0), capacity_(0) {}
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
                T temp(value);

                std::size_t new_capacity =
                    capacity_ == 0 ? 1 : capacity_ * 2;

                reserve(new_capacity);

                std::construct_at(data_ + size_, std::move(temp));
            }

            else
            {
                std::construct_at(data_ + size_, value);
            }

            ++size_;
        }

        void pop_back()
        {
            std::destroy_at(data_ + size_ - 1);
            --size_;
        }

        void clear() noexcept
        {
            while (size_ > 0)
            {
                pop_back();
            }
        }

        void reserve(std::size_t new_capacity)
        {
            if (new_capacity <= capacity_)
                return;

            T *new_data_ = allocator_.allocate(new_capacity);

            std::size_t constructed = 0;
            try
            {
                for (std::size_t i = 0; i < size_; ++i)
                {
                    std::construct_at(new_data_ + i, data_[i]);
                    ++constructed;
                }
            }
            catch (...)
            {
                for (std::size_t i = 0; i < constructed; ++i)
                {
                    std::destroy_at(new_data_ + i);
                }

                allocator_.deallocate(new_data_, new_capacity);

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
            capacity_ = new_capacity;
        }

        T &operator[](std::size_t index)
        {
            return data_[index];
        };
        const int &operator[](std::size_t index) const
        {
            return data_[index];
        }

        std::size_t size() const noexcept
        {
            return size_;
        };

        std::size_t capacity() const noexcept
        {
            return capacity_;
        }
    };
}