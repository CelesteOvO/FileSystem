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
        std::string command = GetNextCommand();
        AddOutput("> " + command);
        ExecuteCommand();
        AddOutput("Command executed.");
    }
}

void Terminal::AddOutput(const std::string &output) {
    outputBuffer.push_back(output);
}

void Terminal::ExecuteCommand() {
// 模拟执行命令的函数
    std::string commandStr(command);
    if (!commandStr.empty())
    {
        AddOutput("> " + commandStr);

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
    std::string command = commandHistory[commandHistoryIndex++];
    return command;
}

void Terminal::AddCommand(const std::string &command) {
    std::lock_guard<std::mutex> lock(commandMutex);
    commandHistory.push_back(command);
    commandReady.notify_one();
}





