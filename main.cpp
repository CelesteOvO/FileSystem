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

#include <iostream>
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
}







