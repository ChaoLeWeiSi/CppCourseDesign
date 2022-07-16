#pragma once
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <sstream>

// #    define PORT 300

class Server
{
public:
  Server() : serverSock(INVALID_SOCKET), acceptSock(INVALID_SOCKET)
  {
    /* WORD w_req = MAKEWORD(2, 2);  //°æ±¾ºÅ
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0)
    {
        std::cout << "³õÊ¼»¯Ì×½Ó×Ö¿âÊ§°Ü£¡" << std::endl;
    }
    else
    {
        std::cout << "³õÊ¼»¯Ì×½Ó×Ö¿â³É¹¦£¡" << std::endl;
    }
    //¼ì²â°æ±¾ºÅ
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
    {
        std::cout << "Ì×½Ó×Ö¿â°æ±¾ºÅ²»·û£¡" << std::endl;
        // WSACleanup();
    }
    else
    {
        std::cout << "Ì×½Ó×Ö¿â°æ±¾ÕýÈ·£¡" << std::endl;
    } */
  }
  ~Server()
  {
    closesocket(serverSock);
    closesocket(acceptSock);
    // WSACleanup();
  }
  bool serverInit();
  bool sendMessage(std::stringstream &message);
  bool recvMessage(std::string &buff);
  // int recvMessage();

private:
  SOCKET serverSock, acceptSock;
  SOCKADDR_IN serverAddr, acceptAddr;
  std::stringstream error;
  int sendLength, recvLength;
  const int PORT = 8889;
};

class Client
{
public:
  Client() : serverSock(INVALID_SOCKET)
  {
    /* WORD w_req = MAKEWORD(2, 2);  //°æ±¾ºÅ
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0)
    {
        std::cout << "³õÊ¼»¯Ì×½Ó×Ö¿âÊ§°Ü£¡" << std::endl;
    }
    else
    {
        std::cout << "³õÊ¼»¯Ì×½Ó×Ö¿â³É¹¦£¡" << std::endl;
    }
    //¼ì²â°æ±¾ºÅ
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
    {
        std::cout << "Ì×½Ó×Ö¿â°æ±¾ºÅ²»·û£¡" << std::endl;
        // WSACleanup();
    }
    else
    {
        std::cout << "Ì×½Ó×Ö¿â°æ±¾ÕýÈ·£¡" << std::endl;
    } */
  }
  ~Client()
  {
    closesocket(serverSock);
    // WSACleanup();
  }
  bool clientInit();
  bool sendMessage(std::string &message);
  // void sendMessage(const int num);
  bool recvMessage(std::string &buff);

private:
  SOCKET serverSock;
  SOCKADDR_IN serverAddr;
  std::stringstream error;
  int sendLength, recvLength;
  const int PORT = 8889;
};
