#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include "Express.hpp"

/**
 * @brief 存储用户信息，重载相等运算符，实现std::find函数
 *
 */
struct AccInfo
{
  std::string name, pwd, fullname, tel, addr;
  double bala;
  int t;
  bool operator==(const AccInfo &ac) const;
};

/**
 * @brief 用户类，实现有关用户的各种操作
 *
 */
class User
{
public:
  User(Server &s) : server(&s), type(0), balance(0) // 构造函数，读入用户数据
  {
    num = 0;
    accfp.open("D:\\AccInfo2.txt");
    if (!accfp)
    {
      std::cout << "打开文件失败！\n";
      return;
    }
    while (accfp.peek() != EOF)
    {
      AccInfo accTemp;
      accfp >> accTemp.name >> accTemp.pwd >> accTemp.t >> accTemp.bala >> accTemp.fullname >> accTemp.tel >> accTemp.addr;
      if (accTemp.name.empty()) // 略过空行
        continue;
      else
        accInfo.emplace_back(accTemp);
    }
  }
  virtual ~User() // 析构函数，将用户数据存入文件
  {
    accfp.close();
    accfp.open("D:\\AccInfo2.txt", std::ios::out | std::ios::trunc);
    accfp.seekg(0, std::fstream::beg);
    for (auto st : accInfo)
      accfp << st.name << " " << st.pwd << " " << st.t << " " << st.bala << st.fullname << " " << st.tel << " " << st.addr << std::endl;
    accfp.close();
  }

  virtual int getUserType() const = 0; // 纯虚函数，返回用户类型

  int search(const std::string &userName) const; // 查找用户名是否存在，若存在返回在数组中位置
  void userRegister();                           // 用户注册
  bool login(const int userType);                // 用户登录

  bool login(const int userType, const std::string &admin);   // 用户登录
  void changePwd();                                           // 修改密码
  void confirmPwd(std::string &tmpPwd) const;                 // 确认密码是否正确
  double queryBalance() const;                                // 查询余额
  void topUpAndDown();                                        // 修改余额
  void topUpAndDown(const int price);                         // 修改余额
  void changeItems(const std::string &, const std::string &); // 删除快递员
  void show(std::vector<AccInfo> &);                          // 查找快递，并存入vector中，实验二中用
  std::string arrange(std::vector<AccInfo> &);                //分配快递员
  void input(std::string &x);
  void input(int &x); // 检测输入
  void input(double &x);
  bool isInt(const std::string &input) const;
  bool isDouble(const std::string &input) const;
  void copyInfo(AccInfo &) const;                        //将类中数据成员信息拷贝到临时变量中
  void copyInfo(const std::vector<AccInfo>::iterator &); //将数组中数据成员信息拷贝到数据成员中
  //修改为set get
protected:
  std::string name, password, fullname, tel, addr; // 用户名与密码
  // bool logged;
  int type;       // 用户类型
  double balance; // 用户余额
  // std::vector<AccInfo> accInfo;
  std::stringstream output;
  Server *server;

private:
  std::fstream accfp;           // 文件指针
  std::vector<AccInfo> accInfo; // 所有用户信息的数组
  int num;                      // 该用户在数组中下标
};

/**
 * @brief 用户类型
 *
 */
class Subscriber : public User
{
public:
  Subscriber(const std::string ss, Server &s) : User(s)
  {
    name = ss;
    type = 1;
  }
  virtual ~Subscriber() override
  {
  }
  virtual int getUserType() const override
  {
    return type;
  }

private:
};
/**
 * @brief 快递员类型
 *
 */
class Expressman : public User
{
public:
  Expressman(const std::string ss, Server &s) : User(s)
  {
    name = ss;
    type = 2;
  }
  virtual ~Expressman() override
  {
  }
  virtual int getUserType() const override
  {
    return type;
  }

private:
};

/**
 * @brief 管理员类型
 *
 */
class Admin : public User
{
public:
  Admin(const std::string ss, Server &s) : User(s)
  {
    name = ss;
    type = 0;
  }
  virtual ~Admin() override
  {
  }
  virtual int getUserType() const override
  {
    return type;
  }
};
