#ifndef __CHINESE_H__
#define __CHINESE_H__

#include <stdint.h>
#include <stddef.h>
#include <algorithm>

template <typename key_t, typename value_t>
struct KeyValuePairs
{
    key_t key;
    value_t value;
};

template <typename key_t, typename value_t, size_t size>
struct Dictionary
{
private:
    static constexpr uint8_t empty[30] = {0};
    KeyValuePairs<key_t, value_t> elements[size];

public:
    constexpr Dictionary(void);

    template <typename... Args>
    constexpr Dictionary(Args... args);

    static constexpr size_t get_size(void) { return size; }
    const value_t &operator[](wchar_t key) const;
    constexpr Dictionary<key_t, value_t, size> sort(void) const;
    constexpr Dictionary<key_t, value_t, size> inv(void) const;
};

template <typename key_t, typename value_t, typename... Args>
Dictionary(KeyValuePairs<key_t, value_t>, Args...) -> Dictionary<key_t, value_t, sizeof...(Args) + 1>;

template <typename key_t, typename value_t, size_t size>
constexpr Dictionary<key_t, value_t, size>::Dictionary(void)
    : elements{}
{
    static_assert(size == 0);
}

template <typename key_t, typename value_t, size_t size>
template <typename... Args>
constexpr Dictionary<key_t, value_t, size>::Dictionary(Args... elements) : elements{elements...}
{
    static_assert(sizeof...(elements) == size);
    
    for (size_t i = 0; i < size - 1; i++)
    {
        size_t k = i;
        for (size_t j = i + 1; j < size; j++)
        {
            if (this->elements[k].key > this->elements[j].key)
                k = j;
            auto temp = this->elements[k];
            this->elements[k] = this->elements[i];
            this->elements[i] = temp;
        }
    }
}

template <typename key_t, typename value_t, size_t size>
const value_t &Dictionary<key_t, value_t, size>::operator[](wchar_t key) const
{
    for (size_t left = 0, right = size - 1, mid; left <= right;)
    {
        mid = (left + right) / 2;
        if (this->elements[mid].key == key)
        {
            return this->elements[mid].value;
        }
        else if (this->elements[mid].key < key)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return empty;
}

template <typename key_t, typename value_t, size_t size>
constexpr Dictionary<key_t, value_t, size> Dictionary<key_t, value_t, size>::sort(void) const
{
    auto other = *this;

    for (size_t i = 0, k = 0; i < size - 1; i++)
    {
        k = i;
        for (size_t j = i + 1; j < size; j++)
        {
            if (other.elements[k].key > other.elements[j].key)
            {
                k = j;
            }
        }
        auto t = other.elements[i];
        other.elements[i] = other.elements[k];
        other.elements[k] = t;
    }

    return other;
}

template <typename key_t, typename value_t, size_t size>
constexpr Dictionary<key_t, value_t, size> Dictionary<key_t, value_t, size>::inv(void) const
{
    Dictionary<key_t, value_t, size> other;

    for (size_t i = 0; i < size; i++)
    {
        other.elements[i].key = this->elements[i].key;
        for (size_t j = 0; j < 30; j++)
        {
            other.elements[i].value[j] = ~this->elements[i].value[j];
        }
    }

    return other;
}

/**
 * @brief ChineseFontDictionary 的一个空实例
 * 
 */
template <typename key_t, typename value_t>
static inline constexpr auto __empty_chinese_font__ = Dictionary<key_t, value_t, 0>();

#endif // !__CHINESE_H__
