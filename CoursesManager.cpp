//
// Created by Yuval.M on 04/12/2020.
//
#include "CoursesManager.h"


void update_inorder_pointers(AVLNode<Lecture> *avl_node, Lecture* arr);
int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt );
int stc_inorder(int numOfClasses, int *courses, int *classes, AVLNode<SubTreeCourse> *stc_node);
int lectures_inorder(int numOfClasses, int *courses, int *classes, AVLNode<Lecture> *lecture_node);

void* CoursesManager::Init(){

    CoursesManager* cm = new CoursesManager;

    return (void*)cm;

}


StatusType CoursesManager::AddCourse (int courseID, int numOfClasses) {

    Course c;

    c.course_id = courseID;

    c.lectures_num = numOfClasses;

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
        // create time tree 0
        TimeTree* tt = new TimeTree;
        tt -> time_watched = 0;
        tt -> subtree_tree = nullptr;
        tt -> bigger = nullptr;
        tt -> smaller = nullptr;
        this -> smallest_time_tree = &tt;
    } else if(this -> smallest_time_tree -> time_watched > 0){
        // create time tree 0.
        TimeTree* tt = new TimeTree;
        tt -> time_watched = 0;
        tt -> subtree_tree = nullptr;
        tt -> bigger = this -> smallest_time_tree;
        tt -> bigger -> smaller = &tt; 
        tt -> smaller = nullptr;
        this -> smallest_time_tree = &tt;
    }

    // Create subtree course from course lectures and insert
    SubTreeCourse stc(numOfClasses, new_lectures, (void*)this ->
    smallest_time_tree);

    // Go to each lecture one by one and add holder_sub_tree_course, and
    // put pointer in lectures field in course array.
    update_inorder_pointers(stc.lectures_tree.root, c.lectures, &stc);

    // Add stc to tt.
    tt->subtree_tree = stc;

    this -> course_tree.Insert(c);

    return SUCCESS;

}


StatusType CoursesManager::RemoveCourse(int courseID){
	Course temp;
	temp.course_id = courseID;
	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);
	Course c = course_node->val;

	for(int i=0; i < c.lectures_num; i++){

		SubTreeCourse* stc = c.lectures[i] -> holder_sub_tree_course;

		stc -> remove(*c.lectures[i]);

		// If stc is empty, remove it from the time tree.
		if(stc->lectures_tree.root == nullptr){

			TimeTree* tt = stc -> holder_time_tree;

			tt -> subtree_tree.remove(*stc);

			if(tt->subtree_tree.root == nullptr){

				if(tt -> bigger != nullptr){
					tt -> bigger -> smaller = tt -> smaller;
				} else{
					this -> biggest = tt -> smaller;
				}

				if(tt -> smaller != nullptr){
					tt -> smaller -> bigger = tt -> bigger;
				} else{
					this -> smallest = tt -> bigger;
				}

				delete tt;
			
			}
		}
	}


	this -> course_tree.remove(c);

	return SUCCESS;

}

StatusType CoursesManager::WatchClass(int courseID, int classID, int time){
	
	Course temp;
	temp.course_id = courseID;
	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);
	Course c = course_node->val;

	Lecture *lecture_ptr = c.lectures[classID];
	Lecture lecture =  *lecture_ptr;
	SubTreeCourse* stc = lecture_ptr -> holder_sub_tree_course;
	stc -> remove(lecture);

	TimeTree* original_tt = stc -> holder_time_tree;

	TimeTree* current_tt = original_tt;

	lecture.watch_num =  original_tt -> time_watched + time;

	while(current_tt -> bigger != nullptr || current_tt -> bigger -> time_watched < original_tt -> time_watched + time){
		current_tt = current_tt -> bigger;
	}

	if(current_tt -> time_watched == original_tt -> time_watched + time){
		AVLNode<SubTreeCourse>* current_stc_node = current_tt -> subtree_tree.FindValue(*stc);
		if (current_stc_node){
			lecture.holder_sub_tree_course = &(current_stc_node -> val);
			c.lectures[classID] = current_stc_node -> val.insert(lecture);
		}else{
			SubTreeCourse new_stc;
			new_stc.holder_time_tree = current_tt;
			new_stc.course_id = courseID;
			c.lectures[classID] = new_stc.lectures_tree.insert(lecture);
			c.lectures[classID].holder_sub_tree_course = current_tt -> subtree_tree.insert(new_stc);
		}
	}else{
		TimeTree* new_tt = new TimeTree;
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
		c.lectures[classID] = new_stc.lectures_tree.insert(lecture);
		c.lectures[classID].holder_sub_tree_course = new_tt -> subtree_tree.insert(new_stc);
	}
	
	// If stc is empty, remove it from the time tree.
	if(stc->lectures_tree.root == nullptr){

		TimeTree* tt = stc -> holder_time_tree;

		tt -> subtree_tree.remove(*stc);

		if(tt->subtree_tree.root == nullptr){

			if(tt -> bigger != nullptr){
				tt -> bigger -> smaller = tt -> smaller;
			} else{
				this -> biggest = tt -> smaller;
			}

			if(tt -> smaller != nullptr){
				tt -> smaller -> bigger = tt -> bigger;
			} else{
				this -> smallest = tt -> bigger;
			}

			delete tt;
		
		}
	}

}


StatusType CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
	Course temp;
	temp.course_id = courseID;
	AVLNode<Course> *course_node = this-> course_tree.FindValue(temp);
	Course c = course_node->val;

	Lecture *lecture_ptr = c.lectures[classID];
	*timeViewed = lecture_ptr->watch_num;	
}


StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int *courses, int *classes){
	timeTree_search(numOfClasses, courses, classes, this -> largest_time_tree);

	return SUCCESS;
}

int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt ){
	if(tt == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = stc_reversed_inorder(numOfClasses,courses, classes, tt->subtree_tree->root);
	if(num_Of_Classes_left > 0){
		return timeTree_search(num_Of_Classes_left,courses, classes,tt -> smaller);
	}
	else{
		return 0;
	}
}

int stc_inorder(int numOfClasses, int *courses, int *classes, AVLNode<SubTreeCourse> *stc_node){
	if(tt == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = stc_inorder(numOfClasses,courses, classes, stc_node->left_son);

	if(num_Of_Classes_left == 0){
		return 0;
	}
	else{
		num_Of_Classes_left =  lectures_inorder(num_Of_Classes_left,courses[num_Of_Classes_left], 
								classes[num_Of_Classes_left], stc_nod->val->lectures_tree->root);
	}
	if(num_Of_Classes_left == 0){
		return 0;
	else{
		return num_Of_Classes_left = stc_inorder(num_Of_Classes_left,courses[num_Of_Classes_left],
								 classes[num_Of_Classes_left], stc_node->right_son);
	}
}

int lectures_inorder(int numOfClasses, int *courses, int *classes, AVLNode<Lecture> *lecture_node){
	if(tt == nullptr){
		return numOfClasses;
	}
	int num_Of_Classes_left = lectures_inorder(numOfClasses,courses, classes, lecture_node->left_son);
	
	if(num_Of_Classes_left == 0){
		return 0;
	else{
		classes[num_Of_Classes_left] = lecture_node->val->lecture_id;
		courses[num_Of_Classes_left] = lecture_node->val->holder_sub_tree_course->course_id;
		num_Of_Classes_left -= 1;
	}
	if(num_Of_Classes_left == 0){
		return 0;
	else{
		return num_Of_Classes_left = lectures_inorder(num_Of_Classes_left,courses[num_Of_Classes_left], 
							classes[num_Of_Classes_left], lecture_node->right_son);
	}
}

void update_inorder_pointers(AVLNode<Lecture> *avl_node, Lecture* arr, SubTreeCourse* stc_ptr){
	if(avl_node == nullptr){
		return;
	}
	update_inorder_pointers(avl_node->left_son,arr);
	
	arr[avl_node->val->lecture_id] = &avl_node->val;
	avl_node->val->holder_sub_tree_course = stc_ptr;

	update_inorder_pointers(avl_node->right_son,arr);
}