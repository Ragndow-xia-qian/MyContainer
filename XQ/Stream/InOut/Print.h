/**
  ******************************************************************************
  * @file           : InOut.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/8
  ******************************************************************************
  */
#ifndef MYCONTAINER_PRINT_H
#define MYCONTAINER_PRINT_H

#include <iostream>
#include <ranges>
#include <format>
#include <locale>

template<typename Container>
concept HasIterator = requires(Container c) {
    std::ranges::begin(c);
    std::ranges::end(c);
};

template<HasIterator Container>
struct std::formatter<Container, char> {
    std::formatter<std::ranges::range_value_t<Container>, char> formatter;

    constexpr typename std::basic_format_parse_context<char>::iterator
    parse(std::basic_format_parse_context<char> &pc) {
        return formatter.parse(pc);
    }

    template<class Out>
    constexpr typename std::basic_format_context<Out, char>::iterator
    format(Container value, std::basic_format_context<Out, char> &fc) const {
        *fc.out() = '[';
        for (auto it = std::ranges::begin(value); it != std::ranges::end(value); it++) {
            if (it != std::ranges::begin(value)) {
                *fc.out() = ',';
            }
            formatter.format(*it, fc);
        }
        *fc.out() = ']';
        return ++fc.out();
    }
};

namespace XQ::Stream::InOut {
    template<typename ...Args>
    void Print(std::format_string<Args...> fmt, const Args &... args) {
        std::cout << std::vformat(fmt.get(), std::make_format_args(args...));
    }

    template<typename ...Args>
    void Println(std::format_string<Args...> fmt, const Args &... args) {
        std::cout << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename ...Args>
    void Print(const std::locale &loc, std::format_string<Args...> fmt, const Args &... args) {
        std::cout << std::vformat(loc, fmt.get(), std::make_format_args(args...));
    }

    template<typename ...Args>
    void Println(const std::locale &loc, std::format_string<Args...> fmt, const Args &... args) {
        std::cout << std::vformat(loc, fmt.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename ...Args>
    void Print(const std::ostream_iterator<char> &file, std::format_string<Args...> fmt, const Args &... args) {
        std::vformat_to(file, fmt.get(), std::make_format_args(args...));
    }

    template<typename ...Args>
    void Println(const std::ostream_iterator<char> &file, std::format_string<Args...> fmt, const Args &... args) {
        std::vformat_to(file, fmt.get() + "{}", std::make_format_args(args..., '\n'));
    }

    template<typename ...Args>
    void Print(const std::ostream_iterator<char> &file, const std::locale &loc, std::format_string<Args...> fmt,
               const Args &... args) {
        std::vformat_to(file, loc, fmt.get(), std::make_format_args(args...));
    }

    template<typename ...Args>
    void Println(const std::ostream_iterator<char> &file, const std::locale &loc, std::format_string<Args...> fmt,
                 const Args &... args) {
        std::vformat_to(file, loc, fmt.get() + "{}", std::make_format_args(args..., '\n'));
    }
}
// XQ::Stream::InOut

#endif //MYCONTAINER_PRINT_H
