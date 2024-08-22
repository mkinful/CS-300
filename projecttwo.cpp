#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <numeric>

using namespace std;

// Course Structure
struct Course {
    string courseNumber;
    string title;
    list<string> prerequisites;
};

// Function to parse a line into a Course object
Course parseCourse(const string& line) {
    stringstream ss(line);
    string number, title, prereq;
    getline(ss, number, ',');
    getline(ss, title, ',');
    Course course = {number, title};

    while (getline(ss, prereq, ',')) {
        course.prerequisites.push_back(prereq);
    }
    return course;
}

// Option 1: Using Vector
vector<Course> loadVectorData(const string& filename) {
    vector<Course> courses;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        courses.push_back(parseCourse(line));
    }
    return courses;
}

void printVectorCourseInformation(const vector<Course>& courses, const string& courseNumber) {
    for (const auto& course : courses) {
        if (course.courseNumber == courseNumber) {
            cout << "Course: " << course.title << endl;
            cout << "Prerequisites: ";
            for (const auto& prereq : course.prerequisites) {
                cout << prereq << " ";
            }
            cout << endl;
            return;
        }
    }
    cout << "Course not found." << endl;
}

void printSortedVectorCourses(vector<Course>& courses) {
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });

    for (const auto& course : courses) {
        cout << course.courseNumber << ": " << course.title << endl;
    }
}

// Option 2: Using HashTable
class HashTable {
    static const int SIZE = 100;
    list<Course> table[SIZE];

    int hash(const string& key) {
        return accumulate(key.begin(), key.end(), 0) % SIZE;
    }

public:
    void insert(const Course& course) {
        table[hash(course.courseNumber)].push_back(course);
    }

    Course* find(const string& courseNumber) {
        int index = hash(courseNumber);
        for (auto& course : table[index]) {
            if (course.courseNumber == courseNumber) {
                return &course;
            }
        }
        return nullptr;
    }

    vector<Course> getAllCourses() {
        vector<Course> courses;
        for (int i = 0; i < SIZE; ++i) {
            for (const auto& course : table[i]) {
                courses.push_back(course);
            }
        }
        return courses;
    }
};

HashTable loadHashTableData(const string& filename) {
    HashTable ht;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        ht.insert(parseCourse(line));
    }
    return ht;
}

void printHashTableCourseInformation(HashTable& ht, const string& courseNumber) {
    Course* course = ht.find(courseNumber);
    if (course) {
        cout << "Course: " << course->title << endl;
        cout << "Prerequisites: ";
        for (const auto& prereq : course->prerequisites) {
            cout << prereq << " ";
        }
        cout << endl;
    } else {
        cout << "Course not found." << endl;
    }
}

void printSortedHashTableCourses(HashTable& ht) {
    vector<Course> courses = ht.getAllCourses();
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });

    for (const auto& course : courses) {
        cout << course.courseNumber << ": " << course.title << endl;
    }
}

// Option 3: Using Tree (Binary Search Tree)
struct TreeNode {
    Course course;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};

class Tree {
    TreeNode* root;

    void insert(TreeNode*& node, const Course& course) {
        if (!node) {
            node = new TreeNode(course);
        } else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        } else {
            insert(node->right, course);
        }
    }

    Course* find(TreeNode* node, const string& courseNumber) {
        if (!node) return nullptr;
        if (node->course.courseNumber == courseNumber) return &node->course;
        if (courseNumber < node->course.courseNumber) return find(node->left, courseNumber);
        return find(node->right, courseNumber);
    }

    void inOrderTraversal(TreeNode* node, vector<Course>& courses) {
        if (!node) return;
        inOrderTraversal(node->left, courses);
        courses.push_back(node->course);
        inOrderTraversal(node->right, courses);
    }

public:
    Tree() : root(nullptr) {}

    void insert(const Course& course) {
        insert(root, course);
    }

    Course* find(const string& courseNumber) {
        return find(root, courseNumber);
    }

    vector<Course> getSortedCourses() {
        vector<Course> courses;
        inOrderTraversal(root, courses);
        return courses;
    }
};

Tree loadTreeData(const string& filename) {
    Tree tree;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        tree.insert(parseCourse(line));
    }
    return tree;
}

void printTreeCourseInformation(Tree& tree, const string& courseNumber) {
    Course* course = tree.find(courseNumber);
    if (course) {
        cout << "Course: " << course->title << endl;
        cout << "Prerequisites: ";
        for (const auto& prereq : course->prerequisites) {
            cout << prereq << " ";
        }
        cout << endl;
    } else {
        cout << "Course not found." << endl;
    }
}

void printSortedTreeCourses(Tree& tree) {
    vector<Course> courses = tree.getSortedCourses();
    for (const auto& course : courses) {
        cout << course.courseNumber << ": " << course.title << endl;
    }
}

// Menu function
void menu() {
    int structureChoice;
    cout << "Choose Data Structure:\n1. Vector\n2. HashTable\n3. Tree\n";
    cout << "Enter your choice: ";
    cin >> structureChoice;

    int choice;
    do {
        cout << "\nMenu:\n1. Load course data\n2. Print sorted courses\n3. Print course information\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (structureChoice == 1) { // Vector
            static vector<Course> courses;
            if (choice == 1) {
                courses = loadVectorData("data.csv");
            } else if (choice == 2) {
                printSortedVectorCourses(courses);
            } else if (choice == 3) {
                string courseNumber;
                cout << "Enter course number: ";
                cin >> courseNumber;
                printVectorCourseInformation(courses, courseNumber);
            }
        } else if (structureChoice == 2) { // HashTable
            static HashTable ht;
            if (choice == 1) {
                ht = loadHashTableData("data.csv");
            } else if (choice == 2) {
                printSortedHashTableCourses(ht);
            } else if (choice == 3) {
                string courseNumber;
                cout << "Enter course number: ";
                cin >> courseNumber;
                printHashTableCourseInformation(ht, courseNumber);
            }
        } else if (structureChoice == 3) { // Tree
            static Tree tree;
            if (choice == 1) {
                tree = loadTreeData("data.csv");
            } else if (choice == 2) {
                printSortedTreeCourses(tree);
            } else if (choice == 3) {
                string courseNumber;
                cout << "Enter course number: ";
                cin >> courseNumber;
                printTreeCourseInformation(tree, courseNumber);
            }
        } else {
            cout << "Invalid data structure choice." << endl;
            break;
        }

        if (choice == 9) {
            cout << "Exiting program." << endl;
        } else if (choice != 1 && choice != 2 && choice != 3) {
            cout << "Invalid choice." << endl;
        }
    } while (choice != 9);
}

int main() {
    menu();
    return 0;
}