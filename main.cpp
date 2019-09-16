//Joshua Guempel
//cs3800
//Fabricated files n' folders

#include "file_system.h"

int main() {
    folder _root = folder("/", nullptr); //Construct shell, root folder
    shell _shell = shell(_root);
    std::string _input;
    bool quit = false;
    while(!quit) { //Main loop: take input, process command
        _shell.print_dir_for_shell();
        std::getline(std::cin, _input);
        if(_input == "quit" || _input == "exit")
            quit = true;
        _shell.process_command(_input);
    }
    return 0;
}
