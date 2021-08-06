#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<unordered_map>
#include<set>
#include<functional>
#include<mutex>
//#include<format>
#include<type_traits>
using namespace std;

#define _assert(flag) if(!(flag))throw 1;
//stringstream Á÷µÄ×ª»»
inline stringstream& ssOS2SS(ostream & ssOS) {
	auto* p = (stringstream*)&ssOS;
	return *p;
}
