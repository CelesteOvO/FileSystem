//
// Created by 86151 on 2023/5/29.
//

#include "UserInterface.h"

void FileSystemInterface::start() {
    displayWelcomeMessage();

    while (true) {
        displayPrompt();
        std::string command = getUserInput();
        executeCommand(command);
    }
}

void FileSystemInterface::executeCommand(const std::string &command) {
    std::vector<std::string> args = parseCommandArgs(command);
    if (args.empty()) {
        return;
    }

    std::string cmd = args[0];
    args.erase(args.begin());

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
    } else {
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
    std::string username = currentUser->username;

    // 获取当前目录信息
    std::string currentDir = fileSystem->getDirectoryPath(fileSystem->currentDirectory);

    // 显示命令提示符
    std::cout << username << "@" << currentDir << " $ ";
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

    const std::string& directoryName = args[1];

    // 调用文件系统类的切换目录方法
    fileSystem->changeDirectory(directoryName);
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

    // 检查文件是否已打开
    if (fileSystem->currentFile == nullptr) {
        std::cout << "No file is currently open." << std::endl;
        return;
    }

    // 获取用户输入的内容
    std::string content;
    std::cout << "Enter the content to write: ";
    std::getline(std::cin, content);

    // 调用文件系统类的重写文件内容方法
    fileSystem->ReWriteFile(content);

    std::cout << "Content has been written to the file." << std::endl;
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

    std::cout << "File pointer has been set to position " << position << std::endl;
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
    std::cout << "write <content>     : Write content to the current open file." << std::endl;
    std::cout << "close               : Close the current open file." << std::endl;
    std::cout << "lseek <position>    : Move the file pointer to the specified position." << std::endl;
    std::cout << "help                : Display this help message." << std::endl;
    std::cout << "time                : Display the system time." << std::endl;
    std::cout << "ver                 : Display the system version." << std::endl;
    std::cout << "rename <old> <new>  : Rename a file or directory." << std::endl;
    std::cout << "import <source> <destination> : Import a file from the local disk to the virtual disk." << std::endl;
    std::cout << "export <source> <destination> : Export a file from the virtual disk to the local disk." << std::endl;
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

    std::vector<File *> existingFile = fileSystem->getFileByName(oldName);
    std::vector<Directory *> existingDirectory = fileSystem->getDirectoryByName(oldName);

    int choice = 0;
    if (!existingFile.empty() && !existingDirectory.empty()) {
        std::cout << "File or directory with the same name already exists.\n";
        std::cout << "Please select the object to rename:\n";
        std::cout << "1. File\n";
        std::cout << "2. Directory\n";
        std::cout << "Enter your choice (1 or 2): ";

        std::cin >> choice;
    }
    if(existingFile.size() > 1 || choice == 1){
        std::cout << "Multiple files found with the name '" << oldName << "'. Please select the file to export:" << std::endl;
        for (int i = 0; i < existingFile.size(); ++i) {
            std::cout << i+1 << ". " << fileSystem->getDirectoryPath(existingFile[i]->parentDirectory)  << std::endl;
        }
        // 获取用户选择的文件索引
        int selection;
        std::cin >> selection;

        // 验证用户选择的文件索引是否有效
        if (selection < 1 || selection > existingFile.size()) {
            std::cout << "Invalid selection." << std::endl;
            return;
        }

        // 获取用户选择的文件
        File* selectedFile = existingFile[selection - 1];

        std::cout << "Renaming file '" << oldName << "' to '" << newName << "'...\n";
        selectedFile->name = newName;
        std::cout << "File renamed successfully.\n";
    }
    else if(existingDirectory.size() > 1 || choice == 2){
        std::cout << "Multiple directory found with the name '" << oldName << "'. Please select the directory to export:" << std::endl;
        for (int i = 0; i < existingDirectory.size(); ++i) {
            std::cout << i+1 << ". " << fileSystem->getDirectoryPath(existingDirectory[i])  << std::endl;
        }
        // 获取用户选择的文件索引
        int selection;
        std::cin >> selection;

        // 验证用户选择的文件索引是否有效
        if (selection < 1 || selection > existingDirectory.size()) {
            std::cout << "Invalid selection." << std::endl;
            return;
        }

        // 获取用户选择的目录
        Directory* selectedDirectory = existingDirectory[selection - 1];
        std::cout << "Renaming directory '" << oldName << "' to '" << newName << "'...\n";
        selectedDirectory->name = newName;
        std::cout << "Directory renamed successfully.\n";

    }else if(existingFile.empty() && existingDirectory.empty()){
        std::cout << "File or directory with the same name does not exist.\n";
    }else if(existingFile.size() == 1){
        std::cout << "Renaming file '" << oldName << "' to '" << newName << "'...\n";
        existingFile[0]->name = newName;
        std::cout << "File renamed successfully.\n";
    }else if(existingDirectory.size() == 1){
        std::cout << "Renaming directory '" << oldName << "' to '" << newName << "'...\n";
        existingDirectory[0]->name = newName;
        std::cout << "Directory renamed successfully.\n";
    }
}

void FileSystemInterface::handleImport(const std::vector<std::string> &args) const {
    // 检查参数数量
    if (args.size() < 2) {
        std::cout << "Invalid command syntax. Usage: import <sourcePath> <destinationName>" << std::endl;
        return;
    }

    // 获取源文件路径和目标文件名
    const std::string& sourcePath = args[0];
    const std::string& destinationName = args[1];

    // 检查源文件是否存在
    std::ifstream sourceFile(sourcePath);
    if (!sourceFile) {
        std::cout << "Failed to open source file: " << sourcePath << std::endl;
        return;
    }

    // 检查目标文件名是否已存在
    if (!fileSystem->getFileByName(destinationName).empty() || !fileSystem->getDirectoryByName(destinationName).empty()) {
        std::cout << "A file or directory with the same name already exists: " << destinationName << std::endl;
        return;
    }

    // 创建新文件并设置内容
    fileSystem->createFile(destinationName, std::string(std::istreambuf_iterator<char>(sourceFile), std::istreambuf_iterator<char>()));

    std::cout << "File imported successfully." << std::endl;
}

void FileSystemInterface::handleExport(const std::vector<std::string> &args) const {
    // 检查参数数量
    if (args.size() < 2) {
        std::cout << "Invalid command syntax. Usage: export <sourceName> <destinationPath>" << std::endl;
        return;
    }

    // 获取源文件名和目标文件路径
    const std::string& sourceName = args[0];
    const std::string& destinationPath = args[1];

    // 检查源文件是否存在
    std::vector<File *> sourceFile = fileSystem->getFileByName(sourceName);
    if (sourceFile.empty()) {
        std::cout << "File not found: " << sourceName << std::endl;
        return;
    }

    // 打开目标文件
    std::ofstream destinationFile(destinationPath);
    if (!destinationFile) {
        std::cout << "Failed to open destination file: " << destinationPath << std::endl;
        return;
    }
    File* selectedFile;
    if(sourceFile.size() > 1) {
        std::cout << "Multiple files found with the name '" << sourceName << "'. Please select the file to export:" << std::endl;
        for (int i = 0; i < sourceFile.size(); ++i) {
            std::cout << i+1 << ". " << fileSystem->getDirectoryPath(sourceFile[i]->parentDirectory)  << std::endl;
        }
        // 获取用户选择的文件索引
        int selection;
        std::cin >> selection;

        // 验证用户选择的文件索引是否有效
        if (selection < 1 || selection > sourceFile.size()) {
            std::cout << "Invalid selection." << std::endl;
            return;
        }

        // 获取用户选择的文件
        selectedFile = sourceFile[selection - 1];
    }else {
        selectedFile = sourceFile[0];
    }

    // 写入文件内容到目标文件
    destinationFile << selectedFile->content;

    std::cout << "File exported successfully." << std::endl;
}







