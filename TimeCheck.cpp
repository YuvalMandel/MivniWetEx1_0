#include <map>
#include <iostream>
#include <sstream>
#include <functional>
#include <cmath>
#include <exception>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
// Change path if necessary
#include "library.h"

using std::cout;
using std::endl;
using std::string;
using namespace std::chrono;

// Asserts that x is true
#define ASSERT_TEST(x) do{if(!(x)){ \
    std::cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<std::endl;\
    return false; }} while(false);

// Asserts that x causes no errors
#define ASSERT_NO_ERROR(x) do{try{x;}catch(std::exception& e){std::cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received error "<<"\""<<e.what()<<"\""<<" while expecting no errors"<<std::endl; return false;}} while(false);

// Asserts that x causes this error (and no other error)
#define ASSERT_ERROR(x, error) do{ \
    try{x; std::cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received no error while expecting "<<#error<<std::endl; return false;}\
    catch(error& e){}\
    catch(const std::exception& e){std::cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received error: "<<"\""<<e.what()<<"\" while expecting "<<"\""<<#error<<"\""<<std::endl; return false;}\
    } while(false);

// Asserts that x causes this error (and no other error) with message
#define ASSERT_ERROR_WITH_MESSAGE(x, error, message) do{ \
    try{x; std::cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received no error while expecting "<<#error<<std::endl; return false;}\
    catch(error& e){ASSERT_TEST(string(e.what()) == (message));}\
    catch(const std::exception& e){std::cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received error: "<<"\""<<e.what()<<"\" while expecting "<<"\""<<#error<<"\""<<std::endl; return false;}\
    } while(false);

#define ADD_TEST(x) tests[#x]=x;

//Test functions:

//Helper function to calculate the average duration of a vector of durations
long long calculateAverageDuration(std::vector<long long> durations, int repetitions){
    long long sum = 0;
    for(long long& element : durations){
        sum += element;
    }
    return sum/repetitions;
}

//Helper function to check the complexity of a certain input size
bool measureRuntime(int num_courses, int classes_per_course, int time_per_course, int percentage_to_fill_with_time, int repetitions, std::vector<long long>& result){

    // Create vectors for storing repetition results. Will be used to calculate average runtime.
    std::vector<long long> init_durations;
    std::vector<long long> add_durations;
    std::vector<long long> watch_durations;
    std::vector<long long> remove_durations;
    std::vector<long long> get_half_durations;
    std::vector<long long> get_all_durations;
    std::vector<long long> quit_durations;
    // Perform the measurements.
    for(int i = 0; i < repetitions; i++){
        // Measure the time it takes to initialize.
        auto start = high_resolution_clock::now();
        void* DS = Init();
        auto stop = high_resolution_clock::now(); 
        auto init_duration = duration_cast<nanoseconds>(stop - start); 
        init_durations.push_back(init_duration.count());
        ASSERT_TEST(DS);
        Quit(&DS);
        ASSERT_TEST(DS == nullptr);            
    }

    //Initialize the system and fill it with courses and classes.
    void* DS = Init();
    ASSERT_TEST(DS);

    // Fill the system with courses.
    for(int i = 1; i <= num_courses; i++){
        ASSERT_TEST(AddCourse(DS,i,classes_per_course) == SUCCESS);
    }
    // Fill the system so that a certain percentage of the classes has time viewed.
    int courses_to_fill_with_time = (classes_per_course*percentage_to_fill_with_time)/100;
    if(courses_to_fill_with_time != 0){
        for(int i = 1; i <= num_courses; i++){
            for(int j = 0; j < courses_to_fill_with_time; j++){
                ASSERT_TEST(WatchClass(DS,i,j,time_per_course) == SUCCESS);
            }
        }
    }

    for(int i = 0; i < repetitions; i++){

        // Measure the time it takes to add a course.
        auto start = high_resolution_clock::now();
        ASSERT_TEST(AddCourse(DS,num_courses+1,classes_per_course) == SUCCESS);
        auto stop = high_resolution_clock::now(); 
        auto add_duration = duration_cast<nanoseconds>(stop - start); 
        add_durations.push_back(add_duration.count());

        // Measure the time it takes to add time to a class.
        start = high_resolution_clock::now();
        ASSERT_TEST(WatchClass(DS,num_courses+1,classes_per_course-1,time_per_course) == SUCCESS);
        stop = high_resolution_clock::now();
        auto watch_duration = duration_cast<nanoseconds>(stop - start); 
        watch_durations.push_back(watch_duration.count());

        // Measure the time it takes to remove a course.
        start = high_resolution_clock::now(); 
        ASSERT_TEST(RemoveCourse(DS,num_courses+1) == SUCCESS);
        stop = high_resolution_clock::now();
        auto remove_duration = duration_cast<nanoseconds>(stop - start); 
        remove_durations.push_back(remove_duration.count());

        // Measure the time it takes to get half of the classes in top to bottom order.
        int total_num_classes = num_courses*classes_per_course;
        int* courses = new int[total_num_classes/2];
        int* classes = new int[total_num_classes/2];
        start = high_resolution_clock::now(); 
        ASSERT_TEST(GetMostViewedClasses(DS,total_num_classes/2,courses,classes) == SUCCESS);
        stop = high_resolution_clock::now();
        auto get_half_duration = duration_cast<nanoseconds>(stop - start);
        get_half_durations.push_back(get_half_duration.count());
        delete[] courses;
        delete[] classes;

        // Measure the time it takes to get all of the classes in top to bottom order.
        int* courses2 = new int[total_num_classes];
        int* classes2 = new int[total_num_classes];
        start = high_resolution_clock::now(); 
        ASSERT_TEST(DS);
        ASSERT_TEST(courses2);
        ASSERT_TEST(classes2);

        ASSERT_TEST(GetMostViewedClasses(DS,total_num_classes,courses2,classes2) == SUCCESS);
        stop = high_resolution_clock::now();
        auto get_all_duration = duration_cast<nanoseconds>(stop - start);
        get_all_durations.push_back(get_all_duration.count());
        delete[] courses2;
        delete[] classes2;
    }
    // Measure the time it takes to quit.
    auto start = high_resolution_clock::now();
    Quit(&DS);
    auto stop = high_resolution_clock::now(); 
    auto quit_duration = duration_cast<nanoseconds>(stop - start); 
    quit_durations.push_back(quit_duration.count());
    ASSERT_TEST(DS == nullptr); 

    // Calculate the average duration of each operations.
    long long init_average_duration = calculateAverageDuration(init_durations,repetitions);
    long long add_average_duration = calculateAverageDuration(add_durations,repetitions);
    long long watch_average_duration = calculateAverageDuration(watch_durations,repetitions);
    long long remove_average_duration = calculateAverageDuration(remove_durations,repetitions);
    long long get_half_average_duration = calculateAverageDuration(get_half_durations,repetitions);
    long long get_all_average_duration = calculateAverageDuration(get_all_durations,repetitions);
    // Not using average for quit because that would take a lot of time to run.
    long long quit_average_duration = quit_durations[0];

    // Return the results as a vector.
    std::vector<long long> output = {init_average_duration,add_average_duration,watch_average_duration,remove_average_duration,get_half_average_duration,get_all_average_duration,quit_average_duration};
    result = output;
    return true;
}

// Measure the time it takes for inputs in logarithmic scale to perform the various functions.
bool testTimeComplexity(){
    // Change these parameters to set the size of the test:
    int base = 10;
    int max_power = 5;
    int number_of_classes_per_course = 10;
    int time_per_course = 10;
    int percentage_to_fill_with_time = 25; // an integer from 0 to 100.
    int repetitions = 100;
    // Run the test on different output sizes in logarithmic scale.
    std::vector<long long> result;
    for(int i = 1; i <= max_power; i++){
        std::vector<long long> result_i;
        ASSERT_TEST(measureRuntime(pow(base,i),number_of_classes_per_course,time_per_course,percentage_to_fill_with_time,repetitions,result_i));
        result.insert(result.end(),result_i.begin(),result_i.end());
    }
    // Print the results of the tests
    std::cout<<"Showing average runtime results from "<<repetitions<<" repetitions (except Quit):"<<std::endl;
    std::cout<<"Init (Ignore the first result, it's probably larger than the others):"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7]<<" nanoseconds."<<std::endl;
    }
    std::cout<<"AddCourse:"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7+1]<<" nanoseconds."<<std::endl;
    }
    std::cout<<"WatchClass:"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7+2]<<" nanoseconds."<<std::endl;
    }
    std::cout<<"RemoveCourse:"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7+3]<<" nanoseconds."<<std::endl;
    }
    std::cout<<"GetMostViewedClassses with half of the classes:"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7+4]<<" nanoseconds."<<std::endl;
    }
    std::cout<<"GetMostViewedClassses with all of the classes:"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7+5]<<" nanoseconds."<<std::endl;
    }
    std::cout<<"Quit:"<<std::endl;
    for(int i = 1; i <= max_power; i++){
        std::cout<<pow(base,i)<<": "<<result[(i-1)*7+6]<<" nanoseconds."<<std::endl;
    }
    return true;
}

// Functions to run the program:

bool run_test(std::function<bool()> test, std::string test_name){
    auto start = high_resolution_clock::now(); 
    if(!test()){
        std::cout<<test_name<<" - FAILED."<<std::endl;
        return false;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    std::cout<<test_name<<" - PASSED in "<<duration.count()<<" milliseconds."<<std::endl;
    return true;
}

int main(){

    std::map<std::string, std::function<bool()>> tests;

    ADD_TEST(testTimeComplexity);

    int passed = 0;
    for (std::pair<std::string, std::function<bool()>> element : tests)
    {
        passed += run_test(element.second, "Running " + element.first);
    }

    std::cout<<"Passed "<<passed<<" out of "<<tests.size()<<" tests."<<std::endl;

    return 0;
}