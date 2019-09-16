//Joshua Guempel
//CS 3800
//File system header

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <iostream>
#include <unordered_map>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <ctime>
#include <math.h>
#include <algorithm>

struct file {
    std::string name = "UNNAMED";
    std::string permissions = "----------";
    std::string date = "----------";
    file(){}
    file(const std::string& file_name);
};

struct folder { 
    std::string name = "UNNAMED";
    std::string permissions = "----------";
    std::string date = "----------";

    //The main implementation of this system: map of strings to folder/file pointers,
    //with a pointer to the parent for traversal, NOT resource management 
    folder* parent = nullptr;
    std::unordered_map<std::string, std::shared_ptr<folder>> folders;
    std::unordered_map<std::string, std::shared_ptr<file>> files;

    folder(){}
    folder(const std::string& folder_name, folder* folder_parent = nullptr);
};

class shell {
private:
    folder* curr_dir = nullptr; //Pointer for traversal
    folder root_folder;
public:
    //list file in current directory
    void ls(const std::string& args);

    //Connect to a directory
    void cd(const std::string& args);

    //Print directory and parents
    void pwd();

    //print directory for shell (slightly different than pwd)
    void print_dir_for_shell();

    //Make a directory  
    void mkdir(const std::string& args);

    //remove a directory
    void rmdir(const std::string& args);

    //remove a file
    void rm(const std::string& args);

    //Change permissions
    void chmod(const std::string& args, const std::string& args1);

    //Create a file
    void touch(const std::string& args);

    //Quit
    void quit();

    //Parse out command arguments and call one of the above functions
    void process_command(const std::string& args);

    shell(){}

    //construct shell with root folder (/)
    shell(folder& root_dir);

};

//get date in a formatted string.
std::string get_date();

//convert octal to binary
std::vector<int> octal_to_binary(std::string octal_num);

#endif
