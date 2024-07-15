/**
  ******************************************************************************
  * @file           : ArrayList.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/2/20
  ******************************************************************************
  */
#ifndef MYCONTAINER_ARRAYLIST_H
#define MYCONTAINER_ARRAYLIST_H

#include <initializer_list>
#include <utility>
#include <iterator>
#include <limits>

#include "../../Memory/Allocator.h"

namespace XQ::Container::Sequence {

    template<typename Tp, typename Alloc = XQ::Memory::Allocator<Tp>> requires XQ::Memory::Alloc<Alloc>
    class ArrayList {
    public:
        using value_type = Tp;
#if defined(_WIN64) or defined(__x86_64__)
        using size_type = signed long long;
#else
        using size_type = signed long;
#endif
        using iterator = Tp *;
        using const_iterator = const Tp *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reference = Tp &;
        using const_reference = const Tp &;
        using self_type = ArrayList<Tp, Alloc>;
        using allocator_type = Alloc;

        static constexpr size_type npos = std::numeric_limits<size_type>::max();

        constexpr ArrayList() : _begin(nullptr), _end(nullptr), _illegal(nullptr) {}

        constexpr explicit ArrayList(size_type n, const value_type &val = value_type{}) {
            this->_begin = this->_end = this->_illegal = nullptr;
            this->resize(n, val);
        }

        constexpr ArrayList(std::initializer_list<value_type> list) {
            this->_begin = this->_end = this->_illegal = nullptr;
            for (auto &&item: list) {
                this->append(item);
            }
        }

        constexpr ArrayList(const self_type &oth) {
            this->_begin = this->_end = this->_illegal = nullptr;
            for (int i = 0; i < oth._end - oth._begin; ++i) {
                this->append(oth._begin[i]);
            }
        }

        constexpr ArrayList(self_type &&oth) noexcept {
            this->_begin = oth._begin;
            this->_end = oth._end;
            this->_illegal = oth._illegal;
            oth._begin = nullptr;
            oth._end = nullptr;
            oth._illegal = nullptr;
        }

        constexpr ~ArrayList() {
            this->_alloc.destroy(this->_begin, this->_end - this->_begin);
            this->_alloc.deallocate(this->_begin, this->_end - this->_begin);
        }

        constexpr self_type &operator=(self_type oth) {
            this->_alloc.deallocate(this->_begin, this->_end - this->_begin);
            this->_begin = oth._begin;
            this->_end = oth._end;
            this->_illegal = oth._illegal;
            oth._begin = nullptr;
            oth._end = nullptr;
            oth._illegal = nullptr;
            return *this;
        }

        constexpr void reserve(size_type n) {
            auto *tmp = this->_alloc.allocate(n);
            size_type siz = 0;
            size_type siz2 = 0;
            if (_begin) {
                siz = this->_end - this->_begin;
                siz2 = this->_illegal - this->_begin;
                for (size_type i = 0; i < (n < siz ? n : siz); ++i) {
                    this->_alloc.construct(tmp + i, std::move(this->_begin[i]));
                }
            }
            this->_alloc.deallocate(this->_begin, siz2);
            _begin = tmp;
            _end = this->_begin + (n < siz ? n : siz);
            _illegal = this->_begin + n;
        }

        constexpr void resize(size_type n, const value_type &val = value_type{}) {
            size_type siz = this->_end - this->_begin;
            this->reserve(n);
            for (size_type i = siz; i < n; ++i) {
                this->_alloc.construct(this->_begin + i, val);
            }
            this->_end = this->_illegal;
        }

        template<typename ...Args>
        constexpr void append(Args &&...args) {
            if (_end == this->_illegal) {
                size_type siz = this->_end - this->_begin;
                this->reserve(siz == 0 ? 1 : siz * 2);
            }
            this->_alloc.construct(_end, std::forward<Args>(args)...);
            ++_end;
        }

        constexpr void append(std::initializer_list<value_type> l) {
            for (auto &&item: l) {
                this->append(item);
            }
        }

        constexpr void pop() {
            if (this->_begin == this->_end) {
                throw std::runtime_error("Empty container can't pop");
            }
            this->resize(this->size() - 1);
        }

        [[nodiscard]] constexpr size_type size() {
            return this->_end - this->_begin;
        }

        [[nodiscard]] constexpr size_type capacity() {
            return this->_illegal - this->_begin;
        }

        [[nodiscard]] constexpr bool empty() {
            return this->_begin == this->_end;
        }

        [[nodiscard]] constexpr iterator begin() {
            return this->_begin;
        }

        [[nodiscard]] constexpr const_iterator begin() const {
            return this->_begin;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const {
            return this->_begin;
        }

        [[nodiscard]] constexpr iterator end() {
            return this->_end;
        }

        [[nodiscard]] constexpr const_iterator end() const {
            return this->_end;
        }

        [[nodiscard]] constexpr const_iterator cend() const {
            return this->_end;
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() {
            return this->_end;
        }

        [[nodiscard]] constexpr const_reverse_iterator rbegin() const {
            return this->_end;
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const {
            return this->_end;
        }

        [[nodiscard]] constexpr reverse_iterator rend() {
            return this->_begin;
        }

        [[nodiscard]] constexpr const_reverse_iterator rend() const {
            return this->_begin;
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const {
            return this->_begin;
        }

        [[nodiscard]] constexpr reference operator[](size_type idx) {
            size_type siz = this->_end - this->_begin;
            idx %= siz;
            idx += siz;
            idx %= siz;
            return this->_begin[idx];
        }

        [[nodiscard]] constexpr const_reference operator[](size_type idx) const {
            return const_cast<self_type *>(this)->operator[](idx);
        }

        [[nodiscard]] constexpr self_type operator[](size_type left, size_type right, size_type step = 1) const {
            if (step == 0) {
                throw std::runtime_error{"slice step cannot be zero"};
            }
            if (step < 0) {
                std::swap(left, right);
            }
            if (this->_end == this->_begin) {
                if (left != npos || right != npos) {
                    throw std::runtime_error{"list index out of range"};
                }
                return {};
            }
            int siz = this->_end - this->_begin;
            self_type tmp;
            if (left != npos && right != npos && left > right) {
                return tmp;
            }
            size_type pos = 0;
            if (left == npos && right == npos) {
                tmp = *this;
            } else if (left == npos) {
                right %= siz;
                right += siz;
                right %= siz;
                while (pos != right) {
                    tmp.append(this->_begin[pos]);
                    ++pos;
                }
            } else if (right == npos) {
                left %= siz;
                left += siz;
                left %= siz;
                pos = left;
                while (pos != siz) {
                    tmp.append(this->_begin[pos]);
                    ++pos;
                }
            } else {
                left %= siz;
                left += siz;
                left %= siz;
                right %= siz;
                right += siz;
                right %= siz;
                pos = left;
                while (pos != right) {
                    tmp.append(this->_begin[pos]);
                    ++pos;
                }
            }
            self_type res;
            siz = tmp._end - tmp._begin;
            if (step > 0) {
                pos = 0;
                while (pos < siz) {
                    res.append(tmp._begin[pos]);
                    pos += step;
                }
            }
            if (step < 0) {
                pos = siz - 1;
                while (pos >= 0) {
                    res.append(tmp._begin[pos]);
                    pos += step;
                }
            }
            return res;
        }

    private:
        iterator _begin;
        iterator _end;
        iterator _illegal;

        allocator_type _alloc;
    };

} // XQ::Container::Sequence

#endif //MYCONTAINER_ARRAYLIST_H
