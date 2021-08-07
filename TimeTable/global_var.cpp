//this file define all those global static var
#pragma once
#include"course_define.hpp"
#define rep_path "data/286885/"
task_lib static_var_initer;

using vectorCourse = vector<course>;
using vectorTable = vector<course_table_one_day>;
#define here_register(type,name) __auto_udf_register(type,name,,rep_path#name".ini")

here_register(vectorCourse, all_course);
here_register(vectorTable, all_table);
