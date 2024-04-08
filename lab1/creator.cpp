#include "creator.h"
#include <iostream>
#include <fstream>

struct employee {
    int num;
    char name[10];
    double hours;
};

void createBinaryFile(const std::string& fileName, int numRecords) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create binary file." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numRecords; ++i) {
        employee emp;
        std::cout << "Enter employee number: ";
        std::cin >> emp.num;
        std::cout << "Enter employee name: ";
        std::cin >> emp.name;
        std::cout << "Enter number of hours worked: ";
        std::cin >> emp.hours;

        file.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    file.close();
}

void displayBinaryFileContents(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open binary file for reading." << std::endl;
        exit(EXIT_FAILURE);
    }

    employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << "Number: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << std::endl;
    }

    file.close();
}
