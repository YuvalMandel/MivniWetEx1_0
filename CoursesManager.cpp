//
// Created by Yuval.M on 04/12/2020.
//
#include "CoursesManager.h"

void* CoursesManager::Init(){

    CoursesManager* cm = new CoursesManager;

    return (void*)cm;

}


StatusType CoursesManager::AddCourse (int courseID, int numOfClasses) {

    Course c;

    c.course_id = courseID;

    Lecture new_lectures[numOfClasses];

    // Go one by one and create lectures by id.
    for (int i = 1; i < numOfClasses + 1; ++i) {

        Lecture l;

        l.lecture_id = i;
        l.watch_num = 0;
        l.holder_sub_tree_course = nullptr;

        // Give pointer to course lectures in the right place.
        new_lectures[i - 1] = l;

    }

    // If smallest time is not 0, create it.
    if(this -> smallest_time_tree == nullptr){
        // create time tree 0.
    } else if(this -> smallest_time_tree -> time_watched > 0){
        // create time tree 0.
    }



    // Create subtree course from course lectures and insert
    SubTreeCourse stc(numOfClasses, new_lectures, (void*)this ->
    smallest_time_tree);

    // Go to each lecture one by one and add holder_sub_tree_course, and
    // put pointer in lectures field in course array.

    this -> course_tree.Insert(c);

}