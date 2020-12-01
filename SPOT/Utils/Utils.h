#pragma once
#include<fstream>
#include<iostream>
#include<vector>
#include"../Rules.h"
#include"../GUI/Drawable.h"

using namespace std;

vector<CourseInfo> createAllCoursesVector(string fileName);
void addExtraSpace(Course_Code& code);
//graphicsInfo** coursesGridArray();