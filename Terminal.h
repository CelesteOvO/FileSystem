//
// Created by 86151 on 2023/6/1.
//

#ifndef FILESYSTEM_TERMINAL_H
#define FILESYSTEM_TERMINAL_H

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <deque>
#include "FileSystem.h"
#include "FileSystemInterface.h"

class Terminal
{
public:
    FileSystem* fileSystem;
    Directory* ThreadCurrentDirectory; // 当前目录
    File* ThreadCurrentFile; // 当前文件
    int ThreadCurrentFilePointer; // 当前文件指针
public:
    Terminal(FileSystem* fs) : commandHistoryIndex(0), isRunning(true), fileSystem(fs){
        memset(command, 0, sizeof(command)); // 初始化输入缓冲区
        ThreadCurrentDirectory = &fileSystem->root;
        ThreadCurrentFile = nullptr;
        ThreadCurrentFilePointer = 0;
    }
    void Run();

    void Stop();

    void TerminalThread();

    void AddOutput(const std::string& output);

    void ExecuteCommand();

    std::string GetNextCommand();

    void AddCommand(const std::string& command);
public:
    // 执行具体的命令
    void handleCd(const std::vector<std::string>& args) ;
    void handleDir(const std::vector<std::string>& args) ;
    void handleMkdir(const std::vector<std::string>& args) ;
    void handleRmdir(const std::vector<std::string>& args);
    void handleCreate(const std::vector<std::string>& args);
    void handleOpen(const std::vector<std::string>& args);
    void handleRead(const std::vector<std::string>& args);
    void handleWrite(const std::vector<std::string>& args);
    void handleClose(const std::vector<std::string>& args);
    void handleLseek(const std::vector<std::string>& args);
    void handleHelp();
    void handleTime();
    void handleVer();
    void handleRename(const std::vector<std::string>& args);
    void handleImport(const std::vector<std::string>& args);
    void handleExport(const std::vector<std::string>& args);
public:
    static std::vector<std::string> parseCommandArgs(const std::string &command);

    // 从本地磁盘读取文件系统
    void loadFileSystem(const std::string& filePath);

    // 从本地磁盘读取文件夹
    Directory loadDirectory(std::ifstream &inputFile) const;

    // 从本地磁盘读取文件
    static File loadFile(std::ifstream &inputFile);

    // 从本地磁盘读取字符
    static std::string loadString(std::ifstream &inputFile);
private:
    std::thread thread;
    std::deque<std::string> commandHistory;
    std::mutex commandMutex;
    std::condition_variable commandReady;
    size_t commandHistoryIndex;
    std::atomic<bool> isRunning;
public:
    char command[256];
    std::deque<std::string> outputBuffer;
};

#endif //FILESYSTEM_TERMINAL_H
