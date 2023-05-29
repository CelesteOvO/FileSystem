#include <iostream>
#include "UserInterface.h"

int main()
{
    FileSystem fileSystem1;
    User user1("liyifan", "password", fileSystem1);
    FileSystemInterface fileSystemInterface(&fileSystem1, &user1);
    fileSystemInterface.start();
    return 0;
}
