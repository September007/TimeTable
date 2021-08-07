#include "pch.h"
#include"../TimeTable/type.hpp"
using pic = pair<int, char>;
using pii = pair<int, int>;
template<Container T>
int t_cont(T t) { return 1; }
template<nonContainer T>
int t_cont(T t) { return 0; }
TEST(type, Container) {
	using T = vector<int>;
	T::value_type;
	T::size_type;
	T::allocator_type;
	T::iterator;
	T::const_iterator;
	EXPECT_EQ(t_cont(1), 0);
	EXPECT_EQ(t_cont(1.0), 0);
	EXPECT_EQ(t_cont(123456789876), 0);
	EXPECT_EQ(t_cont('a'), 0);
	EXPECT_EQ(t_cont(vector<int>()), 1);
	EXPECT_EQ(t_cont(map<int,char>()), 1);
}

TEST(type,SequenceContainer)
{
	//EXPECT_EQ(false, SequenceContainer<int>);
	//EXPECT_EQ(false, SequenceContainer<int>);
}


TEST(type, is_pair_type) {
	EXPECT_EQ(is_pair_type<int>, false);
	EXPECT_EQ(is_pair_type<bool> , false);
	EXPECT_EQ(is_pair_type<char>, false);
	EXPECT_EQ(is_pair_type<float>, false);
	EXPECT_EQ(is_pair_type<float[]>, false);
	EXPECT_EQ(is_pair_type<string>, false);
	EXPECT_EQ(is_pair_type<pic>, true);
	EXPECT_EQ(is_pair_type<pii>, true);
}

TEST(type, is_built_in) {
	EXPECT_EQ(is_built_in_type<int>, true);
	EXPECT_EQ(is_built_in_type<float>, true);
	EXPECT_EQ(is_built_in_type<char>, true);
	EXPECT_EQ(is_built_in_type<int[]>, false);
	EXPECT_EQ(is_built_in_type<string>, false);
	EXPECT_EQ(is_built_in_type<pic>, false);
}