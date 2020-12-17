//
// Created by Yuval.M on 04/12/2020.
//
#include "CoursesManager.h"


void update_inorder_pointers(AVLNode<Lecture> *avl_node, Lecture** arr,
                             SubTreeCourse* stc_ptr);
int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt_ptr );
int stc_inorder(int numOfClasses, int *courses, int *classes, AVLNode<SubTreeCourse> *stc_node);
int lectures_inorder(int numOfClasses, int *courses, int *classes, AVLNode<Lecture> *lecture_node);

void CoursesManager::AddCourse (int courseID, int numOfClasses) {

    Course c(courseID, numOfClasses);

    if(this -> course_tree.FindValue(c) != nullptr){
        throw std::invalid_argument("FAILURE");
    }

    Lecture new_lectures[numOfClasses];

    // Go one by one and create lectures by id.
    for (int i = 0; i < numOfClasses; ++i) {

        Lecture l;

        l.lecture_id = i;
        l.watch_num = 0;
        l.holder_sub_tree_course = nullptr;

        // Give pointer to course lectures in the right place.
        new_lectures[i] = l;

    }

    TimeTree* tt_ptr;

    // If smallest time is not 0, create it.
    if(this -> smallest_time_tree == nullptr){

        // create time tree 0
        try {
            tt_ptr = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        tt_ptr -> time_watched = 0;
        tt_ptr -> bigger = nullptr;
        tt_ptr -> smaller = nullptr;
        this -> smallest_time_tree = tt_ptr;
        this -> largest_time_tree = tt_ptr;
    } else if(this -> smallest_time_tree -> time_watched > 0){
        // create time tree 0.

        try {
            tt_ptr = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        tt_ptr -> time_watched = 0;
        tt_ptr -> bigger = this -> smallest_time_tree;
        tt_ptr -> bigger -> smaller = tt_ptr;
        tt_ptr -> smaller = nullptr;
        this -> smallest_time_tree = tt_ptr;
    }

    // Create subtree course from course lectures and insert
    SubTreeCourse stc(courseID, new_lectures, numOfClasses, (void*)this ->
    smallest_time_tree);

//    try {
//        c.lectures = new Lecture*[c.lectures_num];
//    }
//    catch(std::bad_alloc&) {
//        throw std::invalid_argument("ALLOCATION_ERROR");
//    }

    // Go to each lecture one by one and add holder_sub_tree_course, and
    // put pointer in lectures field in course array.
    update_inorder_pointers(stc.lectures_tree.root, c.lectures, &stc);

    // Add stc to tt.
    SubTreeCourse* inserted_stc_ptr = this -> smallest_time_tree -> subtree_tree
            .Insert(stc);

    update_inorder_pointers(inserted_stc_ptr -> lectures_tree.root, c.lectures,
                            inserted_stc_ptr);

    stc.lectures_tree.root = nullptr;

    this -> course_tree.Insert(c);

}


void CoursesManager::RemoveCourse(int courseID){

	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

	Course c = course_node -> val;

	for(int i=0; i < c.lectures_num; i++){

		SubTreeCourse* stc_ptr = (SubTreeCourse*)(c.lectures[i] ->
		        holder_sub_tree_course);

		stc_ptr ->lectures_tree.Remove(*c.lectures[i]);

		// If stc is empty, remove it from the time tree.
		if(stc_ptr->lectures_tree.root == nullptr){

			TimeTree* tt_ptr = (TimeTree*)stc_ptr -> holder_time_tree;

			tt_ptr -> subtree_tree.Remove(*stc_ptr);

			if(tt_ptr->subtree_tree.root == nullptr){

				if(tt_ptr -> bigger != nullptr){
                    tt_ptr -> bigger -> smaller = tt_ptr -> smaller;
				} else{
					this -> largest_time_tree = tt_ptr -> smaller;
				}

				if(tt_ptr -> smaller != nullptr){
                    tt_ptr -> smaller -> bigger = tt_ptr -> bigger;
				} else{
					this -> smallest_time_tree = tt_ptr -> bigger;
				}

				delete tt_ptr;
			
			}
		}

	}


	this -> course_tree.Remove(c);

}

void CoursesManager::WatchClass(int courseID, int classID, int time){
	
	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

    if(course_node ->val.lectures_num < classID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

	Lecture* lecture_ptr = (course_node -> val).lectures[classID];
	Lecture lecture =  *lecture_ptr;
	SubTreeCourse* stc_ptr = (SubTreeCourse*)lecture_ptr -> holder_sub_tree_course;
	stc_ptr -> lectures_tree.Remove(lecture); //TODO

	TimeTree* original_tt_ptr = (TimeTree*)stc_ptr -> holder_time_tree;

	TimeTree* current_tt_ptr = original_tt_ptr;

	lecture.watch_num = original_tt_ptr -> time_watched + time;

	while(current_tt_ptr -> bigger != nullptr){
	    if(current_tt_ptr -> bigger -> time_watched < original_tt_ptr ->
	    time_watched + time){
	        break;
	    }
        current_tt_ptr = current_tt_ptr -> bigger;
	}

	if(current_tt_ptr -> time_watched == original_tt_ptr -> time_watched + time){
		AVLNode<SubTreeCourse>* current_stc_node = current_tt_ptr -> subtree_tree.FindValue(*stc_ptr);
		if (current_stc_node){
			lecture.holder_sub_tree_course = &(current_stc_node -> val);
            (course_node -> val).lectures[classID] =
                    current_stc_node -> val.lectures_tree.Insert(lecture);
		}else{
			SubTreeCourse new_stc;
			new_stc.holder_time_tree = current_tt_ptr;
			new_stc.course_id = courseID;
            (course_node -> val).lectures[classID] = new_stc.lectures_tree.Insert(lecture);
            (course_node -> val).lectures[classID] -> holder_sub_tree_course = current_tt_ptr ->
			        subtree_tree.Insert(new_stc);
            new_stc.lectures_tree.root = nullptr;
		}
	}else{

        TimeTree* new_tt_ptr;

        try {
            new_tt_ptr = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        new_tt_ptr -> time_watched = lecture.watch_num;
        new_tt_ptr -> smaller = current_tt_ptr;
        new_tt_ptr -> bigger = current_tt_ptr -> bigger;
        current_tt_ptr -> bigger = new_tt_ptr;
		if(new_tt_ptr -> bigger != nullptr){
            new_tt_ptr -> bigger -> smaller = new_tt_ptr;
		}else{
		    this -> largest_time_tree = new_tt_ptr;
		}
		SubTreeCourse new_stc;
		new_stc.holder_time_tree = new_tt_ptr;
		new_stc.course_id = courseID;
        (course_node -> val).lectures[classID] = new_stc.lectures_tree.Insert(lecture);
        (course_node -> val).lectures[classID] -> holder_sub_tree_course = new_tt_ptr -> subtree_tree
		        .Insert(new_stc);
        new_stc.lectures_tree.root = nullptr;
	}
	
	// If stc is empty, remove it from the time tree.
	if(stc_ptr->lectures_tree.root == nullptr){

		TimeTree* tt_ptr = (TimeTree*)stc_ptr -> holder_time_tree;

		tt_ptr -> subtree_tree.Remove(*stc_ptr);

		if(tt_ptr->subtree_tree.root == nullptr){

			if(tt_ptr -> bigger != nullptr){
                tt_ptr -> bigger -> smaller = tt_ptr -> smaller;
			} else{
				this -> largest_time_tree = tt_ptr -> smaller;
			}

			if(tt_ptr -> smaller != nullptr){
                tt_ptr -> smaller -> bigger = tt_ptr -> bigger;
			} else{
				this -> smallest_time_tree = tt_ptr -> bigger;
			}

			delete tt_ptr;
		
		}
	}

}


void CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

    if(course_node ->val.lectures_num < classID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

	Lecture *lecture_ptr = (course_node->val).lectures[classID];
	*timeViewed = lecture_ptr->watch_num;
}


void CoursesManager::GetMostViewedClasses(int numOfClasses, int *courses, int
*classes){
	int num_Of_Classes_left = timeTree_search(numOfClasses, courses, classes,
                                         this -> largest_time_tree);

	if(num_Of_Classes_left > 0){throw std::invalid_argument("FAILURE");}
}

int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt_ptr ){
	if(tt_ptr == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = stc_inorder(numOfClasses, courses, classes,
                                          tt_ptr->subtree_tree.root);

	if(num_Of_Classes_left > 0){
		return timeTree_search(num_Of_Classes_left,
                         &(courses[numOfClasses - num_Of_Classes_left]),
                         &(classes[numOfClasses - num_Of_Classes_left]),
                         tt_ptr -> smaller);
	}
	else{
		return 0;
	}
}

int stc_inorder(int numOfClasses, int *courses, int *classes, AVLNode<SubTreeCourse> *stc_node){
	if(stc_node == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = stc_inorder(numOfClasses,courses, classes, stc_node->left_son);

	if(num_Of_Classes_left == 0){
		return 0;
	}
	else{
		num_Of_Classes_left =
		        lectures_inorder(num_Of_Classes_left,
                &courses[numOfClasses - num_Of_Classes_left],
                &classes[numOfClasses - num_Of_Classes_left],
                stc_node->val.lectures_tree.root);
	}
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		return num_Of_Classes_left = stc_inorder(num_Of_Classes_left,&courses[num_Of_Classes_left],
                                                 &classes[num_Of_Classes_left], stc_node->right_son);
	}
}

int lectures_inorder(int numOfClasses, int *courses, int *classes, AVLNode<Lecture> *lecture_node){
	if(lecture_node == nullptr){
		return numOfClasses;
	}
	int num_Of_Classes_left = lectures_inorder(numOfClasses,courses, classes, lecture_node->left_son);
	
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		classes[numOfClasses - num_Of_Classes_left] = lecture_node->val
		        .lecture_id;
		SubTreeCourse* temp_stc_ptr = (SubTreeCourse*)lecture_node->val
		        .holder_sub_tree_course;
		courses[numOfClasses - num_Of_Classes_left] = temp_stc_ptr -> course_id;
		num_Of_Classes_left -= 1;
	}
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		return num_Of_Classes_left = lectures_inorder(num_Of_Classes_left,
                                                &courses[numOfClasses -
                                                num_Of_Classes_left],
                                                &classes[numOfClasses -
                                                num_Of_Classes_left],
                                                lecture_node->right_son);
	}
}

void update_inorder_pointers(AVLNode<Lecture> *avl_node, Lecture** arr,
                             SubTreeCourse* stc_ptr){
	if(avl_node == nullptr){
		return;
	}
	update_inorder_pointers(avl_node->left_son,arr ,stc_ptr);
	
	arr[avl_node->val.lecture_id] = &(avl_node->val);
	avl_node->val.holder_sub_tree_course = stc_ptr;

	update_inorder_pointers(avl_node->right_son,arr ,stc_ptr);
}

CoursesManager::~CoursesManager(){

    // Go to each tree from smallest to largest and call delete each tree,so
    // each tree destructor will be called.
    TimeTree* current_tt_ptr = this->smallest_time_tree;
    TimeTree* next_tt_ptr;
    while(current_tt_ptr != nullptr){
        next_tt_ptr = current_tt_ptr -> bigger;
        delete current_tt_ptr;
        current_tt_ptr = next_tt_ptr;
    }

    // The course tree destructor will be called in the end of the time trees.

}

Course::~Course(){

    if(this -> lectures != nullptr) {
        delete[] this -> lectures;
    }

}

Course::Course(int course_id, int lectures_num){

    this->course_id = course_id;
    this->lectures_num=lectures_num;
    try {
        this->lectures = new Lecture*[lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

}

Course::Course(const Course& c){

    this->course_id = c.course_id;
    this->lectures_num = c.lectures_num;
    try {
        this->lectures=new Lecture*[lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }
    for (int i = 0; i < c.lectures_num; ++i) {
        this->lectures[i] = c.lectures[i];
    }

}

Course& Course::operator=(const Course& c){
    if (this == &c) {
        return *this;
    }
    delete this->lectures;
    this->course_id = c.course_id;
    this->lectures_num = c.lectures_num;
    try {
        this->lectures = new Lecture*[c.lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }
    for (int i = 0; i < c.lectures_num; ++i) {
        this->lectures[i] = c.lectures[i];
    }
    return *this;
}

SubTreeCourse::SubTreeCourse(const SubTreeCourse& stc){
    this->course_id=stc.course_id;
    this->holder_time_tree=stc.holder_time_tree;
    this->lectures_tree=stc.lectures_tree;
}
SubTreeCourse& SubTreeCourse::operator=(const SubTreeCourse& stc){
    this->course_id=stc.course_id;
    this->holder_time_tree=stc.holder_time_tree;
    this->lectures_tree=stc.lectures_tree;
    return *this;
}



Lecture::Lecture(){
    this->lecture_id=0;
    this->watch_num=0;
    this->holder_sub_tree_course = nullptr;
}
Lecture::Lecture(const Lecture& l){
    this->lecture_id=l.lecture_id;
    this->watch_num=l.watch_num;
    this->holder_sub_tree_course=l.holder_sub_tree_course;
}
Lecture& Lecture::operator=(const Lecture& l){
    this->lecture_id=l.lecture_id;
    this->watch_num=l.watch_num;
    this->holder_sub_tree_course=l.holder_sub_tree_course;
    return *this;
}

bool operator<(const Course& c1, const Course& c2){
    return c1.course_id < c2.course_id;
}

bool operator>(const Course& c1, const Course& c2){
    return c2 < c1;
}

bool operator==(const Course& c1, const Course& c2){
    return !(c2 < c1) && !(c1 < c2);
}

bool operator<=(const Course& c1, const Course& c2){
    return !(c2 < c1);
}

bool operator>=(const Course& c1, const Course& c2){
    return !(c1 < c2);
}

bool operator<(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return c1.course_id < c2.course_id;
}

bool operator>(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return c2 < c1;
}

bool operator==(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return !(c2 < c1) && !(c1 < c2);
}

bool operator<=(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return !(c2 < c1);
}

bool operator>=(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return !(c1 < c2);
}

bool operator<(const Lecture& c1, const Lecture& c2){
    return c1.lecture_id < c2.lecture_id;
}

bool operator>(const Lecture& c1, const Lecture& c2){
    return c2 < c1;
}

bool operator==(const Lecture& c1, const Lecture& c2){
    return !(c2 < c1) && !(c1 < c2);
}

bool operator<=(const Lecture& c1, const Lecture& c2){
    return !(c2 < c1);
}

bool operator>=(const Lecture& c1, const Lecture& c2){
    return !(c1 < c2);
}