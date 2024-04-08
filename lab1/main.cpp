#include <iostream>
#include "creator.h"
#include "reporter.h"

int main() {
    std::string binaryFileName, reportFileName;
    int numRecords;
    double payPerHour;

    std::cout << "Enter binary file name: ";
    std::cin >> binaryFileName;
    std::cout << "Enter number of records: ";
    std::cin >> numRecords;

    createBinaryFile(binaryFileName, numRecords);

    std::cout << "Binary file contents:" << std::endl;
    displayBinaryFileContents(binaryFileName);

    std::cout << "Enter report file name: ";
    std::cin >> reportFileName;
    std::cout << "Enter pay per hour: ";
    std::cin >> payPerHour;

    generateReport(binaryFileName, reportFileName, payPerHour);

    std::cout << "Report generated successfully." << std::endl;

    return 0;
}