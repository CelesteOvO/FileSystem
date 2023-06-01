//
// Created by 86151 on 2023/5/29.
//

#include "FileSystemInterface.h"

void FileSystemInterface::start() {
    displayWelcomeMessage();

    while (true) {
        displayPrompt();
        std::string command = getUserInput();
        executeCommand(command);
    }
}

void FileSystemInterface::executeCommand(const std::string &command) const {
    std::vector<std::string> args = parseCommandArgs(command);
    if (args.empty()) {
        return;
    }

    std::string cmd = args[0];
    //args.erase(args.begin());

    if (cmd == "cd") {
        handleCd(args);
    } else if (cmd == "dir") {
        handleDir(args);
    } else if (cmd == "mkdir") {
        handleMkdir(args);
    } else if (cmd == "rmdir") {
        handleRmdir(args);
    } else if (cmd == "create") {
        handleCreate(args);
    } else if (cmd == "open") {
        handleOpen(args);
    } else if (cmd == "read") {
        handleRead(args);
    } else if (cmd == "write") {
        handleWrite(args);
    } else if (cmd == "close") {
        handleClose(args);
    } else if (cmd == "lseek") {
        handleLseek(args);
    } else if (cmd == "help") {
        handleHelp();
    } else if (cmd == "time") {
        handleTime();
    } else if (cmd == "ver") {
        handleVer();
    } else if (cmd == "rename") {
        handleRename(args);
    } else if (cmd == "import") {
        handleImport(args);
    } else if (cmd == "export") {
        handleExport(args);
    } else if (cmd == "exit") {
        std::cout <<  "Exit successfully." << std::endl;
        fileSystem->clearCurrentPointers();
        exit(0);
    } else if (cmd == "save") {
        std::cout << "Saving file system..." << std::endl;
        fileSystem->saveFileSystem(*fileSystem, "liyifan"); /// TODO
    } else if ( cmd == "load"){
        std::cout << "Loading file system..." << std::endl;
        loadFileSystem("liyifan");
    }
    else {
        std::cout << "Unknown command. Type 'help' for command list." << std::endl;
    }
}

std::vector<std::string> FileSystemInterface::parseCommandArgs(const std::string &command) {
    std::vector<std::string> args;
    std::istringstream iss(command);
    std::string arg;

    // 使用空格作为分隔符逐个获取参数
    while (iss >> arg) {
        args.push_back(arg);
    }

    return args;
}

void FileSystemInterface::displayPrompt() const {
    // 获取当前用户信息
    // std::string username = currentUser->username;

    // 获取当前目录信息
    std::string currentDir = fileSystem->getDirectoryPath(fileSystem->currentDirectory);

    // 显示命令提示符
    std::cout /*<< username << "\\" */<< currentDir /*<< ">"*/;
}

void FileSystemInterface::displayWelcomeMessage() {
    std::cout << "Welcome to the File System Interface!" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "This interface allows you to interact with the file system." << std::endl;
    std::cout << "Enter commands to perform various operations." << std::endl;
    std::cout << "Use the 'help' command to display available commands." << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

std::string FileSystemInterface::getUserInput() {
    std::string userInput;
    std::cout << "> ";  // 显示输入提示符

    std::getline(std::cin, userInput);  // 读取用户输入

    return userInput;
}

void FileSystemInterface::handleCd(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: cd <directory>" << std::endl;
        return;
    }

    const std::string& directoryPath = args[1];

    // 调用文件系统类的切换目录方法
    fileSystem->changeDirectory(directoryPath);
}

void FileSystemInterface::handleDir(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 1) {
        std::cout << "Invalid command. Usage: dir" << std::endl;
        return;
    }

    // 调用文件系统类的列出目录方法
    fileSystem->listDirectory();
}

void FileSystemInterface::handleMkdir(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: mkdir <directory>" << std::endl;
        return;
    }

    const std::string& directoryName = args[1];

    // 调用文件系统类的创建目录方法
    fileSystem->createDirectory(directoryName);
}

void FileSystemInterface::handleRmdir(const std::vector<std::string> &args) const {
// 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: rmdir <directory_name>" << std::endl;
        return;
    }

    // 获取目录名称
    const std::string& directoryName = args[1];

    // 调用文件系统类的删除目录方法
    fileSystem->removeDirectory(directoryName);
}

void FileSystemInterface::handleCreate(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: create <file_name>" << std::endl;
        return;
    }

    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的创建文件方法
    fileSystem->createFile(fileName, "");
}

void FileSystemInterface::handleOpen(const std::vector<std::string> &args) const {
// 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: open <file_name>" << std::endl;
        return;
    }

    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的打开文件方法
    fileSystem->openFile(fileName);
}

void FileSystemInterface::handleRead(const std::vector<std::string> &args) const  {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: read <file_name>" << std::endl;
        return;
    }

    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的打开文件方法
    fileSystem->openFile(fileName);

    // 读取文件内容
    fileSystem->readFile();

    // 关闭文件
    fileSystem->closeFile();
}

void FileSystemInterface::handleWrite(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: write <file_name>" << std::endl;
        return;
    }
    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的打开文件方法
    fileSystem->openFile(fileName);

    // 检查文件是否已打开
    if (fileSystem->currentFile == nullptr) {
        std::cout << "No file is currently open." << std::endl;
        return;
    }

    if(fileSystem->currentFile->content.empty()){
        // 获取用户输入的内容
        std::string content;
        std::cout << "Enter the content to write: ";
        std::getline(std::cin, content);

        // 调用文件系统类的重写文件内容方法
        fileSystem->ReWriteFile(content);
        fileSystem->closeFile();
    }else{
        std::cout << "The content of the file is: " << fileSystem->currentFile->content << std::endl;
        std::cout << "Do you want to append to the file? (y/n) " << std::endl;
        std::string answer;
        std::cin >> answer;
        if(answer == "y" || answer == "Y"){
            std::cout << "Please select append mode: " << std::endl;
            std::cout << "1. Append to the end of the file" << std::endl;
            std::cout << "2. Write at a specific position" << std::endl;
            int type;
            std::cin >> type;
            if(type == 1)
            {
                // 获取用户输入的内容
                std::string content;
                std::cout << "Enter the content to write: ";
                std::cin >> content;
                // 调用文件系统类的写文件内容方法
                fileSystem->appendFileAtPosition(content, fileSystem->currentFile->content.size());
                std::cout << "Content has been written to the file." << std::endl;
                fileSystem->closeFile();
            }else if(type == 2) {
                std::cout << "Current File pointer is at position: " << fileSystem-> currentFilePointer << std::endl;
                std::cout << "Do you want to change the file pointer? (y/n) " << std::endl;
                std::string answer1;
                std::cin >> answer1;
                if(answer1 == "y" || answer1 == "Y"){
                    std::cout << "Enter the position to write: ";
                    int position;
                    std::cin >> position;
                    fileSystem->currentFilePointer = position;
                }
                std::cout << "Enter the content to write: ";
                std::string content;
                std::cin >> content;
                fileSystem->appendFileAtPosition(content, fileSystem->currentFilePointer);
                std::cout << "Content has been written to the file." << std::endl;
                fileSystem->closeFile();
            }else{
                std::cout << "Invalid command." << std::endl;
                fileSystem->closeFile();
                return;
            }
        }else{
            fileSystem->closeFile();
            std::cout << "Closing the file..." << std::endl;
        }
    }
}

void FileSystemInterface::handleClose(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 1) {
        std::cout << "Invalid command. Usage: close" << std::endl;
        return;
    }

    // 检查是否有文件被打开
    if (fileSystem->currentFile == nullptr) {
        std::cout << "No file is currently open." << std::endl;
        return;
    }

    // 关闭文件
    fileSystem->closeFile();

    std::cout << "File has been closed." << std::endl;
}

void FileSystemInterface::handleLseek(const std::vector<std::string> &args) const {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        std::cout << "Invalid command. Usage: lseek <position>" << std::endl;
        return;
    }

    // 检查是否有文件被打开
    if (fileSystem->currentFile == nullptr) {
        std::cout << "No file is currently open." << std::endl;
        return;
    }

    // 解析位置参数
    int position;
    try {
        position = std::stoi(args[1]);
    } catch (std::exception& e) {
        std::cout << "Invalid position value. Please enter a valid integer." << std::endl;
        return;
    }

    // 定位文件指针
    fileSystem->seekFile(position);

    // std::cout << "File pointer has been set to position " << position << std::endl;
}

void FileSystemInterface::handleHelp() {
    std::cout << "========================= Help =========================" << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "cd <directory>      : Change current directory." << std::endl;
    std::cout << "dir                 : List files and directories in the current directory." << std::endl;
    std::cout << "mkdir <directory>   : Create a new directory." << std::endl;
    std::cout << "rmdir <directory>   : Remove an empty directory." << std::endl;
    std::cout << "create <file>       : Create a new file." << std::endl;
    std::cout << "open <file>         : Open a file for reading or writing." << std::endl;
    std::cout << "read                : Read the content of the current open file." << std::endl;
    std::cout << "write <file>        : Write content to the current open file." << std::endl;
    std::cout << "close               : Close the current open file." << std::endl;
    std::cout << "lseek <position>    : Move the file pointer to the specified position." << std::endl;
    std::cout << "help                : Display this help message." << std::endl;
    std::cout << "time                : Display the system time." << std::endl;
    std::cout << "ver                 : Display the system version." << std::endl;
    std::cout << "rename <old> <new>  : Rename a file or directory." << std::endl;
    std::cout << "import <source> <destination> : Import a file from the local disk to the virtual disk." << std::endl;
    std::cout << "export <destination> <source> : Export a file from the virtual disk to the local disk." << std::endl;
    std::cout << "exit                : Exit the program." << std::endl;
    std::cout << "========================================================" << std::endl;
}

void FileSystemInterface::handleTime() {
    // 获取当前时间
    auto now = std::chrono::system_clock::now();

    // 将时间转换为字符串格式
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::string timeString = std::ctime(&currentTime);

    // 去除换行符
    if (!timeString.empty() && timeString[timeString.length() - 1] == '\n') {
        timeString.erase(timeString.length() - 1);
    }

    // 输出时间
    std::cout << "Current time: " << timeString << std::endl;
}

void FileSystemInterface::handleVer() {
    // 获取系统版本信息
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    // 输出系统版本
    std::cout << "Operating System Version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
}

void FileSystemInterface::handleRename(const std::vector<std::string> &args) const {
    if (args.size() != 3) {
        std::cout << "Invalid arguments. Usage: rename <old_name> <new_name>\n";
        return;
    }

    const std::string& oldName = args[1];
    const std::string& newName = args[2];

    fileSystem->rename(oldName, newName);
}

void FileSystemInterface::handleImport(const std::vector<std::string> &args) const {
    if (args.size() < 2) {
        std::cout << "Usage: import <destination> <source>" << std::endl;
        return;
    }

    const std::string& destinationPath = args[1];
    const std::string& sourceName = args[2];

    fileSystem->importFile(destinationPath, sourceName);
}

void FileSystemInterface::handleExport(const std::vector<std::string> &args) const {
    // 检查参数数量
    if (args.size() < 2) {
        std::cout << "Invalid command syntax. Usage: export <sourceName> <destinationPath>" << std::endl;
        return;
    }

    // 获取源文件名和目标文件路径
    const std::string& sourceName = args[1];
    const std::string& destinationPath = args[2];

    fileSystem->exportFile(sourceName, destinationPath);
}

void FileSystemInterface::loadFileSystem(const std::string &filePath) const {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cout << "Failed to open file: " << filePath << std::endl;
    }

    // 读取根目录
    fileSystem->root = loadDirectory(file);

    file.close();
}

Directory FileSystemInterface::loadDirectory(std::ifstream &inputFile) const{
    Directory directory;

    // 读取目录名称
    directory.name = loadString(inputFile);

    // 读取子目录数量
    int numSubdirectories;
    inputFile.read(reinterpret_cast<char*>(&numSubdirectories), sizeof(int));

    // 读取子目录
    for (int i = 0; i < numSubdirectories; ++i) {
        directory.subdirectories.push_back(loadDirectory(inputFile));
    }

    // 读取文件数量
    int numFiles;
    inputFile.read(reinterpret_cast<char*>(&numFiles), sizeof(int));

    // 读取文件
    for (int i = 0; i < numFiles; ++i) {
        directory.files.push_back(loadFile(inputFile));
    }

    return directory;
}

File FileSystemInterface::loadFile(std::ifstream &inputFile) {
    File file;

    // 读取文件名称
    file.name = loadString(inputFile);

    // 读取文件内容
    file.content = loadString(inputFile);

    return file;
}

std::string FileSystemInterface::loadString(std::ifstream &inputFile) {
    // 读取字符串长度
    int length;
    inputFile.read(reinterpret_cast<char*>(&length), sizeof(int));

    // 读取字符串内容
    std::string str;
    str.resize(length);
    inputFile.read(&str[0], length);

    return str;
}
