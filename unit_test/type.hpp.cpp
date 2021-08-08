#include "pch.h"
#include"../TimeTable/type.hpp"
#include"../TimeTable/course_define.hpp"
using pic = pair<int, char>;
using pii = pair<int, int>;
template<Container T>
int t_cont(T t) { return 1; }
template<nonContainer T>
int t_cont(T t) { return 0; }
TEST(type, Container) {
	using T = vector<int>;
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
template<typename L,typename R>
struct _non_pair_type
{
	using first_type = L;
	using second_type = R;
	L l;
	R r;
};
using npii = _non_pair_type<int, int>;
TEST(type, pair_type) {
	EXPECT_EQ(pair_type<int>, false);
	EXPECT_EQ(pair_type<bool> , false);
	EXPECT_EQ(pair_type<char>, false);
	EXPECT_EQ(pair_type<float>, false);
	EXPECT_EQ(pair_type<float[]>, false);
	EXPECT_EQ(pair_type<string>, false);
	EXPECT_EQ(pair_type<npii>, false);
	EXPECT_EQ(pair_type<pic>, true);
	EXPECT_EQ(pair_type<pii>, true);
}

TEST(type, built_in_non_array_type) {
	EXPECT_EQ(built_in_non_array_type<int>, true);
	EXPECT_EQ(built_in_non_array_type<float>, true);
	EXPECT_EQ(built_in_non_array_type<char>, true);
	EXPECT_EQ(built_in_non_array_type<int[]>, false);
	EXPECT_EQ(built_in_non_array_type<string>, false);
	EXPECT_EQ(built_in_non_array_type<pic>, false);
}
struct _imple_recover_store
{
	static _imple_recover_store recover(stringstream&) { return _imple_recover_store(); };
	static stringstream& store(stringstream& ss, const _imple_recover_store&) { return ss; };
};
struct __nonimple_recover_store{};
TEST(type,imple_recover_store)
{
	EXPECT_EQ(imple_recover_store<int>, false);
	EXPECT_EQ(imple_recover_store<lesson_table_one_day>, true);
	EXPECT_EQ(imple_recover_store<course>, true);
	EXPECT_EQ(imple_recover_store<_imple_recover_store>, true);
	EXPECT_EQ(imple_recover_store<__nonimple_recover_store>, false);
}