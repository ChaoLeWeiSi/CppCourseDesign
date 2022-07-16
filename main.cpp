#include <iostream>
#include <stdio.h>
#include "Platform.hpp"
#include "PlatformShow.hpp"
int main()
{
    std::cout << "建立服务端(输入0)或者客户端(输入1)\n";
    int choice;
    std::cin >> choice;
    while (std::cin.fail() || std::cin.get() != '\n')
    {
        std::cin.clear();
        std::cin.ignore(LLONG_MAX, '\n');
        std::cout << "输入不合法，请输入数字\n";
        std::cin >> choice;
        continue;
    }
    if (choice == 0)
    {
        Platform sys;
        sys.show();
        sys.freeUser();
        sys.freeExpress();
    }
    else if (choice == 1)
    {
        // getchar();
        PlatformShow sysc;
        sysc.show();
    }
    else
        std::cout << "选择错误，已退出\n";
    // if (choice == 0)
    // {
    //     if (!server.serverInit())
    //         return -1;
    // }
    // else if (choice == 1)
    // {
    //     if (!client.clientInit())
    //         return -1;
    // }
    // Platform sys(client, server);
    // sys.show();
    // sys.freeUser();
    // system("pause");
    return 0;
}
