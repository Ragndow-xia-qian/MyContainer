/**
  ******************************************************************************
  * @file           : Any.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/7/4
  ******************************************************************************
  */

#ifndef MYCONTAINER_ANY_H
#define MYCONTAINER_ANY_H

#include <vector>
#include <ostream>
#include <type_traits>
#include <utility>

namespace XQ::Tools {

    class Any {
    public:
        Any() = default;

        ~Any() {
            delete _ptr;
        }

        template<typename T>
        Any(T &&t): _ptr(new Data<T>(std::forward<T>(t))) {}

        Any(const Any &rhs) {
            _ptr = rhs._ptr->clone();
        }

        Any &operator=(const Any &rhs) {
            delete _ptr;
            _ptr = rhs._ptr->clone();
            return *this;
        }

        Any(Any &&rhs) noexcept {
            _ptr = rhs._ptr;
            rhs._ptr = nullptr;
        }

        Any &operator=(Any &&rhs) noexcept {
            delete _ptr;
            _ptr = rhs._ptr;
            rhs._ptr = 0;
            return *this;
        }

        template<typename T>
        T &get_data() {
            return dynamic_cast<Data <T> *>(_ptr)->data;
        }

    private:
        struct Base {
            virtual ~Base() = default;

            virtual Base *clone() = 0;

            virtual std::ostream &print(std::ostream &) = 0;
        };

        template<typename T>
        struct Data : Base {
            T data;

            template<typename F, typename = std::enable_if_t<std::is_same_v<T, std::decay_t<F>>>>
            Data(F &&t) : data(std::forward<F>(t)) {}

            ~Data() override {
                data.~T();
            }

            Base *clone() override {
                return new Data<T>(data);
            }

            std::ostream &print(std::ostream &os) override {
                os << data;
                return os;
            }
        };

        Base *_ptr;

        friend std::ostream &operator<<(std::ostream &os, const Any &a) {
            return a._ptr->print(os);
        }
    };
} // XQ::Tools

#endif //MYCONTAINER_ANY_H
