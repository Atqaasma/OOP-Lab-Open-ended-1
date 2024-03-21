#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Forward declarations
class Teacher;
class Student;
class Course;

// File handling class
class FileManager {
public:
    static void saveData(const vector<Student>& students, const vector<Teacher>& teachers, const vector<Course>& courses);
    static void loadData(vector<Student>& students, vector<Teacher>& teachers, vector<Course>& courses);
};

// Student class
class Student {
public:
    string studentID;
    string name;
    string email;
    vector<string> coursesEnrolled;

    Student(const string& id, const string& n, const string& e) : studentID(id), name(n), email(e) {}

    void enrollCourse(const string& courseCode) {
        coursesEnrolled.push_back(courseCode);
    }

    void dropCourse(const string& courseCode) {
        auto it = coursesEnrolled.begin();
        while (it != coursesEnrolled.end()) {
            if (*it == courseCode) {
                it = coursesEnrolled.erase(it);
            } else {
                ++it;
            }
        }
    }

    void viewCourses() const {
        cout << "Courses enrolled by " << name << ":" << endl;
        for (const auto& course : coursesEnrolled) {
            cout << course << endl;
        }
    }
};

// Teacher class
class Teacher {
public:
    string teacherID;
    string name;
    string email;
    vector<string> coursesTaught;

    Teacher(const string& id, const string& n, const string& e) : teacherID(id), name(n), email(e) {}

    void assignCourse(const string& courseCode) {
        coursesTaught.push_back(courseCode);
    }

    void removeCourse(const string& courseCode) {
        auto it = coursesTaught.begin();
        while (it != coursesTaught.end()) {
            if (*it == courseCode) {
                it = coursesTaught.erase(it);
            } else {
                ++it;
            }
        }
    }

    void viewCourses() const {
        cout << "Courses taught by " << name << ":" << endl;
        for (const auto& course : coursesTaught) {
            cout << course << endl;
        }
    }
};

// Course class
class Course {
public:
    string courseCode;
    string courseName;
    Teacher* teacher;
    vector<Student*> studentsEnrolled;
    int maxCapacity;

    Course(const string& code, const string& name, Teacher* t, int capacity) : courseCode(code), courseName(name), teacher(t), maxCapacity(capacity) {}

    bool addStudent(Student* student) {
        if (studentsEnrolled.size() < maxCapacity) {
            studentsEnrolled.push_back(student);
            student->enrollCourse(courseCode);
            return true;
        }
        return false;
    }

    void removeStudent(Student* student) {
        auto it = studentsEnrolled.begin();
        while (it != studentsEnrolled.end()) {
            if (*it == student) {
                it = studentsEnrolled.erase(it);
                student->dropCourse(courseCode);
            } else {
                ++it;
            }
        }
    }

    void viewStudents() const {
        cout << "Students enrolled in " << courseName << ":" << endl;
        for (const auto& student : studentsEnrolled) {
            cout << student->name << endl;
        }
    }
};

// File handling implementation
void FileManager::saveData(const vector<Student>& students, const vector<Teacher>& teachers, const vector<Course>& courses) {
    ofstream outFile("C:/Users/Admin/Desktop/OOP lab Open Ended/data.txt");
    if (!outFile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    // Write students
    for (const auto& student : students) {
        outFile << "STUDENT|" << student.studentID << "|" << student.name << "|" << student.email << "|" << student.coursesEnrolled.size();
        for (const auto& course : student.coursesEnrolled) {
            outFile << "|" << course;
        }
        outFile << endl;
    }

    // Write teachers
    for (const auto& teacher : teachers) {
        outFile << "TEACHER|" << teacher.teacherID << "|" << teacher.name << "|" << teacher.email << "|" << teacher.coursesTaught.size();
        for (const auto& course : teacher.coursesTaught) {
            outFile << "|" << course;
        }
        outFile << endl;
    }

    // Write courses
    for (const auto& course : courses) {
        outFile << "COURSE|" << course.courseCode << "|" << course.courseName << "|" << course.teacher->teacherID << "|" << course.studentsEnrolled.size();
        for (const auto& student : course.studentsEnrolled) {
            outFile << "|" << student->studentID;
        }
        outFile << endl;
    }

    outFile.close();
}

void FileManager::loadData(vector<Student>& students, vector<Teacher>& teachers, vector<Course>& courses) {
    ifstream inFile("C:/Users/Admin/Desktop/OOP lab Open Ended/data.txt");
    if (!inFile) {
        cerr << "Error opening file for reading!" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        string type, id, name, email;
        stringstream ss(line);
        getline(ss, type, '|');
        if (type == "STUDENT") {
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            int numCourses;
            ss >> numCourses;

            Student student(id, name, email);
            string courseCode;
            for (int i = 0; i < numCourses; ++i) {
                getline(ss, courseCode, '|');
                student.enrollCourse(courseCode);
            }
            students.push_back(student);
        } else if (type == "TEACHER") {
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            int numCourses;
            ss >> numCourses;

            Teacher teacher(id, name, email);
            string courseCode;
            for (int i = 0; i < numCourses; ++i) {
                getline(ss, courseCode, '|');
                teacher.assignCourse(courseCode);
            }
            teachers.push_back(teacher);
        } else if (type == "COURSE") {
            getline(ss, id, '|');
            getline(ss, name, '|');
            string teacherID;
            getline(ss, teacherID, '|');
            int numStudents;
            ss >> numStudents;

            // Find the teacher by ID
            Teacher* teacherPtr = nullptr;
            for (auto& teacher : teachers) {
                if (teacher.teacherID == teacherID) {
                    teacherPtr = &teacher;
                    break;
                }
            }
            if (teacherPtr == nullptr) {
                cerr << "Error: Teacher not found for course " << id << endl;
                continue;
            }

            Course course(id, name, teacherPtr, numStudents);
            string studentID;
            for (int i = 0; i < numStudents; ++i) {
                getline(ss, studentID, '|');
                // Find the student by ID
                for (auto& student : students) {
                    if (student.studentID == studentID) {
                        course.addStudent(&student);
                        break;
                    }
                }
            }
            courses.push_back(course);
        }
    }

    inFile.close();
}

int main() {
    vector<Student> students;
    vector<Teacher> teachers;
    vector<Course> courses;

    FileManager::loadData(students, teachers, courses);

    // Display loaded data
    cout << "Students:" << endl;
    for (const auto& student : students) {
        cout << "ID: " << student.studentID << ", Name: " << student.name << ", Email: " << student.email << endl;
        student.viewCourses();
        cout << endl;
    }

    cout << "Teachers:" << endl;
    for (const auto& teacher : teachers) {
        cout << "ID: " << teacher.teacherID << ", Name: " << teacher.name << ", Email: " << teacher.email << endl;
        teacher.viewCourses();
        cout << endl;
    }

    cout << "Courses:" << endl;
    for (const auto& course : courses) {
        cout << "Code: " << course.courseCode << ", Name: " << course.courseName << endl;
        course.viewStudents();
        cout << endl;
    }

    return 0;
}
