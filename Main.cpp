#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <direct.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

bool fileExists(const string& fileName) {
    DWORD attributes = GetFileAttributesA(fileName.c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool isEmpty(const string& folderName) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((folderName + "/*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        return false; // Not empty
    }
    else {
        return true; // Empty
    }
}

class FileCommandLine {
private:
    string currDir; // to store the current directory pathway
public:
    FileCommandLine() {
        WCHAR buffer[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, buffer);
        currDir = WCHAR(buffer) + "\\";
    }

    string getFileName() {
        string fileName;
        cout << "Enter file name: ";
        cin >> fileName;

        fileName = currDir + fileName;

        return fileName;
    }

    void mkdir(const string& directoryName) {  // creates a directory (folder)
        if (fileExists(directoryName)) {
            cerr << "Error: Directory already exists: " << directoryName << "\n\n";
            return;
        }

        if (CreateDirectoryA(directoryName.c_str(), NULL)) {
            cout << "Directory created successfully: " << directoryName << "\n\n";
        }
        else {
            cerr << "Error creating directory: " << directoryName << "\n\n";
        }
    }

    void rmdir(const string& directoryName) { // deletes a directory (folder)
        if (!fileExists(directoryName)) {
            cerr << "Error: Directory does not exist: " << directoryName << "\n\n";
            return;
        }

        if (!isEmpty(directoryName)) {
            cerr << "Error: Directory is not empty. Cannot be removed: " << directoryName << "\n\n";
            return;
        }

        if (RemoveDirectoryA(directoryName.c_str())) {
            cout << "Directory removed successfully: " << directoryName << "\n\n";
        }
        else {
            cerr << "Error removing directory: " << directoryName << "\n\n";
        }
    }

    void touchFile(const string& fileName) { // edits or creates a file.txt
        if (fileExists(fileName)) {
            cerr << "Error: File already exists: " << fileName << "\n\n";
            return;
        }

        ofstream file(fileName);

        if (file.is_open()) {
            file.close();
            cout << "File created successfully." << "\n\n";
        }
        else {
            cout << "Error creating file." << "\n\n";
        }
    }

    void catFile(const string& fileName) { // display file content
        ifstream file(fileName);

        if (file.is_open()) {
            string textLine;
            cout << endl << "------------------------------" << endl;
            while (getline(file, textLine)) {
                cout << textLine << endl;
            }
            file.close();
        }
        else {
            cout << "Error displaying file content.\n";
        }
    }

    void rmFile(const string& fileName) { // delete file
        if (!fileExists(fileName)) {
            cerr << "Error: File does not exist: " << fileName << "\n\n";
            return;
        }

        if (DeleteFileA(fileName.c_str())) {
            cout << "File deleted successfully." << "\n\n";
        }
        else {
            cout << "Error occurred, deletion failed." << "\n\n";
        }
    }

    void cd(const string& directoryName) { // change directory path
        if (!SetCurrentDirectoryA(directoryName.c_str())) {
            cerr << "Error: Directory does not exist: " << directoryName << "\n\n";
            return;
        }

        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        currDir = string(buffer) + "\\";
        cout << "Directory changed successfully: " << currDir << "\n\n";
    }

    void topScreen() { // top screen styling
        for (int i = 0; i < 120; i++)
            cout << "=";
        cout << endl;
    }

    void title() { // project title
        topScreen();
        cout << "\t\t\t\t\t" << "----------------------------------------" << endl
            << "\t\t\t\t\t\t" << "Basic File Manager CMD" << endl
            << "\t\t\t\t\t" << "----------------------------------------" << endl << endl;
    }

    void program() { // menu
        string command;// hostname = "user@host: " + currDir;
        bool loop = true;

        while (loop) {
            title();

            cout << "\t\t" << "Type the desired command" << endl
                << "\t\t" << "(1) mkdir -> create a directory" << endl
                << "\t\t" << "(2) rmdir -> delete a directory" << endl
                << "\t\t" << "(3) touch -> create/modify text file" << endl
                << "\t\t" << "(4) rm    -> delete text file" << endl
                << "\t\t" << "(5) cat   -> display content of text file" << endl
                << "\t\t" << "(6) cd    -> change directory" << endl
                << "\t\t" << "(7) cls   -> clear screen" << endl
                << "\t\t" << "(8) exit  -> end program" << endl
                << "\t\t" << currDir;
            cin >> command;

            if (command == "mkdir") {
                string fileName = getFileName();
                mkdir(fileName);
            }
            else if (command == "rmdir") {
                string fileName = getFileName();
                rmdir(fileName);
            }
            else if (command == "touch") {
                string fileName = getFileName();
                touchFile(fileName);
            }
            else if (command == "rm") {
                string fileName = getFileName();
                rmFile(fileName);
            }
            else if (command == "cat") {
                string fileName = getFileName();
                catFile(fileName);
            }
            else if (command == "cd") {
                string directoryName;
                cout << "Enter directory name: ";
                cin >> directoryName;
                cd(directoryName);
            }
            else if (command == "cls") {
                system("cls");
            }
            else if (command == "exit") {
                break;
            }
            else {
                cout << "Invalid command.\n";
            }
        }
    }

};

int main()
{
    FileCommandLine cmd;
    cmd.program();

    return 0;
}
