#include"library.h"
#include"CoursesManager.h"

void *Init() {
    CoursesManager *DS = new CoursesManager();
    return (void*)DS;
}
StatusType AddCourse(void *DS, int courseID, int numOfClasses) {
    return ((CoursesManager *)DS)-> AddCourse(courseID, numOfCourses);
}
