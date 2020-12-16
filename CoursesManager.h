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

    friend bool operator<(const Lecture& c1, const Lecture& c2);
    friend bool operator>(const Lecture& c1, const Lecture& c2);
    friend bool operator==(const Lecture& c1, const Lecture& c2);
    friend bool operator<=(const Lecture& c1, const Lecture& c2);
    friend bool operator>=(const Lecture& c1, const Lecture& c2);

};

class SubTreeCourse{

public:

    SubTreeCourse() = default;
    SubTreeCourse(int course_id, Lecture* lectures, int lectures_num, void*
    holder_time_tree) :
    course_id(course_id), lectures_tree(lectures, lectures_num),
    holder_time_tree(holder_time_tree){}
    int course_id;
    AVLTree<Lecture> lectures_tree;
    void* holder_time_tree;

    friend bool operator<(const SubTreeCourse& c1, const SubTreeCourse& c2);
    friend bool operator>(const SubTreeCourse& c1, const SubTreeCourse& c2);
    friend bool operator==(const SubTreeCourse& c1, const SubTreeCourse& c2);
    friend bool operator<=(const SubTreeCourse& c1, const SubTreeCourse& c2);
    friend bool operator>=(const SubTreeCourse& c1, const SubTreeCourse& c2);

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

    ~Course();
    int lectures_num;
    int course_id;
    Lecture** lectures;

    friend bool operator<(const Course& c1, const Course& c2);
    friend bool operator>(const Course& c1, const Course& c2);
    friend bool operator==(const Course& c1, const Course& c2);
    friend bool operator<=(const Course& c1, const Course& c2);
    friend bool operator>=(const Course& c1, const Course& c2);

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

    ~CoursesManager();

    void AddCourse (int courseID, int numOfClasses);

    void RemoveCourse(int courseID);

    void WatchClass(int courseID, int classID, int time);

    void TimeViewed(int courseID, int classID, int *timeViewed);

    void GetMostViewedClasses(int numOfClasses, int *courses, int *classes);


};

#endif //MIVNIWETEX1_0_COURSESMANAGER_H
