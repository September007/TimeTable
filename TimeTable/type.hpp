//define some assistant concept and template boolean
//PS
//concept solve a problem, that is
//if i want specify condiftion:x by sfine
//how could i specify {1-confition:x}.like int:specify and non-int:specify
#pragma once
#include"inc.hpp"
template<typename T>
concept Container = requires(T x) {
	typename T::value_type;
	typename T::size_type;
	typename T::allocator_type;
	typename T::iterator;
	typename T::const_iterator;
	//{x} noexcept -> same_as<T>;
} && !is_same_v<decay_t<T>, string>;
template<typename T>
concept nonContainer = !(requires(T x) {
	typename T::value_type;
	typename T::size_type;
	typename T::allocator_type;
	typename T::iterator;
	typename T::const_iterator;
	//{x} noexcept -> same_as<T>;
} && !is_same_v<decay_t<T>, string>);

template<template<typename ...rest> typename T,typename ...rest>
concept SequenceContainer = Container<T<rest...>> && (is_same_v<decay_t<T<rest...>>, vector<rest...>>|| is_same_v<decay_t<T<rest...>>, set<rest...>>);
//non class,non array,non union
template<typename T>
constexpr bool is_built_in_type = /*is_pod_v<T> && */!is_union_v<T> && !is_class_v<T>&&!is_array_v<T>;

template<typename T>
concept pair_concept = requires(T t) {
	typename T::first_type;
	typename T::second_type;
	//decltype(x.first);
	//decltype(x.second);
};
template<typename T>
constexpr bool is_pair_type = pair_concept<T>;

template<typename T>
constexpr bool is_string_type = is_same_v<string, T> && is_same_v<T, string>;

template<typename T>requires nonContainer<T>
	constexpr bool is_container() { return false; }

template<typename T>requires Container<T>
	constexpr bool is_container() { return true; }
