#pragma once
#include"inc.hpp"
template<typename T>
constexpr bool is_built_in_type = is_pod_v<T> && !is_union<T> && !is_class_v<T>;

template<typename T>
constexpr bool is_pair = requires(T x) {
	typename T::first_type;
	typename T::second_type;
	decltype(x.first);
	decltype(x.second);
};

template<typename T>
constexpr bool is_string = is_same_v<string, T> && is_same_v<T, string>;

template<typename T>
concept Container = requires(T x) {
	typename T::value_type;
	typename T::size_type;
	typename T::allocator_type;
	typename T::iterator;
	typename T::const_iterator;
	{x} noexcept -> same_as<T>;
};
template<typename T>
concept nonContainer = !requires(T x) {
	typename T::value_type;
	typename T::size_type;
	typename T::allocator_type;
	typename T::iterator;
	typename T::const_iterator;
	{x} noexcept -> same_as<T>;
};

template<typename T>requires nonContainer<T>
	constexpr bool is_container() { return false; }

template<typename T>requires Container<T>
	constexpr bool is_container() { return true; }
