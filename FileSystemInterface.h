//
// Created by 86151 on 2023/5/29.
//

#ifndef FILESYSTEM_FILESYSTEMINTERFACE_H
#define FILESYSTEM_FILESYSTEMINTERFACE_H

#include <iostream>
#include <thread>
#include "FileSystem.h"

// 文件系统接口类
class FileSystemInterface {
public:
    FileSystem* fileSystem;
public:
    FileSystemInterface(FileSystem* fs) : fileSystem(fs) {}
public:
    // 启动文件系统
    void start();

    // 解析并执行命令
    void executeCommand(const std::string& command) const;

    // 解析命令参数
    static std::vector<std::string> parseCommandArgs(const std::string& command);

    // 显示命令提示符
    void displayPrompt() const;

    // 显示欢迎信息
    static void displayWelcomeMessage();

    // 获取用户输入
    static std::string getUserInput();

    // 执行具体的命令
    void handleCd(const std::vector<std::string>& args) const;
    void handleDir(const std::vector<std::string>& args) const;
    void handleMkdir(const std::vector<std::string>& args) const;
    void handleRmdir(const std::vector<std::string>& args) const;
    void handleCreate(const std::vector<std::string>& args) const;
    void handleOpen(const std::vector<std::string>& args) const;
    void handleRead(const std::vector<std::string>& args) const;
    void handleWrite(const std::vector<std::string>& args) const;
    void handleClose(const std::vector<std::string>& args) const;
    void handleLseek(const std::vector<std::string>& args) const;
    static void handleHelp();
    static void handleTime();
    static void handleVer();
    void handleRename(const std::vector<std::string>& args) const;
    void handleImport(const std::vector<std::string>& args) const;
    void handleExport(const std::vector<std::string>& args) const;
public:
    // 从本地磁盘读取文件系统
    void loadFileSystem(const std::string& filePath) const;

    // 从本地磁盘读取文件夹
    Directory loadDirectory(std::ifstream &inputFile) const;

    // 从本地磁盘读取文件
    static File loadFile(std::ifstream &inputFile);

    // 从本地磁盘读取字符
    static std::string loadString(std::ifstream &inputFile);
};

#endif //FILESYSTEM_FILESYSTEMINTERFACE_H
