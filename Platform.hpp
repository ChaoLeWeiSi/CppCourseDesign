#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Express.hpp"
#include "User.hpp"

/**
 * @brief 平台类，用来展示信息以及选择操作
 *
 */
class Platform
{
public:
  Platform()
  {
    server.serverInit();
    user = nullptr;
    express = nullptr;
  }
  ~Platform()
  {
  }
  void show();                  // 展示基础界面
  void userCenter();            // 用户中心界面
  void userRegisterOrLog();     // 注册登录功能
  void userInformationChange(); // 修改用户信息
  void userQuit();              // 用户退出
  void expressInformation();    // 快递信息
  void userInformation();       // 用户信息
  void actExpress();            //操作快递
  void expressTask();           //分配任务
  void definiteType();          // 确定用户种类
  void freeUser();              // 释放用户指针
  void freeExpress();           // 释放快递指针
  void input(int &x);           // 检查输入
  void input(double &x);
  void input(std::string &x);
  bool isInt(const std::string &input) const;
  bool isDouble(const std::string &input) const;

private:
  std::string name; // 用户名
  User *user;       // 用户指针
  User *user1;      // 用户指针1
  User *user2;      // 用户指针2
  Express *express; // 快递指针
  std::vector<ExpressInfo> allExpress;
  std::vector<ExpressInfo> showExpress; // 展示的快递
  std::vector<AccInfo> showUser;        // 展示的用户
  std::stringstream output, inputStream;
  Server server;
};
