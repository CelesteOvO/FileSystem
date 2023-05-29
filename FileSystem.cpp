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
    Directory newDirectory;
    newDirectory.name = directoryName;
    newDirectory.parent = currentDirectory;
    currentDirectory->subdirectories.push_back(newDirectory);
}

void FileSystem::removeDirectory(const std::string &directoryName) const {
    for (auto it = currentDirectory->subdirectories.begin(); it != currentDirectory->subdirectories.end(); ++it) {
        if (it->name == directoryName) {
            currentDirectory->subdirectories.erase(it);
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void FileSystem::createFile(const std::string &fileName, std::string content) const {
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
    std::vector<File*> files = getFileByName(sourceName);
    if (files.empty()) {
        std::cout << "File not found: " << sourceName << std::endl;
        return;
    }

    // 显示同名文件列表供用户选择
    std::cout << "Multiple files found with the name '" << sourceName << "'. Please select the file to export:" << std::endl;
    for (int i = 0; i < files.size(); ++i) {
        std::cout << i+1 << ". " << files[i]->name << std::endl;
    }

    // 获取用户选择的文件索引
    int selection;
    std::cin >> selection;

    // 验证用户选择的文件索引是否有效
    if (selection < 1 || selection > files.size()) {
        std::cout << "Invalid selection." << std::endl;
        return;
    }

    // 获取用户选择的文件
    File* selectedFile = files[selection - 1];

    // 打开目标文件
    std::ofstream destinationFile(destinationPath);
    if (!destinationFile) {
        std::cout << "Failed to open destination file: " << destinationPath << std::endl;
        return;
    }

    // 写入文件内容到目标文件
    destinationFile << selectedFile->content;

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

std::vector<File*> FileSystem::getFileByName(const std::string &fileName) const {
    std::vector<File*> matchingFiles;

    Directory* current = currentDirectory;

    for (File& file : current->files) {
        if (file.name == fileName) {
            matchingFiles.push_back(&file);
        }
    }

    return matchingFiles;
}

std::vector<Directory *> FileSystem::getDirectoryByName(const std::string &directoryName) const {
    std::vector<Directory*> matchingDirectories;

    Directory* current = currentDirectory;

    for (Directory& directory : current->subdirectories) {
        if (directory.name == directoryName) {
            matchingDirectories.push_back(&directory);
        }
    }

    return matchingDirectories;
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















