//
// Created by 86151 on 2023/6/1.
//
#include "Terminal.h"

void Terminal::Run() {
    thread = std::thread([this]() { TerminalThread(); });
}

void Terminal::Stop() {
    isRunning = false;
    if (thread.joinable())
        thread.join();
}

void Terminal::TerminalThread() {
    while (isRunning)
    {
        std::string commands = GetNextCommand();
        std::string currentDir = FileSystem::getDirectoryPath(fileSystem->currentDirectory);
        AddOutput(currentDir + " > " + commands);
        ExecuteCommand();
    }
}

void Terminal::AddOutput(const std::string &output) {
    outputBuffer.push_back(output);
}

void Terminal::ExecuteCommand() {
// 模拟执行命令的函数
    std::string commandStr(command);
    std::vector<std::string> args = parseCommandArgs(commandStr);
    std::string cmd = args[0];
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
        //handleWrite(args);
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
        // handleRename(args);
    } else if (cmd == "import") {
        handleImport(args);
    } else if (cmd == "export") {
        handleExport(args);
    } else if (cmd == "exit") {
        AddOutput("Exit successfully.");
        fileSystem->clearCurrentPointers();
        exit(0);
    } else if (cmd == "save") {
        AddOutput("Saving file system...");
        fileSystem->saveFileSystem(*fileSystem, "liyifan"); /// TODO
    } else if ( cmd == "load"){
        AddOutput("Loading file system...");
        loadFileSystem("liyifan");
    }
    else {
        AddOutput("Unknown command. Type 'help' for command list.");
    }

    memset(command, 0, sizeof(command)); // 清空输入缓冲区
    std::this_thread::sleep_for(std::chrono::milliseconds (1));  // 模拟命令执行的耗时
}

std::string Terminal::GetNextCommand() {
    std::unique_lock<std::mutex> lock(commandMutex);
    if (commandHistoryIndex >= commandHistory.size())
    {
        commandReady.wait(lock, [this]() { return commandHistoryIndex < commandHistory.size(); });
    }
    std::string commands = commandHistory[commandHistoryIndex++];
    return commands;
}

void Terminal::AddCommand(const std::string &commands) {
    std::lock_guard<std::mutex> lock(commandMutex);
    std::strncpy(this->command, commands.c_str(), sizeof(this->command));
    commandHistory.push_back(commands);
    commandReady.notify_one();
}

void Terminal::AddOperation(const std::string &operations) {
    std::strncpy(this->operation, operations.c_str(), sizeof(this->operation));
    operationsHistory.emplace_back(" ");
    operationsHistory.push_back(operations);
}

std::vector<std::string> Terminal::parseCommandArgs(const std::string &commands) {
    std::vector<std::string> args;
    std::istringstream iss(commands);
    std::string arg;

    // 使用空格作为分隔符逐个获取参数
    while (iss >> arg) {
        args.push_back(arg);
    }

    return args;
}

void Terminal::handleCd(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: cd <directory>");
        return;
    }

    const std::string& directoryPath = args[1];

    // 调用文件系统类的切换目录方法
    // fileSystem->changeDirectory(directoryPath);
    /// 为了输出我只能委屈一下代码了

    Directory* directory = fileSystem->getDirectoryByPath(directoryPath); // 根据路径获取目录
    if (directory) {
        fileSystem->currentDirectory = directory; // 切换当前目录
    } else {
        AddOutput("Invalid path.");
    }
}

void Terminal::handleDir(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 1) {
        AddOutput("Invalid command. Usage: dir");
        return;
    }

    // 调用文件系统类的列出目录方法
    // fileSystem->listDirectory();
    AddOutput("Directory: " + fileSystem->currentDirectory->name);
    AddOutput("Subdirectories:");
    for (const auto& directory : fileSystem->currentDirectory->subdirectories) {
        AddOutput( "  " + directory.name);
    }
    AddOutput("Files:");
    for (const auto& file : fileSystem->currentDirectory->files) {
        AddOutput("  " + file.name);
    }
}

void Terminal::handleMkdir(const std::vector<std::string> &args) {
// 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: mkdir <directory>");
        return;
    }

    const std::string& directoryName = args[1];

    // 调用文件系统类的创建目录方法
    // fileSystem->createDirectory(directoryName);

    // 检查当前目录下是否存在同名的文件夹
    bool duplicateFound = false;
    int duplicateCount = 1;
    std::string newName = directoryName;

    for (const auto& subdirectory : fileSystem->currentDirectory->subdirectories) {
        if (subdirectory.name == newName) {
            duplicateFound = true;
            break;
        }
    }

    // 如果存在同名的文件夹，则在新目录名后面添加适当的编号
    if(duplicateFound) {
        newName = directoryName + "_" + std::to_string(duplicateCount);
        AddOutput( "Directory already exists. Renaming to " + newName);
        duplicateCount++;

        duplicateFound = false;
        for (const auto& subdirectory : fileSystem->currentDirectory->subdirectories) {
            if (subdirectory.name == newName) {
                duplicateFound = true;
                break;
            }
        }
    }
    Directory newDirectory; // 创建新目录
    newDirectory.name = newName; // 设置目录名
    newDirectory.parent = fileSystem->currentDirectory; // 设置父目录
    fileSystem->currentDirectory->subdirectories.push_back(newDirectory); // 将新目录添加到当前目录的子目录中
}

void Terminal::handleRmdir(const std::vector<std::string> &args) {
// 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: rmdir <directory_name>");
        return;
    }

    // 获取目录名称
    const std::string& directoryName = args[1];

    // 调用文件系统类的删除目录方法
    // fileSystem->removeDirectory(directoryName);
    for (auto it = fileSystem->currentDirectory->subdirectories.begin(); it != fileSystem->currentDirectory->subdirectories.end(); ++it) {
        // 遍历当前目录的子目录
        if (it->name == directoryName) { // 找到目标目录
            fileSystem->currentDirectory->subdirectories.erase(it); // 删除目标目录
            return;
        }
    }
    AddOutput("Directory not found.");
}

void Terminal::handleCreate(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: create <file_name>");
        return;
    }

    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的创建文件方法
    fileSystem->createFile(fileName, "");
}

void Terminal::handleOpen(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: open <file_name>");
        return;
    }

    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的打开文件方法
    // fileSystem->openFile(fileName);
    for (auto& file : fileSystem->currentDirectory->files) { // 遍历当前目录的子文件
        if (file.name == fileName) { // 找到目标文件
            fileSystem->currentFile = &file; // 打开目标文件
            return;
        }
    }
    AddOutput("File not found.");
}

void Terminal::handleRead(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 1) {
        AddOutput("Invalid command. Usage: read");
        return;
    }

    // 调用文件系统类的读取文件方法
    // fileSystem->readFile();
    if (fileSystem->currentFile) { // 检查是否有打开的文件
        AddOutput(fileSystem->currentFile->content); // 输出文件内容
    } else {
        AddOutput("No file is currently open.");
    }
}

void Terminal::handleWrite(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: write <file_name>");
        return;
    }
    // 获取文件名
    const std::string& fileName = args[1];

    // 调用文件系统类的打开文件方法
    fileSystem->openFile(fileName);

    // 检查文件是否已打开
    if (fileSystem->currentFile == nullptr) {
        AddOutput("No file is currently open.");
        return;
    }

    if(fileSystem->currentFile->content.empty()){
        // 获取用户输入的内容
        std::string content;
        AddOutput("Enter the content to write: ");
        //std::getline(std::cin, content);

        // 调用文件系统类的重写文件内容方法
        fileSystem->ReWriteFile(content);
        fileSystem->closeFile();
    }else{
        AddOutput("The content of the file is: ");
        AddOutput("Do you want to append to the file? (y/n) ");
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

void Terminal::handleClose(const std::vector<std::string> &args) {
// 检查参数数量是否正确
    if (args.size() != 1) {
        AddOutput("Invalid command. Usage: close");
        return;
    }

    // 检查是否有文件被打开
    if (fileSystem->currentFile == nullptr) {
        AddOutput("No file is currently open.");
        return;
    }

    // 关闭文件
    fileSystem->closeFile();

    AddOutput("File has been closed.");
}

void Terminal::handleLseek(const std::vector<std::string> &args) {
    // 检查参数数量是否正确
    if (args.size() != 2) {
        AddOutput("Invalid command. Usage: lseek <position>");
        return;
    }

    // 检查是否有文件被打开
    if (fileSystem->currentFile == nullptr) {
        AddOutput("No file is currently open.");
        return;
    }

    // 解析位置参数
    int position;
    try {
        position = std::stoi(args[1]);
    } catch (std::exception& e) {
        AddOutput("Invalid position value. Please enter a valid integer.");
        return;
    }

    // 定位文件指针
    fileSystem->seekFile(position);
}

void Terminal::handleHelp() {
    AddOutput( "========================= Help =========================" );
    AddOutput("Available commands:" );
    AddOutput("cd <directory>      : Change current directory." );
    AddOutput( "dir                 : List files and directories in the current directory." );
    AddOutput( "mkdir <directory>   : Create a new directory." );
    AddOutput("rmdir <directory>   : Remove an empty directory." );
    AddOutput("create <file>       : Create a new file." );
    AddOutput("open <file>         : Open a file for reading or writing." );
    AddOutput("read                : Read the content of the current open file." );
    AddOutput("write <file>        : Write content to the current open file." );
    AddOutput("close               : Close the current open file." );
    AddOutput("lseek <position>    : Move the file pointer to the specified position." );
    AddOutput("help                : Display this help message." );
    AddOutput("time                : Display the system time." );
    AddOutput("ver                 : Display the system version." );
    AddOutput("rename <old> <new>  : Rename a file or directory." );
    AddOutput("import <source> <destination> : Import a file from the local disk to the virtual disk." );
    AddOutput("export <destination> <source> : Export a file from the virtual disk to the local disk." );
    AddOutput("exit                : Exit the program." );
    AddOutput("========================================================" );
}

void Terminal::handleTime() {
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
    AddOutput( "Current time: " + timeString);
}

void Terminal::handleVer() {
// 获取系统版本信息
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    std::ostringstream oss;
    oss << "Operating System Version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
    std::string output = oss.str();
    AddOutput(output);
}

void Terminal::handleRename(const std::vector<std::string> &args) {
    if (args.size() != 3) {
        std::cout << "Invalid arguments. Usage: rename <old_name> <new_name>\n";
        return;
    }

    const std::string& oldName = args[1];
    const std::string& newName = args[2];

    fileSystem->rename(oldName, newName);
}

void Terminal::handleImport(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        AddOutput("Usage: import <destination> <source>");
        return;
    }

    const std::string& destinationPath = args[1];
    const std::string& sourceName = args[2];

    fileSystem->importFile(destinationPath, sourceName);
}

void Terminal::handleExport(const std::vector<std::string> &args) {
    // 检查参数数量
    if (args.size() < 2) {
        AddOutput("Invalid command syntax. Usage: export <sourceName> <destinationPath>");
        return;
    }

    // 获取源文件名和目标文件路径
    const std::string& sourceName = args[1];
    const std::string& destinationPath = args[2];

    fileSystem->exportFile(sourceName, destinationPath);
}

void Terminal::loadFileSystem(const std::string &filePath) const {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cout << "Failed to open file: " << filePath << std::endl;
    }

    // 读取根目录
    fileSystem->root = loadDirectory(file);

    file.close();
}

Directory Terminal::loadDirectory(std::ifstream &inputFile) const {
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

File Terminal::loadFile(std::ifstream &inputFile) {
    File file;

    // 读取文件名称
    file.name = loadString(inputFile);

    // 读取文件内容
    file.content = loadString(inputFile);

    return file;
}

std::string Terminal::loadString(std::ifstream &inputFile) {
    // 读取字符串长度
    int length;
    inputFile.read(reinterpret_cast<char*>(&length), sizeof(int));

    // 读取字符串内容
    std::string str;
    str.resize(length);
    inputFile.read(&str[0], length);

    return str;
}











