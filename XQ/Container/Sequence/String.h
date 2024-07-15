/**
******************************************************************************
* @file           : _String.h
* @author         : 夏浅
* @brief          : None
* @attention      : None
* @date           : 2024/3/22
******************************************************************************
*/
#ifndef MYCONTAINER_STRING_H
#define MYCONTAINER_STRING_H

// 未完工

#include <iterator>
#include <limits>
#include <cstring>

#include "../../Memory/Allocator.h"

namespace XQ::Container::Sequence {

    template<typename Alloc = XQ::Memory::Allocator<char>>
    class string_type {
    public:
        using value_type = char;
#if defined(_WIN64) or defined(__x86_64__)
        using size_type = signed long long;
#else
        using size_type = signed long;
#endif
        using iterator = char *;
        using const_iterator = const char *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reference = char &;
        using const_reference = const char &;
        using self_type = string_type<Alloc>;
        using allocator_type = Alloc;

        static constexpr size_type npos = std::numeric_limits<size_type>::max();

        explicit string_type(int siz = 0, char val = ' ') {
            if (siz > 23) {
                this->_data._short.size = 1;
                this->_data._long.cap = siz;
                this->_data._long.size = siz;
                this->_data._long.data = this->_alloc.allocate(siz);
                for (int i = 0; i < siz; ++i) {
                    this->_alloc.construct(this->_data._long.data + i, val);
                }
            } else {
                this->_data._short.size = siz << 1;
                for (int i = 0; i < siz; ++i) {
                    this->_data._short.data[i] = val;
                }
            }
        }

        explicit string_type(const char *str) {
            auto len = static_cast<size_type>(strlen(str));
            if (len > 23) {
                this->_data._short.size = 1;
                this->_data._long.cap = len;
                this->_data._long.size = len;
                this->_data._long.data = this->_alloc.allocate(len);
                for (int i = 0; i < len; ++i) {
                    this->_alloc.construct(this->_data._long.data + i, str[i]);
                }
            } else {
                this->_data._short.size = len << 1;
                for (int i = 0; i < len; ++i) {
                    this->_data._short.data[i] = str[i];
                }
            }
        }

        string_type(const string_type &str) {
            if (str._data._short.size & 1) {
                this->_data._short.size = 1;
                this->_data._long.cap = str._data._long.cap;
                this->_data._long.size = str._data._long.size;
                this->_data._long.data = this->_alloc.allocate(this->_data._long.cap);
                for (int i = 0; i < this->_data._long.size; ++i) {
                    this->_alloc.construct(this->_data._long.data + i, str._data._long.data[i]);
                }
            } else {
                this->_data._short.size = str._data._short.size;
                for (int i = 0; i < this->_data._short.size; ++i) {
                    this->_data._short.data[i] = str._data._short.data[i];
                }
            }
        }

        string_type(string_type &&str) noexcept {

        }

        ~string_type() {
            if (_data._short.size & 1) {
                _alloc.deallocate(_data._long.data, _data._long.cap);
            }
        }

    private:
        struct Alloc_hider : allocator_type {
        } _alloc;

        union {
            struct {
                unsigned char size;
                char data[23];
            } _short;

            struct {
                size_type cap;
                size_type size;
                iterator data;
            } _long;
        } _data;
    };

    using String = string_type<>;
} // XQ::Container::Sequence

#endif //MYCONTAINER_STRING_H

