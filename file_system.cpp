#include "file_system.h"

//List files in current directory
void shell::ls(const std::string& args) {
    //Return if folder is empty or if dir pointer isnt pointing to anything
    if(curr_dir->files.empty() && curr_dir->folders.empty()) {
        return;
    }
    if(curr_dir == nullptr) {
        std::cerr << "Something went wrong, the shell object isnt pointing to a folder for some reason..." << std::endl;
        return;
    }
    if(args == "-l") {
        for (auto const& file : curr_dir->files) {
            std::cout << file.second->permissions << " 1 pbg  user  0 " 
                      << file.second->date << " " << file.second->name << std::endl;
        }
        for (auto const& folder : curr_dir->folders) {
            std::cout << folder.second->permissions << " 1 pbg  user  0 " 
                      << folder.second->date << " " << folder.second->name << std::endl;
        }
    } else {
        for (auto const& file : curr_dir->files) {
            std::cout << file.first << "   ";	
        }
        for (auto const& folder : curr_dir->folders) {
            std::cout << folder.first << "   ";
        }
    }
    std::cout << std::endl;
}

//Connect to another directory
void shell::cd(const std::string& args) {
    if(args == ".." || args == "../") { //Move up a level
        if(curr_dir->parent != nullptr)
            curr_dir = curr_dir->parent;
        return;
    }
    if(curr_dir->folders.count(args) == 0) {
        std::cout << "Folder doesn't exist." << std::endl;
        return;
    }

    //Move the curr_dir pointer to the folder
    curr_dir = curr_dir->folders[args].get();
}

//Prints directory and parents
void shell::pwd() {
    std::stack<std::string> parents;
    for(folder* i = curr_dir; i != nullptr; i = i->parent)
        parents.push(i->name);
    std::cout << "user:~/";

    if(!parents.empty())
        parents.pop();

    while(!parents.empty()) {
        std::cout << parents.top() << "/";
        parents.pop();
    }
    std::cout << std::endl;
}

//print directory for shell (slightly different than pwd)
void shell::print_dir_for_shell() {
    std::stack<std::string> parents;
    for(folder* i = curr_dir; i != nullptr; i = i->parent)
        parents.push(i->name);
    std::cout << "user:~/";

    if(!parents.empty())
        parents.pop();

    while(!parents.empty()) {
        std::cout << parents.top() << "/";
        parents.pop();
    }
    std::cout << "$ ";
}

//Makes folder in current directory
void shell::mkdir(const std::string& args) {
    if(curr_dir->folders.count(args) == 1) {
	std::cout << "Folder already exists." << std::endl;
	return;
    }
    curr_dir->folders[args] = std::make_shared<folder>(args, curr_dir);
}

//Removes folder in current directory
void shell::rmdir(const std::string& args) {
    if(curr_dir->folders.count(args) == 0) {
        std::cout << "Folder does not exist." << std::endl;
        return;
    }
    curr_dir->folders.erase(args);
}

//removes file in current directory
void shell::rm(const std::string& args) {
   if(curr_dir->files.count(args) == 0) {
        std::cout << "File does not exist." << std::endl;
        return;
    }
    curr_dir->files.erase(args);
}

//Change permissions of a file
void shell::chmod(const std::string& args, const std::string& args1) {
    if(curr_dir->files.count(args1) == 0 && curr_dir->folders.count(args1) == 0) {
        std::cout << "File does not exist." << std::endl;
        return;
    }
    //Need to convert XXX to -rwxrwxrwx by first converting XXX in octal to binary
    auto permissions_binary = octal_to_binary(args);
    std::string p_777 = "-rwxrwxrwx";
    std::string new_p = "----------";

    for(unsigned int i = 0; i < permissions_binary.size(); i++) {
        if(permissions_binary[i] == 1) {
            new_p[i+1] = p_777[i+1];
        }
    }
    if(curr_dir->files.count(args1)) {  //it's a file curr_dir->files[args1]->permissions
        curr_dir->files[args1]->permissions = new_p;
    } else {  
	//it's a folder, curr_dir->folders[args1]->permissions
        curr_dir->folders[args1]->permissions = new_p;
    }
}

//Creates file in current directory
void shell::touch(const std::string& args) {
    if(curr_dir->files.count(args)) { //If file is already there just update the date
	curr_dir->files[args]->date = get_date();
    } else {
    	curr_dir->files[args] = std::make_shared<file>(args);
    }
}

//Quit.
void shell::quit() {
    return;
}

shell::shell(folder& root_dir) {
    root_folder = root_dir;
    curr_dir = &root_folder; 
}

folder::folder(const std::string& folder_name, folder* folder_parent) {
    name = folder_name;
    parent = folder_parent;
    permissions = "-rw-r--r--";
    date = get_date();
}

file::file(const std::string& file_name) {
    name = file_name;
    permissions = "-rw-r--r--";
    date = get_date();
}

void shell::process_command(const std::string& args) {
    std::vector<std::string> args_sep;
    std::string arg_buf = "";
    for(const auto& c : args) {
        if(c == ' ') {
            if(arg_buf.length() > 0) {
                args_sep.push_back(arg_buf);
            }
            arg_buf = "";
        } else {
            arg_buf += c;
        } 
    }
    if(arg_buf.length() > 0) {
        args_sep.push_back(arg_buf);
    }
    arg_buf = "";
    if(args_sep.size() == 0) {
        std::cerr << "Error, no command was read through input." << std::endl;
        return; 
    }

    //C++ doesn't support switchcases on strings, RIP

    //ls
    if(args_sep[0] == "ls") {
        if(args_sep.size() == 2) {
            if(args_sep[1] == "-l") {
                ls("-l");
            } else {
                std::cout << "Invalid syntax" << std::endl;
            }
        } else if(args_sep.size() == 1) {
            ls("");
        } else {
            std::cout << "Invalid syntax" << std::endl;
        }

    //cd
    } else if(args_sep[0] == "cd") {
        if(args_sep.size() != 2) {
            std::cout << "too many arguments" << std::endl;
            return;
        } else {
            cd(args_sep[1]);
        }

    //pwd
    } else if (args_sep[0] == "pwd") {
        pwd();

    //mkdir
    } else if (args_sep[0] == "mkdir") {
        //fg++ gives compiler warnings if i var isnt unsigned... so I made it unsigned
        for(unsigned int i = 1; i < args_sep.size(); i++) {
            mkdir(args_sep[i]);
        }

    //rmdir
    } else if (args_sep[0] == "rmdir") {
        for(unsigned int i = 1; i < args_sep.size(); i++) {
            rmdir(args_sep[i]);
        }

    //rm
    } else if (args_sep[0] == "rm") {
        for(unsigned int i = 1; i < args_sep.size(); i++) {
            rm(args_sep[i]);
        }

    //chmod
    } else if (args_sep[0] == "chmod") {
        if(args_sep.size() != 3) {
            std::cout << "invalid syntax" << std::endl;
        } else {
            chmod(args_sep[1], args_sep[2]);
        }
    
    //touch
    } else if (args_sep[0] == "touch") {
        for(unsigned int i = 1; i < args_sep.size(); i++) {
            touch(args_sep[i]);
        }
   
    //quit
    } else if (args_sep[0] == "quit" || args_sep[0] == "exit") {
        quit();
    } else {
        std::cout << args_sep[0] << ": Command not found" << std::endl;
    }
}

//get date in a formatted string.
std::string get_date() {
    time_t time_now = time(0);
    std::string date_time = ctime(&time_now);
    int i = date_time.size() - 1;
    while(i >= 0 && date_time[i] != ':')
        i--;
    date_time.erase(i,date_time.size());
    i = 0;
    while(date_time[i] != ' ' && i < date_time.size())
        i++;
    date_time.erase(0, i+1);
    return date_time;
}

//Convert octal number into binary number. (successive divion)
std::vector<int> octal_to_binary(std::string octal_num) {
    std::vector<int> binary_num;

    int octal_num_b10 = 0;
    int index = octal_num.length()-1;

    //Convert octal_num to b10
    for(unsigned int i = 0; i < octal_num.length(); i++) {
        octal_num_b10 += (octal_num[i] - '0') * pow(8,index);
        index--;
    }

    if(octal_num_b10 == 0) {
        binary_num.push_back(0);
    } else {
        while(octal_num_b10 > 0) {
            int remainder = octal_num_b10 % 2;
            binary_num.push_back(remainder);
            octal_num_b10 /= 2;
        }
    }
    std::reverse(binary_num.begin(), binary_num.end());
    return binary_num;
}










