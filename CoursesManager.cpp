//
// Created by Yuval.M on 04/12/2020.
//
#include "CoursesManager.h"


void update_inorder_pointers(AVLNode<Lecture> *avl_node, Lecture** arr,
                             SubTreeCourse* stc_ptr);
int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt );
int stc_inorder(int numOfClasses, int *courses, int *classes, AVLNode<SubTreeCourse> *stc_node);
int lectures_inorder(int numOfClasses, int *courses, int *classes, AVLNode<Lecture> *lecture_node);

void CoursesManager::AddCourse (int courseID, int numOfClasses) {

    Course c(courseID, numOfClasses);

    if(this -> course_tree.FindValue(c) != nullptr){
        throw std::invalid_argument("FAILURE");
    }

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

    TimeTree* tt;

    // If smallest time is not 0, create it.
    if(this -> smallest_time_tree == nullptr){

        // create time tree 0
        try {
            tt = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        tt -> time_watched = 0;
        tt -> bigger = nullptr;
        tt -> smaller = nullptr;
        this -> smallest_time_tree = tt;
    } else if(this -> smallest_time_tree -> time_watched > 0){
        // create time tree 0.

        try {
            tt = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        tt -> time_watched = 0;
        tt -> bigger = this -> smallest_time_tree;
        tt -> bigger -> smaller = tt;
        tt -> smaller = nullptr;
        this -> smallest_time_tree = tt;
    }

    // Create subtree course from course lectures and insert
    SubTreeCourse stc(numOfClasses, new_lectures, numOfClasses, (void*)this ->
    smallest_time_tree);

    try {
        c.lectures = new Lecture*[c.lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

    // Go to each lecture one by one and add holder_sub_tree_course, and
    // put pointer in lectures field in course array.
    update_inorder_pointers(stc.lectures_tree.root, c.lectures, &stc);

    // Add stc to tt.
    this -> smallest_time_tree -> subtree_tree.Insert(stc);

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

		SubTreeCourse* stc = (SubTreeCourse*)(c.lectures[i] ->
		        holder_sub_tree_course);

		stc ->lectures_tree.Remove(*c.lectures[i]);

		// If stc is empty, remove it from the time tree.
		if(stc->lectures_tree.root == nullptr){

			TimeTree* tt = (TimeTree*)stc -> holder_time_tree;

			tt -> subtree_tree.Remove(*stc);

			if(tt->subtree_tree.root == nullptr){

				if(tt -> bigger != nullptr){
					tt -> bigger -> smaller = tt -> smaller;
				} else{
					this -> largest_time_tree = tt -> smaller;
				}

				if(tt -> smaller != nullptr){
					tt -> smaller -> bigger = tt -> bigger;
				} else{
					this -> smallest_time_tree = tt -> bigger;
				}

				delete tt;
			
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

    if(course_node ->val.lectures_num < courseID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

	Lecture *lecture_ptr = (course_node -> val).lectures[classID];
	Lecture lecture =  *lecture_ptr;
	SubTreeCourse* stc = (SubTreeCourse*)lecture_ptr -> holder_sub_tree_course;
	stc -> lectures_tree.Remove(lecture); //TODO

	TimeTree* original_tt = (TimeTree*)stc -> holder_time_tree;

	TimeTree* current_tt = original_tt;

	lecture.watch_num =  original_tt -> time_watched + time;

	while(current_tt -> bigger != nullptr || current_tt -> bigger -> time_watched < original_tt -> time_watched + time){
		current_tt = current_tt -> bigger;
	}

	if(current_tt -> time_watched == original_tt -> time_watched + time){
		AVLNode<SubTreeCourse>* current_stc_node = current_tt -> subtree_tree.FindValue(*stc);
		if (current_stc_node){
			lecture.holder_sub_tree_course = &(current_stc_node -> val);
            (course_node -> val).lectures[classID] =
                    current_stc_node -> val.lectures_tree.Insert(lecture);
		}else{
			SubTreeCourse new_stc;
			new_stc.holder_time_tree = current_tt;
			new_stc.course_id = courseID;
            (course_node -> val).lectures[classID] = new_stc.lectures_tree.Insert(lecture);
            (course_node -> val).lectures[classID] -> holder_sub_tree_course = current_tt ->
			        subtree_tree.Insert(new_stc);
		}
	}else{

        TimeTree* new_tt;

        try {
            new_tt = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

		new_tt -> time_watched = lecture.watch_num;
		new_tt -> smaller = current_tt;
		new_tt -> bigger = current_tt -> bigger;
		current_tt -> bigger = new_tt;
		if(new_tt -> bigger != nullptr){
			new_tt -> bigger -> smaller = new_tt;
		}
		SubTreeCourse new_stc;
		new_stc.holder_time_tree = new_tt;
		new_stc.course_id = courseID;
        (course_node -> val).lectures[classID] = new_stc.lectures_tree.Insert(lecture);
        (course_node -> val).lectures[classID] -> holder_sub_tree_course = new_tt -> subtree_tree
		        .Insert(new_stc);
	}
	
	// If stc is empty, remove it from the time tree.
	if(stc->lectures_tree.root == nullptr){

		TimeTree* tt = (TimeTree*)stc -> holder_time_tree;

		tt -> subtree_tree.Remove(*stc);

		if(tt->subtree_tree.root == nullptr){

			if(tt -> bigger != nullptr){
				tt -> bigger -> smaller = tt -> smaller;
			} else{
				this -> largest_time_tree = tt -> smaller;
			}

			if(tt -> smaller != nullptr){
				tt -> smaller -> bigger = tt -> bigger;
			} else{
				this -> smallest_time_tree = tt -> bigger;
			}

			delete tt;
		
		}
	}

}


void CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

    if(course_node ->val.lectures_num < courseID + 1){
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

int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt ){
	if(tt == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = stc_inorder(numOfClasses,courses, classes,
                                       tt->subtree_tree.root);

	if(num_Of_Classes_left > 0){
		return timeTree_search(num_Of_Classes_left,courses, classes,tt -> smaller);
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
		num_Of_Classes_left =  lectures_inorder(num_Of_Classes_left,
                                          &courses[num_Of_Classes_left],
                                          &classes[num_Of_Classes_left],
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
		classes[num_Of_Classes_left] = lecture_node->val.lecture_id;
		SubTreeCourse* temp_stc = (SubTreeCourse*)lecture_node->val
		        .holder_sub_tree_course;
		courses[num_Of_Classes_left] = temp_stc -> course_id;
		num_Of_Classes_left -= 1;
	}
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		return num_Of_Classes_left = lectures_inorder(num_Of_Classes_left,
                                                &courses[num_Of_Classes_left],
                                                &classes[num_Of_Classes_left],
                                                lecture_node->right_son);
	}
}

void update_inorder_pointers(AVLNode<Lecture> *avl_node, Lecture** arr,
                             SubTreeCourse* stc_ptr){
	if(avl_node == nullptr){
		return;
	}
	update_inorder_pointers(avl_node->left_son,arr ,stc_ptr);
	
	arr[avl_node->val.lecture_id - 1] = &(avl_node->val);
	avl_node->val.holder_sub_tree_course = stc_ptr;

	update_inorder_pointers(avl_node->right_son,arr ,stc_ptr);
}

CoursesManager::~CoursesManager(){

    // Go to each tree from smallest to largest and call delete each tree,so
    // each tree destructor will be called.
    TimeTree* current_tt = this->smallest_time_tree;
    TimeTree* next_tt;
    while(current_tt != nullptr){
        next_tt = current_tt -> bigger;
        delete current_tt;
        current_tt = next_tt;
    }

    // The course tree destructor will be called in the end of the time trees.

}

Course::~Course(){

    if(lectures != nullptr) {
        delete[] lectures;
    }

}

Course::Course(int course_id, int lectures_num){

    this->course_id = course_id;
    this->lectures_num=lectures_num;
    try {
        this->lectures=new Lecture*[lectures_num];
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