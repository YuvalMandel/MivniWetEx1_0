//
// Created by Yuval.M on 04/12/2020.
//

#ifndef MIVNIWETEX1_0_COURSESMANAGER_H
#define MIVNIWETEX1_0_COURSESMANAGER_H

#include <iostream>
#include "library.h"

class CoursesManager {
private:

    // This is the root of the course tree.


    // This is an internal pointer to the largest time tree.

    // This is an internal pointer to the smallest time tree.


public:

    void *Init();

    StatusType AddCourse (int courseID, int numOfClasses);

    StatusType RemoveCourse(int courseID);

    StatusType WatchClass(int courseID, int classID, int time);

    StatusType TimeViewed(int courseID, int classID, int *timeViewed);

    StatusType GetMostViewedClasses(int numOfClasses, int *courses, int *classes);

};

#endif //MIVNIWETEX1_0_COURSESMANAGER_H
