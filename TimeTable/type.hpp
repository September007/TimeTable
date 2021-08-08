//define some assistant concept and template boolean
#pragma once
#include"inc.hpp"
#include<concepts>

template<typename T>concept built_in_non_array_type
=/*is_pod_v<T> && */!is_union_v<T> && !is_class_v<T> && !is_array_v<T>;

template<typename T>concept imple_recover_store
= requires(T t)
{
	{T::recover(declval<stringstream&>())}->same_as<T>;
	{T::store(declval<stringstream&>(), declval<T>())}->same_as<stringstream&>;
};

template<typename T>concept Container
= requires(T x) {
	typename T::value_type;
	typename T::size_type;
	typename T::allocator_type;
	typename T::iterator;
	typename T::const_iterator;
	//requires块子句是编译约束，不是真值约束
}&&!is_same_v<decay_t<T>, string>;

template<typename T>concept nonContainer
= !Container<T>;

template<typename T>concept pair_type
= requires(T t) {
	typename T::first_type;
	typename T::second_type;
	is_same_v<typename T::first_type, decltype(t.first)>;
	is_same_v<typename T::second_type, decltype(t.second)>;
};

template<typename T>concept string_type
= same_as<string, decay_t<T>>;



////non class,non array,non union
//template<typename T>constexpr bool is_built_in_type
//= /*is_pod_v<T> && */!is_union_v<T> && !is_class_v<T>&&!is_array_v<T>;
//
//template<typename T>
//constexpr bool is_pair_type = pair_type<T>;
//
//template<typename T>
//constexpr bool is_string_type = is_same_v<string, T> && is_same_v<T, string>;
