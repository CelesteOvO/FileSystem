#include <iostream>
#include "UserInterface.h"

int main()
{
    FileSystem fileSystem;
    User user1("username", "password",fileSystem);
    FileSystemInterface fileSystemInterface;
    fileSystemInterface.fileSystem = &fileSystem;
    fileSystemInterface.start();
    return 0;
}
