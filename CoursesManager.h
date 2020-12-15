//
// Created by Yuval.M on 04/12/2020.
//

#ifndef MIVNIWETEX1_0_COURSESMANAGER_H
#define MIVNIWETEX1_0_COURSESMANAGER_H

#include <iostream>
#include "library.h"
#include "AVL.h"

class Lecture{

public:

    int lecture_id;
    int watch_num;
    void* holder_sub_tree_course;

};

class SubTreeCourse{

public:

    SubTreeCourse() = default;
    SubTreeCourse(int course_id, Lecture* lectures, void* holder_time_tree);
    int course_id;
    AVLTree<Lecture> lectures_tree;
    void* holder_time_tree;

};

class TimeTree{

public:
    int time_watched;
    AVLTree<SubTreeCourse> subtree_tree;
    TimeTree* bigger;
    TimeTree* smaller;

};

class Course{

public:
    int lectures_num;
    int course_id;
    Lecture** lectures;

};

class CoursesManager {
private:

    // This is the root of the course tree.
    AVLTree<Course> course_tree;

    // This is an internal pointer to the largest time tree.
    TimeTree* largest_time_tree;

    // This is an internal pointer to the smallest time tree.
    TimeTree* smallest_time_tree;


public:

    void *Init();

    StatusType AddCourse (int courseID, int numOfClasses);

    StatusType RemoveCourse(int courseID);

    StatusType WatchClass(int courseID, int classID, int time);

    StatusType TimeViewed(int courseID, int classID, int *timeViewed);

    StatusType GetMostViewedClasses(int numOfClasses, int *courses, int *classes);

};

#endif //MIVNIWETEX1_0_COURSESMANAGER_H
