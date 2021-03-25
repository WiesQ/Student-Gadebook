#include "menu.h"

using namespace std;

Menu::Menu() : menuON(true), LoggedIn(false), level("Login")
{
    while(menuON)
    {
        while((menuON) && (!LoggedIn))
        {
            system("CLS");
            if(level=="Login") { Login(); }
            else if(level=="TeacherLogIn") { TeacherLogIn(); }
        }
        while((menuON) && (LoggedIn))
        {
            system("CLS");
            if(level=="MainMenu") {MainMenu();}
            else if(level=="DisplayStudents") {DisplayStudents();}
            else if(level=="DisplayYourStudents") {DisplayYourStudents();}
            else if(level=="CheckStudentGrades") {CheckStudentGrades();}
            else if(level=="CheckSubjectGrades") {CheckSubjectGrades();}
            else if(level=="AddStudent") {AddStudent();}
            else if(level=="ClassAverage") {ClassAverage();}
            else if(level=="DeleteStudent") {DeleteStudent();}
            else{MainMenu();}
        }
    }
}

void Menu::WrongInput()
{
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout<<"Wrong input. Try again."<<endl;
    system("pause");
}

void Menu::Login()
{
    cout << "1. Log in" << endl;
    cout << "2. Quit" << endl << endl;

     int option;

        cout << "Choose option: ";
        cin >> option;

        switch(option)
        {
        case 1:
            level = "TeacherLogIn";
            break;
        case 2:
            menuON = false;
            break;
        default:
            WrongInput();
        }
}

void Menu::TeacherLogIn()
{
    string login, password;


        cout << "Login: ";
        cin.ignore();
        getline(cin, login);

        cout << "Password: ";
        getline(cin, password);

        const int x = display.logged_in(login,password);
        if(x == 1)
        {
            cout << "Successfully logged in! Welcome to the Gradebook!" << endl;
            LoggedIn = true;
            level = "MainMenu";
            system("pause");
        }
        else
        {
            cout << "Wrong login or password" << endl;
            level = "Login";
            system("pause");
        }
}

void Menu::MainMenu()
{
    system("CLS");
    cout<<"1. Display all students"<<endl;
    cout<<"2. Display your students"<<endl;
    cout<<"3. Logout"<<endl;
    cout<<"4. Quit"<<endl<<endl;

    int option;

    cout << "Choose option: ";

    cin>>option;

    switch(option)
    {
    case 1:
        level="DisplayStudents";
        break;
    case 2:
        level="DisplayYourStudents";
        break;
    case 3:
        LoggedIn=false;
        level="Login";
        break;
    case 4:
        menuON=false;
        break;
    default:
        WrongInput();
    }
}

void Menu::DisplayStudents()
{
    display.display_students();

    cout<<"\n1. Check student Grades"<<endl;
    cout<<"2. Display class average"<<endl;
    cout<<"3. Return to main menu"<<endl<<endl;

    int option;

    cout << "Choose option: ";

    cin>>option;

    switch(option)
    {
    case 1:
        level="CheckStudentGrades";
        break;
    case 2:
        level="ClassAverage";
        break;
    case 3:
        level="MainMenu";
        break;
    default:
        WrongInput();
    }
}

void Menu::ClassAverage()
{
    cout<<"Class which GPA you want to see: ";
    string s_class;
    cin>>s_class;

    display.class_avg(s_class);

    system("pause");
    level="MainMenu";
}

void Menu::CheckStudentGrades()
{
    string name, surname;
    cout<<"Enter Student's name: ";
    cin>>name;
    cin>>surname;

    display.display_student_grade(name, surname);

    system("pause");
    level="MainMenu";


}

void Menu::DisplayYourStudents()
{
    string teacher;
    teacher = display.getcurrently_logged();

    display.display_teacher_students(teacher);

    cout<<"1. Display Student Grades"<<endl;
    cout<<"2. Add Student"<<endl;
    cout<<"3. Delete Student"<<endl;
    cout<<"4. Return to Main Menu"<<endl<<endl;

    int option;

    cout<<"Choose option: ";

    cin>>option;

       switch(option)
    {
    case 1:
        level="CheckSubjectGrades";
        break;
    case 2:
        level="AddStudent";
        break;
    case 3:
        level="DeleteStudent";
        break;
    case 4:
        level="MainMenu";
        break;
    default:
        WrongInput();
    }
}

void Menu::CheckSubjectGrades()
{
    string name, surname, teacher;
    teacher=display.getcurrently_logged();
    cout<<"Enter Student's name: ";
    cin>>name;
    cin>>surname;

    display.subj_grades(name, surname, teacher);

    cout<<"1. Add Grade"<<endl;
    cout<<"2. Update Grade"<<endl;
    cout<<"3. Delete Grade"<<endl;
    cout<<"4. Return"<<endl<<endl;

    int option;

    cout<<"Choose option: ";

    cin>>option;

      switch(option)
    {
    case 1:
        {
        double grade;
        string type;
        cout<<"Specify Student's grade that is going to be added: ";
        cin>>grade;
        cout<<"Specify the type of the grade: ";
        cin>>type;
        display.add_grade(grade, teacher, name, surname, type);
        system("pause");
        break;
        }
    case 2:
        {
        double grade, up_grade;
        string type, up_type;
        cout<<"First specify which grade you want to update"<<endl;
        cout<<"Specify the grade: ";
        cin>>grade;
        cout<<"Specify the type: ";
        cin>>type;
        cout<<"Now specify the updated parameters of the grade"<<endl;
        cout<<"Specify the updated grade: ";
        cin>>up_grade;
        cout<<"Specify the updated grades type: ";
        cin>>up_type;
        display.update_grade(grade, up_grade, name, surname, teacher, type, up_type);
        system("pause");
        break;
        }
    case 3:
        {
        double grade;
        string type;
        cout<<"Specify the grade that you want to delete: ";
        cin>>grade;
        cout<<"Specify the type of the grade that you want to delete: ";
        cin>>type;
        display.delete_grade(grade, name, surname, teacher, type);
        system("pause");
        break;
        }

    case 4:
        level="DisplayYourStudents";
        break;
    default:
        WrongInput();
    }
}

void Menu::AddStudent()
{
    string name, surname, s_class, teacher;
    teacher = display.getcurrently_logged();

    cout<<"Enter Student's name: ";
    cin>>name;
    cin>>surname;
    cout<<"Specify in which class is the student: ";
    cin>>s_class;

    display.add_student(name, surname, s_class, teacher);

    system("pause");
    level="DisplayYourStudents";
}

void Menu::DeleteStudent()
{
    string name, surname;
    cout<<"Enter Student's name: ";
    cin>>name;
    cin>>surname;

    display.delete_student(name, surname);

    system("pause");
    level="DisplayYourStudents";
}



