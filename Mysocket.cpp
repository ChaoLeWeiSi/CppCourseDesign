#include "MySocket.hpp"
#include <ostream>
#include <string.h>
#include <string>
#include <vector>

bool Server::serverInit()
{
    WORD w_req = MAKEWORD(2, 2); //°æ±¾ºÅ
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0)
    {
        std::cout << "init WSADATA error" << std::endl;
    }
    else
    {
        std::cout << "init WSADATA success" << std::endl;
    }
    //¼ì²â°æ±¾ºÅ
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
    {
        std::cout << "socket version error" << std::endl;
        // WSACleanup();
    }
    else
    {
        std::cout << "socket version right" << std::endl;
    }
    if ((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        return 0;
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 0;
    }
    else
        printf("...OK!\n");
    if (listen(serverSock, SOMAXCONN) < 0)
    {
        std::cout << "listen error\n";
        // WSACleanup();
        return 0;
    }
    std::cout << "listening...\n";

    int addrLen = sizeof(SOCKADDR);
    acceptSock = accept(serverSock, (SOCKADDR *)&acceptAddr, &addrLen);
    if (acceptSock == SOCKET_ERROR)
    {
        std::cout << "accept error\n";
        // WSACleanup();
        return 0;
    }
    std::cout << "accept success\n";
    return 1;
}
bool Server::sendMessage(std::stringstream &output)
{
    /* std::string message;
    std::vector<char> buff;
    buff.assign(message.cbegin(), message.cend()); */
    /* char buff[1024] = {0};
    output >> buff;
    std::cout << output.str(); */
    try
    {
        sendLength = send(acceptSock, output.str().c_str(), output.str().size(), 0);
        if (sendLength < 0)
        {
            error << "send error " << GetLastError() << std::endl;
            throw error.str();
        }
    }
    catch (const std::string &msg)
    {
        std::cerr << msg;
        exit(1);
    }
    output.str("");
    return true;
}
bool Server::recvMessage(std::string &buff)
{
    buff.clear();
    char buffer[1024] = {0};
    try
    {
        recvLength = recv(acceptSock, buffer, sizeof(buffer), 0);
        if (recvLength < 0)
        {
            error << "recv error " << GetLastError() << std::endl;
            throw error.str();
        }
    }
    catch (const std::string &msg)
    {
        std::cerr << msg;
        exit(1);
    }
    buff.append(buffer, buffer + strlen(buffer));
    std::cout << buff;
    return 1;
}
/* int Server::recvMessage()
{
    int num;
    char buffer[1024];
    recvLength = recv(acceptSock, buffer, sizeof(buffer), 0);
    if (recvLength < 0)
    {
        std::cout << "recv error" << WSAGetLastError() << std::endl;
        return INT_MIN;
    }
    else
        memcpy(&num, buffer, sizeof(num));
    return ntohl(num);
} */

bool Client::clientInit()
{
    WORD w_req = MAKEWORD(2, 2); //°æ±¾ºÅ
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0)
    {
        std::cout << "init WSADATA error" << std::endl;
    }
    else
    {
        std::cout << "init WSADATA success" << std::endl;
    }
    //¼ì²â°æ±¾ºÅ
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
    {
        std::cout << "socket version error" << std::endl;
        // WSACleanup();
    }
    else
    {
        std::cout << "socket version right" << std::endl;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Í¨¹ýipconfig/all²éÑ¯
    serverAddr.sin_port = htons(PORT);

    if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        return false;
    }
    err = connect(serverSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    if (err == SOCKET_ERROR)
    {
        printf("Connect failed:%d", WSAGetLastError());
        return false;
    }

    return true;
}

bool Client::sendMessage(std::string &message)
{
    /* std::vector<char> buff;
    buff.assign(message.cbegin(), message.cend()); */
    message.append("\n");
    try
    {
        sendLength = send(serverSock, message.c_str(), message.size(), 0);
        if (sendLength < 0)
        {
            error << "send error " << GetLastError() << std::endl;
            throw error.str();
        }
    }
    catch (const std::string &msg)
    {
        std::cerr << msg;
        exit(1);
    }
    message.clear();
    return true;
}
/* void Client::sendMessage(const int num)
{
    char buff[10] = {0};
    int tmp = htonl(num);
    memcpy(buff, &tmp, sizeof(tmp));
    sendLength = send(serverSock, buff, strlen(buff), 0);
    if (sendLength < 0)
    {
        std::cout << "send error" << WSAGetLastError() << std::endl;
        // return false;
    }
} */
bool Client::recvMessage(std::string &buff)
{
    buff.clear();
    char buffer[1024] = {0};
    try
    {
        recvLength = recv(serverSock, buffer, sizeof(buffer), 0);
        if (recvLength < 0)
        {
            error << "recv error " << GetLastError() << std::endl;
            throw error.str();
        }
    }
    catch (const std::string &msg)
    {
        std::cerr << msg;
        exit(1);
    }
    buff.append(buffer, buffer + strlen(buffer));
    return 1;
}
