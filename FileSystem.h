//
// Created by 86151 on 2023/5/29.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <windows.h>

// 文件结构
struct File {
    std::string name;
    std::string content;
};

// 目录结构
struct Directory {
    std::string name;
    std::vector<Directory> subdirectories;
    std::vector<File> files;
    Directory* parent{}; // 指向父目录的指针
};

// 文件系统类
class FileSystem {
private:
    Directory root;
    Directory* currentDirectory; // 当前目录
    File* currentFile;
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
    void listDirectory();

    // 创建目录
    void createDirectory(const std::string& directoryName);

    // 删除目录
    void removeDirectory(const std::string& directoryName);

    // 创建文件
    void createFile(const std::string &fileName, std::string string);

    // 打开文件
    void openFile(const std::string& fileName);

    // 读取文件内容
    void readFile();

    // 重写文件内容
    void ReWriteFile(const std::string& content);

    // 指定位置追加文件内容
    void appendFileAtPosition(const std::string& content, int position);

    // 关闭文件
    void closeFile();

    // 定位文件指针
    void seekFile(int position);

    // 重命名文件
    void renameFile(const std::string& oldName, const std::string& newName);

    // 重命名目录
    void renameDirectory(const std::string& oldName, const std::string& newName);

    // 导入文件到本地磁盘
    void importFile(const std::string& sourcePath, const std::string& destinationName);

    // 导出文件到本地磁盘
    void exportFile(const std::string& sourceName, const std::string& destinationPath);

    // 显示系统时间
    static void displayTime();

    // 显示系统版本
    static void displayVersion();

    // 显示帮助信息
    static void displayHelp();

private:
    // 辅助函数：拆分路径为目录名称
    static std::vector<std::string> splitPath(const std::string& path);

    // 辅助函数：根据路径获取目录指针
    Directory* getDirectoryByPath(const std::string& path);

    // 辅助函数：根据文件名获取文件指针
    File* getFileByName(const std::string& fileName);

    // 辅助函数：清空当前目录和文件的指针
    void clearCurrentPointers();
};

#endif //FILESYSTEM_FILESYSTEM_H
