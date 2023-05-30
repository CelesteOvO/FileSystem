//
// Created by 86151 on 2023/5/29.
//

#include "FileSystem.h"

#include <utility>

void FileSystem::changeDirectory(const std::string &path) {
    Directory* directory = getDirectoryByPath(path); // 根据路径获取目录
    if (directory) {
        currentDirectory = directory; // 切换当前目录
    } else {
        std::cout << "Invalid path." << std::endl;
    }
}

void FileSystem::listDirectory() const {
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

void FileSystem::createDirectory(const std::string &directoryName) const {
    Directory newDirectory; // 创建新目录
    newDirectory.name = directoryName; // 设置目录名
    newDirectory.parent = currentDirectory; // 设置父目录
    currentDirectory->subdirectories.push_back(newDirectory); // 将新目录添加到当前目录的子目录中
}

void FileSystem::removeDirectory(const std::string &directoryName) const {
    for (auto it = currentDirectory->subdirectories.begin(); it != currentDirectory->subdirectories.end(); ++it) {
        // 遍历当前目录的子目录
        if (it->name == directoryName) { // 找到目标目录
            currentDirectory->subdirectories.erase(it); // 删除目标目录
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void FileSystem::createFile(const std::string &fileName, std::string content) const {
    File newFile; // 创建新文件
    newFile.name = fileName; // 设置文件名
    newFile.content = std::move(content); // 设置文件内容
    newFile.parentDirectory = currentDirectory; // 设置父目录
    currentDirectory->files.push_back(newFile); // 将新文件添加到当前目录的子文件中
}

void FileSystem::openFile(const std::string &fileName) {
    for (auto& file : currentDirectory->files) { // 遍历当前目录的子文件
        if (file.name == fileName) { // 找到目标文件
            currentFile = &file; // 打开目标文件
            return;
        }
    }
    std::cout << "File not found." << std::endl;
}

void FileSystem::readFile() const {
    if (currentFile) {
        std::cout << currentFile->content << std::endl;
    } else {
        std::cout << "No file is opened." << std::endl;
    }
}

void FileSystem::ReWriteFile(const std::string &content) const {
    if (currentFile) {
        currentFile->content = content;
    } else {
        std::cout << "No file is opened." << std::endl;
    }
}

void FileSystem::appendFileAtPosition(const std::string &content, int position) const {
    if (currentFile) {
        if (position >= 0 && position <= currentFile->content.length()) { // 检查位置是否合法
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
    // std::cout << "File closed." << std::endl;
}

void FileSystem::seekFile(int position) {
    if (currentFile) {
        if (position >= 0 && position <= currentFile->content.length()) {
            currentFilePointer = position; // 移动文件指针
            std::cout << "File pointer is moved to position " << position << "." << std::endl;
        } else {
            std::cout << "Invalid file position." << std::endl;
        }
    } else {
        std::cout << "No file is currently open." << std::endl;
    }
}

void FileSystem::renameDirectory(const std::string &oldName, const std::string &newName) const {
    for (auto& directory : currentDirectory->subdirectories) {
        if (directory.name == oldName) {
            directory.name = newName;
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void FileSystem::renameFile(const std::string &oldName, const std::string &newName) const {

    for (auto& file : currentDirectory->files) {
        if (file.name == oldName) {
            file.name = newName;
            return;
        }
    }
    std::cout << "File not found." << std::endl;
}

void FileSystem::importFile(const std::string &sourcePath, const std::string &destinationName) const {
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

void FileSystem::exportFile(const std::string &sourceName, const std::string &destinationPath) const {
    // 查找源文件
    File* file = getFileByName(sourceName);
    if (file == nullptr) {
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

File* FileSystem::getFileByName(const std::string &fileName) const {
    Directory* current = currentDirectory;

    for (File& file : current->files) {
        if (file.name == fileName) {
            return &file;
        }
    }
    return nullptr;  // 文件不存在
}

Directory * FileSystem::getDirectoryByName(const std::string &directoryName) const {
    Directory* current = currentDirectory;

    for (Directory& directory : current->subdirectories) {
        if (directory.name == directoryName) {
            return &directory;
        }
    }
    return nullptr;  // 目录不存在
}

void FileSystem::clearCurrentPointers() {
    currentDirectory = nullptr;
    currentFile = nullptr;
}

std::string FileSystem::getDirectoryPath(Directory* directory) const {
    std::string path = directory->name;
    Directory* parent = directory->parent;

    while (parent != nullptr) {
        path = parent->name + "\\" + path;
        parent = parent->parent;
    }

    return path;
}

std::string FileSystem::getFilePath(File* file) const {
    return getDirectoryPath(file->parentDirectory) + "\\" + currentFile->name;
}















