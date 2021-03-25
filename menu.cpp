#include "menu.h"

using namespace std;

Student::Student(): dir("gradebook.db"){sqlite3_open(dir,&DB); }

int Student::callback_class_avg(void * NotUsed, int argc, char** argv, char** azColName)
{
    if (argc == 1)
    {
        for (int i = 0; i < argc; i++)
        {
            cout << azColName[i] << ": " << argv[i] << endl;
        }
        cout << endl;

        return 0;

    }
    else
    {
       cout<<"There is no such class!"<<endl;
    }
}

void Student::add_grade(double grade, string teacher, string s_name, string s_surname, string type)
{
    char* messageError;
    string sql = "INSERT INTO GRADES(GRADE, ID_TEACHER, ID_STUDENT, TYPE) VALUES('"+to_string(grade)+"', (SELECT ID_TEACHER FROM TEACHERS WHERE "
    "LOGIN_T = '"+teacher+"'), (SELECT ID_STUDENT FROM STUDENTS WHERE S_FIRSTNAME = '"+s_name+"' AND S_LASTNAME = '"+s_surname+"'), '"+type+"')";

    int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if(process != SQLITE_OK)
        {
            cerr << "\nOne or more parameters are wrong. Grade not added!" << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nGrade added successfully!" << endl;
        }
}
int Student::callback(void * NotUsed, int argc, char** argv, char** azColName)
{
        for (int i = 0; i < argc; i++)
        {
            cout << azColName[i] << ": " << argv[i] << endl;
        }
        cout << endl;

        return 0;
}

int Student::log_callback(void * NotUsed, int argc, char** argv, char** azColName)
        {
            int check = atoi(argv[0]);
            if(check == 1)
            {
                return 3;
            }
            else
            {
                return 0;
            }
        }

void Student::display_grades()
{
    string sql = "SELECT g.GRADE, t.T_FIRSTNAME || ' ' || t.T_LASTNAME 'Teacher', s.S_FIRSTNAME || ' ' ||  s.S_LASTNAME 'Student', g.SUBJECT "
    "FROM GRADES g JOIN TEACHERS t ON t.ID_TEACHER = g.ID_TEACHER JOIN STUDENTS s ON s.ID_STUDENT = g.ID_STUDENT";

    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
}

void Student::display_students()
{
    string sql = "SELECT S_FIRSTNAME ||' '|| S_LASTNAME 'Student', CLASS FROM STUDENTS";

    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
}

void Student::display_student_grade(string name, string surname)
{
    string sql1 = "SELECT S_FIRSTNAME ||' '|| S_LASTNAME 'Student', CLASS FROM STUDENTS WHERE S_FIRSTNAME='"+name+"' AND S_LASTNAME='"+surname+"'";

    string sql2 = "SELECT g.GRADE, g.TYPE, t.SUBJECT FROM GRADES g JOIN TEACHERS t ON g.ID_TEACHER = t.ID_TEACHER JOIN STUDENTS s ON s.ID_STUDENT = g.ID_STUDENT WHERE s.S_FIRSTNAME = '"+name+"' AND s.S_LASTNAME = '"+surname+"' ORDER BY t.SUBJECT";

    string sql3 = "SELECT round(avg(g.GRADE),2) 'Average' FROM GRADES g JOIN STUDENTS s ON g.ID_STUDENT = s.ID_STUDENT WHERE s.S_FIRSTNAME = '"+name+"' AND s.S_LASTNAME = '"+surname+"'";

    sqlite3_exec(DB, sql1.c_str(), callback, NULL, NULL);
    sqlite3_exec(DB, sql2.c_str(), callback, NULL, NULL);
    sqlite3_exec(DB, sql3.c_str(), callback, NULL, NULL);
}

void Student::update_grade(double grade, double up_grade, string s_name, string s_surname, string teacher, string type, string up_type)
{
    char* messageError;

    string sql ="UPDATE GRADES SET GRADE = '"+to_string(up_grade)+"', TYPE = '"+up_type+"' WHERE ID_GRADE = (SELECT ID_GRADE "
 "FROM GRADES WHERE ID_STUDENT = (SELECT ID_STUDENT FROM STUDENTS WHERE S_FIRSTNAME = '"+s_name+"' AND S_LASTNAME = '"+s_surname+"') "
 "AND TYPE = '"+type+"' AND ID_TEACHER = (SELECT ID_TEACHER FROM TEACHERS WHERE LOGIN_T = '"+teacher+"') AND GRADE = '"+to_string(grade)+"' LIMIT 1)";
 int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if(process != SQLITE_OK)
        {
            cerr << "\nThere is no such grade!" << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nGrade updated successfully !" << endl;
        }
}

void Student::add_student(string name, string surname, string s_class, string teacher)
{
    char* messageError;
    string sql1 = "INSERT INTO STUDENTS(S_FIRSTNAME, S_LASTNAME, CLASS) VALUES('"+name+"', '"+surname+"', '"+s_class+"')";
    string sql2 = "INSERT INTO GRADES(GRADE, ID_TEACHER, ID_STUDENT, TYPE) "
    "VALUES(0, (SELECT ID_TEACHER FROM TEACHERS WHERE LOGIN_T = '"+teacher+"'), (SELECT ID_STUDENT FROM STUDENTS WHERE S_FIRSTNAME = '"+name+"' AND S_LASTNAME = '"+surname+"'), '0')";

    int process1 = sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messageError);
    if(process1 != SQLITE_OK)
        {
            cerr << "\nOne or more parameters are wrong. Student not added! " << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nStudent added successfully!" << endl;
        }
    int process2 = sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messageError);
    if(process2 != SQLITE_OK)
        {
            cerr << "\nOne or more parameters are wrong. Empty grade not added! " << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nEmpty Grade added successfully!" << endl;
        }

}

void Student::class_avg(string classs)
{
    string sql = "SELECT s.CLASS, round(avg(g.GRADE), 2) 'Average' FROM GRADES g JOIN STUDENTS s ON g.ID_STUDENT = s.ID_STUDENT WHERE s.CLASS = '"+classs+"'";

    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
}

void Student::subj_grades(string s_name, string s_surname, string teacher)
{
    string sql1 = "SELECT g.GRADE, g.TYPE FROM "
    "GRADES g JOIN STUDENTS s ON g.ID_STUDENT = s.ID_STUDENT JOIN TEACHERS t ON g.ID_TEACHER=t.ID_TEACHER WHERE "
    "s.S_FIRSTNAME = '"+s_name+"' AND s.S_LASTNAME = '"+s_surname+"' AND t.LOGIN_T = '"+teacher+"'";

    string sql2 = "SELECT t.SUBJECT, round(avg(g.GRADE), 2) 'Average' FROM "
    "GRADES g JOIN STUDENTS s ON g.ID_STUDENT = s.ID_STUDENT JOIN TEACHERS t ON g.ID_TEACHER=t.ID_TEACHER WHERE "
    "s.S_FIRSTNAME = '"+s_name+"' AND s.S_LASTNAME = '"+s_surname+"' AND t.LOGIN_T = '"+teacher+"'";

    sqlite3_exec(DB, sql1.c_str(), callback, NULL, NULL);
    sqlite3_exec(DB, sql2.c_str(), callback, NULL, NULL);
}

void Student::delete_grade(double grade, string s_name, string s_surname, string teacher, string type)
{
    char* messageError;
    string sql = "DELETE FROM GRADES "
"WHERE ID_GRADE = (SELECT ID_GRADE "
"FROM GRADES g "
"JOIN STUDENTS s "
"ON g.ID_STUDENT = s.ID_STUDENT "
"JOIN TEACHERS t "
"ON t.ID_TEACHER = g.ID_TEACHER "
"WHERE s.S_FIRSTNAME = '"+s_name+"' "
"AND s.S_LASTNAME = '"+s_surname+"' "
"AND t.LOGIN_T = '"+teacher+"' "
"AND GRADE = '"+to_string(grade)+"' "
"AND TYPE = '"+type+"' LIMIT 1)";

int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if(process != SQLITE_OK)
        {
            cerr << "\nOne or more parameters are wrong. Grade not deleted! " << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nGrade deleted successfully!" << endl;

}

}

void Student::delete_student(string name, string surname)
{
    char* messageError;
    string sql1 = "DELETE FROM GRADES WHERE ID_STUDENT = (SELECT ID_STUDENT FROM STUDENTS WHERE S_FIRSTNAME='"+name+"' AND S_LASTNAME='"+surname+"')";

    string sql2 = "DELETE FROM STUDENTS WHERE S_FIRSTNAME = '"+name+"' AND S_LASTNAME = '"+surname+"'";

    int process1 = sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messageError);
    if(process1 != SQLITE_OK)
        {
            cerr << "\nOne or more parameters are wrong. Student's grades not deleted! " << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nStudent's grades deleted successfully!" << endl;

    int process2 = sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messageError);
    if(process2 != SQLITE_OK)
        {
            cerr << "\nOne or more parameters are wrong. Student not deleted! " << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nStudent deleted successfully!" << endl;
        }
        }
}

int Student::logged_in(string login_, string psswd_)
{
        string sql = "SELECT CASE WHEN COUNT(PASSWD_T) > 0 THEN 1 ELSE 0 END AS CHECC FROM TEACHERS WHERE LOGIN_T = \""+login_+"\" AND PASSWD_T = \""+psswd_+"\";";

        int xx = sqlite3_exec(DB, sql.c_str(), log_callback, NULL, NULL);
        if(xx == 4)
        {
            currently_logged = login_;
        }
        else
        {
            return 0;
        }
}

void Student::display_teacher_students(string teacher)
{
    string sql = "SELECT s.S_FIRSTNAME ||' '|| s.S_LASTNAME 'Student', s.CLASS FROM "
    "STUDENTS s JOIN GRADES g ON s.ID_STUDENT = g.ID_STUDENT JOIN TEACHERS t ON g.ID_TEACHER=t.ID_TEACHER WHERE t.LOGIN_T='"+teacher+"'";

    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
}
