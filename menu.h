#include <iostream>
#include <string>
#include "sqlite3.h"
#include <limits>

using namespace std;

class Student{
private:
    sqlite3* DB;
    char* dir;
    string currently_logged;
    static int callback_class_avg(void * NotUsed, int argc, char** argv, char** azColName);
    static int callback(void * NotUsed, int argc, char** argv, char** azColName);
    static int log_callback(void * NotUsed, int argc, char** argv, char** azColName);

public:
    Student();
    void add_grade(double grade, string teacher, string s_name, string s_surname, string type);
    void display_grades();
    void display_students();
    void display_teacher_students(string teacher);
    void display_student_grade(string name, string surname);
    void update_grade(double grade, double up_grade, string s_name, string s_surname, string teacher, string type, string up_type);
    void add_student(string name, string surname, string s_class, string teacher);
    void class_avg(string classs);
    void subj_grades(string s_name, string s_surname, string teacher);
    void delete_grade(double grade, string s_name, string s_surname, string teacher, string type);
    void delete_student(string name, string surname);
    int logged_in(string login_, string psswd_);
    string getcurrently_logged() { return currently_logged; };
    };

class Menu {
private:
    bool menuON;
    bool LoggedIn;
    string level;
    Student display;

public:
    Menu();
    void Login();
    void TeacherLogIn();
    void MainMenu();
    void DisplayStudents();
    void DisplayYourStudents();
    void CheckStudentGrades();
    void CheckSubjectGrades();
    void AddStudent();
    void ClassAverage();
    void DeleteStudent();
    void WrongInput();
};


