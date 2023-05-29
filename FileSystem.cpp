//
// Created by 86151 on 2023/5/29.
//

#include "FileSystem.h"

void FileSystem::changeDirectory(const std::string &path) {
    Directory* directory = getDirectoryByPath(path);
    if (directory) {
        currentDirectory = directory;
    } else {
        std::cout << "Invalid path." << std::endl;
    }
}

void FileSystem::listDirectory() {
    std::cout << "Directory: " << currentDirectory->name << std::endl;
    std::cout << "Subdirectories:" << std::endl;
    for (const auto& directory : currentDirectory->subdirectories) {
        std::cout << "  " << directory.name << std::endl;
    }
    std::cout << "Files:" << std::endl;
    for (const auto& file : currentDirectory->files) {
        std::cout << "  " << file.name << std::endl;
    }
}

void FileSystem::createDirectory(const std::string &directoryName) {
    Directory newDirectory;
    newDirectory.name = directoryName;
    newDirectory.parent = currentDirectory;
    currentDirectory->subdirectories.push_back(newDirectory);
}

void FileSystem::removeDirectory(const std::string &directoryName) {
    for (auto it = currentDirectory->subdirectories.begin(); it != currentDirectory->subdirectories.end(); ++it) {
        if (it->name == directoryName) {
            currentDirectory->subdirectories.erase(it);
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void FileSystem::createFile(const std::string &fileName, std::string content) {
    File newFile;
    newFile.name = fileName;
    newFile.content = content;
    currentDirectory->files.push_back(newFile);
}

void FileSystem::openFile(const std::string &fileName) {
    for (auto& file : currentDirectory->files) {
        if (file.name == fileName) {
            currentFile = &file;
            return;
        }
    }
    std::cout << "File not found." << std::endl;
}

void FileSystem::readFile() {
    if (currentFile) {
        std::cout << currentFile->content << std::endl;
    } else {
        std::cout << "No file is opened." << std::endl;
    }
}

void FileSystem::ReWriteFile(const std::string &content) {
    if (currentFile) {
        currentFile->content = content;
    } else {
        std::cout << "No file is opened." << std::endl;
    }
}

void FileSystem::appendFileAtPosition(const std::string &content, int position) {
    if (currentFile) {
        if (position >= 0 && position <= currentFile->content.length()) {
            currentFile->content.insert(position, content);
            std::cout << "Content appended at position " << position << " successfully." << std::endl;
        } else {
            std::cout << "Invalid position." << std::endl;
        }
    } else {
        std::cout << "No file is currently open." << std::endl;
    }
}

void FileSystem::closeFile() {
    currentFile = nullptr;
    currentFilePointer = 0;
    std::cout << "File closed." << std::endl;
}

void FileSystem::seekFile(int position) {
    if (currentFile) {
        if (position >= 0 && position <= currentFile->content.length()) {
            currentFilePointer = position;
            std::cout << "File pointer is moved to position " << position << "." << std::endl;
        } else {
            std::cout << "Invalid file position." << std::endl;
        }
    } else {
        std::cout << "No file is currently open." << std::endl;
    }
}

void FileSystem::renameDirectory(const std::string &oldName, const std::string &newName) {
    for (auto& directory : currentDirectory->subdirectories) {
        if (directory.name == oldName) {
            directory.name = newName;
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void FileSystem::renameFile(const std::string &oldName, const std::string &newName) {

    for (auto& file : currentDirectory->files) {
        if (file.name == oldName) {
            file.name = newName;
            return;
        }
    }
    std::cout << "File not found." << std::endl;
}

void FileSystem::importFile(const std::string &sourcePath, const std::string &destinationName) {
    // 读取源文件内容
    std::ifstream sourceFile(sourcePath);
    if (!sourceFile) {
        std::cout << "Failed to open source file: " << sourcePath << std::endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(sourceFile)),
                        (std::istreambuf_iterator<char>()));

    // 创建目标文件并写入内容
    createFile(destinationName, content);

    std::cout << "File imported successfully." << std::endl;
}

void FileSystem::exportFile(const std::string &sourceName, const std::string &destinationPath) {
    // 查找源文件
    File* file = getFileByName(sourceName);
    if (!file) {
        std::cout << "File not found: " << sourceName << std::endl;
        return;
    }

    // 打开目标文件
    std::ofstream destinationFile(destinationPath);
    if (!destinationFile) {
        std::cout << "Failed to open destination file: " << destinationPath << std::endl;
        return;
    }

    // 写入文件内容到目标文件
    destinationFile << file->content;

    std::cout << "File exported successfully." << std::endl;
}

void FileSystem::displayTime() {
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

void FileSystem::displayVersion() {
// 获取系统版本信息
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    // 输出系统版本
    std::cout << "Operating System Version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
}

void FileSystem::displayHelp() {
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

std::vector<std::string> FileSystem::splitPath(const std::string& path) {
    std::vector<std::string> directories;
    std::string::size_type prev = 0;
    std::string::size_type pos = path.find('/', prev);

    while (pos != std::string::npos) {
        std::string directoryName = path.substr(prev, pos - prev);
        directories.push_back(directoryName);
        prev = pos + 1;
        pos = path.find('/', prev);
    }

    // 添加最后一个目录名称
    std::string directoryName = path.substr(prev);
    directories.push_back(directoryName);

    return directories;
}


Directory *FileSystem::getDirectoryByPath(const std::string &path) {
    Directory* current = &root;
    std::vector<std::string> directories = splitPath(path);

    for (const std::string& directoryName : directories) {
        bool found = false;
        for (Directory& directory : current->subdirectories) {
            if (directory.name == directoryName) {
                current = &directory;
                found = true;
                break;
            }
        }

        if (!found) {
            return nullptr;  // 目录不存在
        }
    }

    return current;
}

File *FileSystem::getFileByName(const std::string &fileName) {
    Directory* current = currentDirectory;

    for (File& file : current->files) {
        if (file.name == fileName) {
            return &file;
        }
    }
    return nullptr;  // 文件不存在
}

void FileSystem::clearCurrentPointers() {
    currentDirectory = nullptr;
    currentFile = nullptr;
}














