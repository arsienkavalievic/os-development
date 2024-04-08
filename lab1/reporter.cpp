#include "reporter.h"
#include <iostream>
#include <fstream>

struct employee {
    int num;
    char name[10];
    double hours;
};

void generateReport(const std::string& binaryFileName, const std::string& reportFileName, double payPerHour) {
    std::ifstream binaryFile(binaryFileName, std::ios::binary);
    if (!binaryFile) {
        std::cerr << "Failed to open binary file for reading." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ofstream reportFile(reportFileName);
    if (!reportFile) {
        std::cerr << "Failed to create report file." << std::endl;
        exit(EXIT_FAILURE);
    }

    reportFile << "Report for file: " << binaryFileName << std::endl;
    reportFile << "Number\tName\tHours\tSalary" << std::endl;

    employee emp;
    while (binaryFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        double salary = emp.hours * payPerHour;
        reportFile << emp.num << "\t" << emp.name << "\t" << emp.hours << "\t" << salary << std::endl;
    }

    binaryFile.close();
    reportFile.close();
}
