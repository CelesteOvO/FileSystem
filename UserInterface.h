//
// Created by 86151 on 2023/5/29.
//

#ifndef FILESYSTEM_USERINTERFACE_H
#define FILESYSTEM_USERINTERFACE_H

#include <iostream>
#include <sstream>
#include "FileSystem.h"

struct User {
    std::string username;
    std::string password;
    FileSystem fileSystem;
};

// 文件系统接口类
class FileSystemInterface {
public:
    FileSystem* fileSystem;
    User* currentUser;
public:
    FileSystemInterface(FileSystem* fs, User* user) : fileSystem(fs), currentUser(user) {}
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
};

#endif //FILESYSTEM_USERINTERFACE_H
