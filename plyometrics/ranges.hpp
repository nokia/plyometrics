#pragma once

namespace plyometrics
{

struct random_range
{
    random_range(std::size_t size)
        : _engine(std::random_device{}()), _size(size)
    {
    }

    random_range(random_range&& other)
        : _engine(std::move(other._engine)), _size(other._size)
    {
    }

    using engine = std::mt19937;

    struct const_iterator
    {
        using difference_type = std::size_t;
        using value_type = int;
        using pointer = int*;
        using reference = int&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator(engine& e) : _engine(&e), _index(0)
        {
            _value = e();
        }

        /**
         * used to construct `end` iterator
         */
        const_iterator(std::size_t index) : _engine(nullptr), _index(index)
        {
        }

        auto operator*() const
        {
            return _value;
        }

        auto& operator++()
        {
            _value = (*_engine)();
            _index++;
            return *this;
        }

        bool operator!=(const const_iterator& other) const
        {
            return _index != other._index;
        }

    private:
         engine* _engine;
         std::size_t _index;
         int _value;
    };

    auto begin()
    {
        return const_iterator{_engine};
    }

    auto end()
    {
        return const_iterator{_size};
    }

private:
    engine _engine;
    std::size_t _size;
};

struct sequence_range
{
    explicit sequence_range(std::size_t size) : _size(size)
    {
    }

    struct const_iterator
    {
        using difference_type = std::size_t;
        using value_type = std::size_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator(value_type v) : _value(v)
        {
        }

        auto operator*() const
        {
            return _value;
        }

        auto& operator++()
        {
            _value++;
            return *this;
        }

        bool operator!=(const const_iterator& other) const
        {
            return _value != other._value;
        }

    private:
         std::size_t _value;
    };

    auto begin()
    {
        return const_iterator{_start};
    }

    auto end()
    {
        return const_iterator{_size};
    }

private:
    std::size_t _start = 0;
    std::size_t _size;
};

}
