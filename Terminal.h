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

class Terminal
{
public:
    Terminal() : commandHistoryIndex(0), isRunning(true) {
        memset(command, 0, sizeof(command)); // 初始化输入缓冲区
    }
    void Run();

    void Stop();

    void TerminalThread();

    void AddOutput(const std::string& output);

    void ExecuteCommand();

    std::string GetNextCommand();

    void AddCommand(const std::string& command);
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
