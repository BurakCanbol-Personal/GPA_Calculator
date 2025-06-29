#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

struct Course {
    std::string name;
    double grade;
    int credits;
};

std::vector<Course> courses;

void addCourse() {
    Course c;
    std::cout << "Course name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, c.name);

    std::cout << "Grade (0.0 - 10.0): ";
    std::cin >> c.grade;
    if (std::cin.fail() || c.grade < 0.0 || c.grade > 10.0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid grade. Must be between 0.0 and 10.0.\n";
        return;
    }

    std::cout << "Credits: ";
    std::cin >> c.credits;
    if (std::cin.fail() || c.credits <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid credit amount. Must be a positive number.\n";
        return;
    }

    courses.push_back(c);
    std::cout << "Course added.\n";
}


double calculateGPA() {
    double totalPoints = 0.0;
    int totalCredits = 0;
    for (const auto& c : courses) {
        totalPoints += c.grade * c.credits;
        totalCredits += c.credits;
    }
    return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
}

void viewGPA() {
    double gpa = calculateGPA();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Current GPA: " << gpa << "\n";
}

void estimateNeededGPA() {
    double targetGPA;
    int futureCredits;
    std::cout << "Target GPA: ";
    std::cin >> targetGPA;
    std::cout << "Remaining course credits (total): ";
    std::cin >> futureCredits;

    if (futureCredits <= 0) {
        std::cout << "Invalid credit amount.\n";
        return;
    }

    double currentPoints = 0.0;
    int currentCredits = 0;
    for (const auto& c : courses) {
        currentPoints += c.grade * c.credits;
        currentCredits += c.credits;
    }

    double requiredGPA = (targetGPA * (currentCredits + futureCredits) - currentPoints) / futureCredits;
    std::cout << std::fixed << std::setprecision(2);
    if (requiredGPA > 10.0) {
        std::cout << "⚠️ Not possible to reach target GPA with current data.\n";
    } else if (requiredGPA < 0.0) {
        std::cout << "🎉 You've already reached your target GPA!\n";
    } else {
        std::cout << "You need an average GPA of " << requiredGPA << " in your remaining courses.\n";
    }
}

void saveToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Error opening file for writing.\n";
        return;
    }
    for (const auto& c : courses) {
        out << c.name << '\n' << c.grade << ' ' << c.credits << '\n';
    }
    std::cout << "Courses saved to " << filename << "\n";
}

void loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "File not found.\n";
        return;
    }
    courses.clear();
    Course c;
    std::string line;
    while (std::getline(in, c.name)) {
        if (!(in >> c.grade >> c.credits)) break;
        in.ignore();
        courses.push_back(c);
    }
    std::cout << "Courses loaded from " << filename << "\n";
}

int main() {
    int choice;
    std::string filename = "grades.txt";

    do {
        std::cout << "\n=== GPA Tracker ===\n";
        std::cout << "1. Add course\n";
        std::cout << "2. View GPA\n";
        std::cout << "3. Estimate GPA needed for target\n";
        std::cout << "4. Save to file\n";
        std::cout << "5. Load from file\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: addCourse(); break;
            case 2: viewGPA(); break;
            case 3: estimateNeededGPA(); break;
            case 4: saveToFile(filename); break;
            case 5: loadFromFile(filename); break;
            case 0: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
