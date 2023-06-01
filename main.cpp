/*#include <iostream>
#include <mutex>
#include "FileSystemInterface.h"


int main()
{
    FileSystem fileSystem;
    FileSystemInterface fileSystemInterface(&fileSystem);
    fileSystemInterface.start();

    return 0;
}*/

/*#include <iostream>
#include <thread>
#include "FileSystemInterface.h"

class FileSystemThread : public std::thread {
public:
    FileSystemThread(FileSystemInterface* fsInterface) : std::thread([fsInterface]() {
        fsInterface->start();
    }) {}
};

int main() {
    FileSystem fileSystem;

    FileSystemInterface fileSystemInterface1(&fileSystem);
    FileSystemThread thread1(&fileSystemInterface1);

    FileSystemInterface fileSystemInterface2(&fileSystem);
    FileSystemThread thread2(&fileSystemInterface2);

    // 等待线程结束
    thread1.join();
    thread2.join();

    return 0;
}*/

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Terminal.h"
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <string>


std::vector<Terminal*> terminals;
std::mutex terminalsMutex;

void TerminalWindow(Terminal& terminal)
{
    // 显示输出缓冲区的内容
    for (const auto& output : terminal.outputBuffer)
    {
        ImGui::Text("%s", output.c_str());
    }

    // 模拟终端的输入行
    ImGui::Separator();
    static char commands[256] = "";
    if (ImGui::InputText("Command", commands, sizeof(terminal.command), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        terminal.AddCommand(commands);
        memset(commands, 0, sizeof(commands));
    }
    static char operations[256] = "";
    if (ImGui::InputText("operation", operations, sizeof(terminal.command), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        terminal.AddOperation(operations);
        memset(operations, 0, sizeof(operations));
    }
}

int main()
{
    FileSystem fileSystem;

    // 初始化GLFW
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Multiple Terminals", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // 初始化Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // 创建终端
    constexpr int numTerminals = 2;
    for (int i = 0; i < numTerminals; i++)
    {
        Terminal* terminal = new Terminal(&fileSystem); // 分配内存并初始化终端对象
        terminals.emplace_back(terminal); // 添加指针到向量中
        terminals[i]->Run();
    }

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            std::lock_guard<std::mutex> lock(terminalsMutex);
            for (auto& terminal : terminals)
            {
                ImGui::Begin(("Terminal " + std::to_string(&terminal - &terminals[0])).c_str());
                TerminalWindow(*terminal);
                ImGui::End();
            }
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 停止并等待终端线程结束
    {
        std::lock_guard<std::mutex> lock(terminalsMutex);
        for (auto& terminal : terminals)
        {
            terminal->Stop();
        }
    }

    // 清理和释放资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}







