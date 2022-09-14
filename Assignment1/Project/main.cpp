#include <iostream>
#include <fstream>
using namespace std;

int main() {
    std::cout << "Hello, CIS6030!" << std::endl;
    // write to a file is easy:
    // Create a text file
    ofstream MyWriteFile("filename.txt");
    // Write to the file
    MyWriteFile << "Files can be tricky, but it is fun enough!";
    // Close the file
    MyWriteFile.close();



    // need to study how to read file with cpp
    // Create a text string, which is used to output the text file
    string myText;
    // Read from the text file
    ifstream MyReadFile("filename.txt");
    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        // Output the text from the file
        cout << myText;
    }


    return 0;
}
