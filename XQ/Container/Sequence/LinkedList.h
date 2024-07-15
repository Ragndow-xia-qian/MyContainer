/**
  ******************************************************************************
  * @file           : LinkedList.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/2/21
  ******************************************************************************
  */
#ifndef MYCONTAINER_LINKEDLIST_H
#define MYCONTAINER_LINKEDLIST_H

// 未完工

#include <initializer_list>
#include <new>
#include <utility>
#include <execution>
#include <type_traits>

#include "../../Memory/Allocator.h"

namespace XQ::Container::Sequence {

    template<typename Tp, typename Alloc = XQ::Memory::Allocator<Tp>>
    class LinkedList {
    public:
        using value_type = Tp;
        using reference = Tp &;
        using const_reference = const Tp &;
#if defined(_WIN64) or defined(__x86_64__)
        using size_type = signed long long;
#else
        using size_type = signed int;
#endif
        using allocator_type = Alloc;
        using self_type = LinkedList<Tp, Alloc>;

    private:
        struct node {
            value_type val;
            node *nxt{nullptr};
            node *lst{nullptr};

            template<typename T>
            requires std::is_same_v<std::decay_t<T>, value_type>
            explicit node(T &&val, node *nxt = nullptr, node *lst = nullptr) :
                    val(std::forward<T>(val)), nxt(nxt), lst(lst) {}

            node(const node &other) = delete;
            node(node &&other) noexcept = delete;
        };

        template<typename node>
        struct LinkedListIterator {
        public:
            explicit LinkedListIterator(node *ptr) : _ptr(ptr) {}

            LinkedListIterator(const LinkedListIterator &other) : _ptr(other._ptr) {}

            LinkedListIterator &operator=(const LinkedListIterator &other) {
                if (&other == this) {
                    return *this;
                }
                _ptr = other._ptr;
                return *this;
            }

            bool operator==(const LinkedListIterator &other) const {
                return _ptr == other._ptr;
            }

            bool operator!=(const LinkedListIterator &other) const {
                return _ptr != other._ptr;
            }

            reference operator*() const {
                return _ptr->val;
            }

            node *operator->() const {
                return &this->operator*();
            }

            LinkedListIterator &operator++() {
                _ptr = _ptr->nxt;
                return *this;
            }

            auto operator++(int) -> LinkedListIterator {
                LinkedListIterator tmp = *this;
                ++*this;
                return tmp;
            }

            LinkedListIterator &operator--() {
                _ptr = _ptr->lst;
                return *this;
            }

            auto operator--(int) -> LinkedListIterator {
                LinkedListIterator tmp = *this;
                --*this;
                return tmp;
            }

        private:
            node *_ptr;
        };

    public:
        using iterator = LinkedListIterator<node>;
        using const_iterator = LinkedListIterator<const node>;

        LinkedList() : _begin(nullptr) {}

        explicit LinkedList(size_type n, const_reference val) {
        }

        LinkedList(std::initializer_list<value_type> il) {

        }

        LinkedList(const LinkedList &other) {

        }

        LinkedList(LinkedList &&other) noexcept {

        }

        LinkedList &operator=(LinkedList other) {

        }

        ~LinkedList() {
            node *tmp = _begin;
            while (tmp->nxt != nullptr) {
                node *tmp2 = tmp->nxt;
                delete tmp;
                tmp = tmp2;
            }
            delete tmp;
        }

        iterator begin() {
            return iterator(_begin);
        }

        const_iterator begin() const {
            return const_iterator(_begin);
        }

        const_iterator cbegin() const {
            return const_iterator(_begin);
        }

        iterator end() {
            return iterator(_end);
        }

        const_iterator end() const {
            return const_iterator(_end);
        }

        const_iterator cend() const {
            return const_iterator(_end);
        }

        [[nodiscard]] bool empty() const {
            return _begin == _end;
        }

        template<typename ...Args>
        void back_append(Args &&...args) {
            if (_begin == nullptr) {
                _begin = new node(value_type{std::forward<Args>(args)...}, nullptr, _end);
            } else {
                _end = new node(value_type{std::forward<Args>(args)...}, _end->lst, _end);
                _end = _end->nxt;
            }
        }

        template<typename ...Args>
        void front_append(Args &&...args) {
            if (_begin == nullptr) {
                _begin = new node(value_type{std::forward<Args>(args)...}, nullptr, _end);
            } else {
                _begin->lst = new node(value_type{std::forward<Args>(args)...}, nullptr, _begin);
                _begin = _begin->lst;
            }
        }

    private:
        node *_begin;
        node *_end{nullptr};
        int size = 0;
        // null <-> 1 <-> 2 <-> 3 <-> ...... <-> n <-> null
        //          |                                   |
        //       _begin                               _end
        allocator_type alloc;
    };

} // XQ::Container::Sequence

#endif //MYCONTAINER_LINKEDLIST_H
