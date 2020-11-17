#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>

template<typename T,
        typename IteratorType = typename T::iterator,
        typename BeginType = decltype(std::declval<T>().begin()),
        typename EndType = decltype(std::declval<T>().end()),
        typename = typename std::enable_if<std::is_same<IteratorType, BeginType>::value, void>::type,
        typename = typename std::enable_if<std::is_same<IteratorType, EndType>::value, void>::type>
struct IteratableTriats
{
    static constexpr bool value = true;
};

template<typename T>
typename std::enable_if<IteratableTriats<T>::value, std::string>::type
print_ip(T const& ip)
{
    std::stringstream ss;
    for (typename T::const_iterator I = ip.begin(); I != ip.end(); ++I)
    {
        if (I != ip.begin())
            ss << ".";
        ss << static_cast<int>(*I);
    }
    return ss.str();
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
print_ip(T ip)
{
    constexpr size_t totalBytes = sizeof(ip);
    std::vector<uint8_t> bytes;
    bytes.reserve(totalBytes);
    if (totalBytes > 1) {
        for (size_t i = 0; i < totalBytes; ++i) {
            bytes.push_back(ip & 0xFF);
            ip = ip >> 8;
        }
        std::reverse(bytes.begin(), bytes.end());
    } else {
        bytes.push_back(ip);
    }
    return print_ip(bytes);
}

std::string print_ip(std::string const& ip)
{
    return ip;
}

template<typename... T>
struct GetHead;

template<typename T>
struct GetHead<T> { using type = T; };

template<typename Head, typename... Tail>
struct GetHead<Head, Tail...> { using type = Head; };

template<typename... T>
struct IsHomogeneous;

template<typename T>
struct IsHomogeneous<T>
{
    constexpr static bool value = true;
};

template<typename T, typename S>
struct IsHomogeneous<T, S>
{
    constexpr static bool value = std::is_same<T, S>::value;
};

template<typename T, typename... Tail>
struct IsHomogeneous<T, Tail...>
{
    using Head = typename GetHead<Tail...>::type;
    constexpr static bool value =
            IsHomogeneous<T, Head>::value && IsHomogeneous<Tail...>::value;
};

template<class Tuple, std::size_t TupleSize>
struct TuplePrinter;

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(std::ostream& out, Tuple const& tuple)
    {
        out << std::get<0>(tuple);
    }
};

template<class Tuple, std::size_t TupleSize>
struct TuplePrinter {
    static void print(std::ostream& out, Tuple const& tuple)
    {
        TuplePrinter<Tuple, TupleSize - 1>::print(out, tuple);
        out << "." << std::get<TupleSize - 1>(tuple);
    }
};

template<typename... T>
typename std::enable_if<IsHomogeneous<T...>::value, std::string>::type
print_ip(std::tuple<T...> const& ip)
{
    std::stringstream ss;
    TuplePrinter<std::tuple<T...>, sizeof...(T)>::print(ss, ip);
    return ss.str();
}