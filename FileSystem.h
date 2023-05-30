//
// Created by 86151 on 2023/5/29.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <chrono>
#include <stack>
#include <windows.h>

struct Directory;

// 文件结构
struct File {
    std::string name; // 文件名
    std::string content; // 文件内容
    Directory* parentDirectory; // 指向父目录的指针
};

// 目录结构
struct Directory {
    std::string name; // 文件夹名
    std::vector<Directory> subdirectories; // 子文件夹
    std::vector<File> files; // 子文件
    Directory* parent{}; // 指向父目录的指针
};

// 文件系统类
class FileSystem {
public:
    Directory root; // 根目录
    Directory* currentDirectory; // 当前目录
    File* currentFile; // 当前文件
    int currentFilePointer; // 当前文件指针

public:
    FileSystem() {
        // 初始化根目录
        root.name = "root";
        root.parent = nullptr;
        currentDirectory = &root;
        currentFile = nullptr;
        currentFilePointer = 0;
    }

public:
    // 切换当前目录
    void changeDirectory(const std::string& path);

    // 列出当前目录内容
    void listDirectory() const;

    // 创建目录
    void createDirectory(const std::string& directoryName) const;

    // 删除目录
    void removeDirectory(const std::string& directoryName) const;

    // 创建文件
    void createFile(const std::string &fileName, std::string string) const;

    // 打开文件
    void openFile(const std::string& fileName);

    // 读取文件内容
    void readFile() const;

    // 重写文件内容
    void ReWriteFile(const std::string& content) const;

    // 指定位置追加文件内容
    void appendFileAtPosition(const std::string& content, int position) const;

    // 关闭文件
    void closeFile();

    // 定位文件指针
    void seekFile(int position);

    // 重命名
    void rename(const std::string& oldName, const std::string& newName) const;

    // 导入文件到本地磁盘
    void importFile(const std::string& sourcePath, const std::string& destinationName) const;

    // 导出文件到本地磁盘
    void exportFile(const std::string& sourceName, const std::string& destinationPath) const;

public:
    // 辅助函数：根据文件名获取文件指针
    File* getFileByName(const std::string& fileName) const;

    // 辅助函数：根据文件夹名获取文件夹指针
    Directory* getDirectoryByName(const std::string& directoryName) const;

    // 辅助函数：获取当前目录路径
    static std::string getDirectoryPath(Directory* directory) ;

    // 辅助函数：拆分路径为目录名称
    static std::vector<std::string> splitPath(const std::string& path);

    // 辅助函数：根据路径获取目录指针
    Directory* getDirectoryByPath(const std::string& path);

    // 辅助函数：清空当前目录和文件的指针
    void clearCurrentPointers();

public:
    // 保存文件系统到本地磁盘
    void saveFileSystem(const FileSystem& fileSystem, const std::string& fileName);

    // 保存文件夹
    void saveDirectory(const Directory& directory, std::ofstream& outputFile);

    // 保存文件
    static void saveFile(const File& file, std::ofstream& outputFile);

    // 保存字符
    static void saveString(const std::string& str, std::ofstream& outputFile);

};

#endif //FILESYSTEM_FILESYSTEM_H
