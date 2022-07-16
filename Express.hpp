#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "MySocket.hpp"
/**
 * @brief 存储快递信息，重载相等运算符，实现std::find函数
 *
 */
struct ExpressInfo
{
  std::string name, send_name, get_name, send_time, get_time, discription, expressman;
  int state = 0, number, type, price, count;
  bool operator==(const ExpressInfo &) const;
};
/**
 * @brief 快递类，实现有关快递的各种操作
 *
 */
class Express // 构造函数，读入快递数据
{
public:
  Express(Server &s) : server(&s)
  {
    num = 0;
    expressfp.open("D:\\ExpressInfo2.txt");
    if (!expressfp)
    {
      std::cout << "Open file error!\n";
      return;
    }
    while (expressfp.peek() != EOF)
    {
      ExpressInfo expressTemp;
      expressfp >> expressTemp.type >> expressTemp.name >> expressTemp.send_time >> expressTemp.get_time >> expressTemp.number >>
          expressTemp.state >> expressTemp.discription >> expressTemp.get_name >> expressTemp.send_name >> expressTemp.expressman >> expressTemp.count;
      if (expressTemp.name.empty()) // 略过空行
        continue;
      else
        expressInfo.emplace_back(expressTemp);
    }
  }
  virtual ~Express() // 析构函数，将商品数据存入文件
  {
    expressfp.close();
    expressfp.open("D:\\ExpressInfo2.txt", std::ios::out | std::ios::trunc);
    expressfp.seekg(0, std::fstream::beg);
    for (auto st : expressInfo)
      expressfp << st.type << " " << st.name << " " << st.send_time << " " << st.get_time << " " << st.number << " "
                << st.state << " " << st.discription << " " << st.get_name << " " << st.send_name << " " << st.expressman << " " << st.count << std::endl;
    expressfp.close();
  }

  virtual double getPrice() const = 0;                                                         // 纯虚函数，返回快递价格
  void addItems(const std::string &, const std::string &);                                     // 发送快递
  void changeItems(const std::string &, const std::string &);                                  // 接收快递
  void collectItems(const std::string &, const std::string &);                                 // 揽收快递
  void search(std::vector<ExpressInfo> &);                                                     // 管理员查看快递，并存入vector中
  void search(const std::string &, std::vector<ExpressInfo> &);                                // 重载，管理员按名称查找快递
  void search(const std::string &, const int, std::vector<ExpressInfo> &);                     // 重载，快递员按状态查找快递
  void search(const int, std::vector<ExpressInfo> &);                                          // 重载，管理员按状态查找商品
  void search_send(const std::string &, std::vector<ExpressInfo> &);                           //管理员按发送用户查找快递
  void search_send(const std::string &, const std::string &, std::vector<ExpressInfo> &);      //重载，快递员按照发送用户查找快递
  void search_get(const std::string &, std::vector<ExpressInfo> &);                            //管理员按接收用户查找快递
  void search_get(const std::string &, const std::string &, std::vector<ExpressInfo> &);       //重载，快递员按照接收用户查找接收快递
  void search_time(const std::string &, std::vector<ExpressInfo> &);                           //管理员按发送时间查找快递
  void search_time(const std::string &, const std::string &, std::vector<ExpressInfo> &);      //重载，快递员按照接收时间查找快递
  void search_time_user(const std::string &, const std::string &, std::vector<ExpressInfo> &); //用户按发送时间查看快递
  void search_collect(const std::string &, std::vector<ExpressInfo> &);                        //快递员查看揽收快递
  void search_num(const int, std::vector<ExpressInfo> &);                                      //管理员按单号查找快递
  void search_num(const std::string &, const int, std::vector<ExpressInfo> &);                 //快递员按单号查找快递
  void search_num_user(const int, const std::string &, std::vector<ExpressInfo> &);            //用户按照单号查找快递
  template <typename T>
  void input(T &) const;                                     //检测非法输入
  void copyInfo(ExpressInfo &) const;                        //将类中数据成员信息拷贝到临时变量中
  void copyInfo(const std::vector<ExpressInfo>::iterator &); //将数组中数据成员信息拷贝到数据成员中
  void input(double &x);
  void input(int &x);                    // 检测输入
  bool isInt(const std::string &) const; // 正则表达式判断输入是否为int类型
  bool isDouble(const std::string &) const;
  void getExpress(std::vector<ExpressInfo> &) const;
  void input(std::string &x);
  bool changeInt(int &);
  int price, count;

protected:
  std::string name, send_name, get_name, send_time, get_time, discription, expressman;
  int state = 0, number, type;
  std::vector<ExpressInfo> expressInfo;
  std::stringstream output;
  Server *server;

private:
  std::fstream expressfp;
  int num;
};

class Ordinary : public Express
{
public:
  Ordinary(Server &s) : Express(s)
  {
    type = 1;
    price = 5;
  }
  Ordinary(const std::string &tname, Server &s) : Express(s)
  {
    type = 1;
    price = 5;
  }
  virtual ~Ordinary() override
  {
  }
  virtual double getPrice() const override;

private:
};

class Fragile : public Express
{
public:
  Fragile(Server &s) : Express(s)
  {
    type = 2;
    price = 8;
  }
  Fragile(const std::string &tname, Server &s) : Express(s)
  {
    type = 2;
    price = 8;
  }
  virtual ~Fragile() override
  {
  }
  virtual double getPrice() const override;

private:
};

class Books : public Express
{
public:
  Books(Server &s) : Express(s)
  {
    type = 3;
    price = 2;
  }
  Books(const std::string &tname, Server &s) : Express(s)
  {
    type = 3;
    price = 2;
  }
  virtual ~Books() override
  {
  }
  virtual double getPrice() const override;

private:
};
