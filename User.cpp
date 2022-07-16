#include "User.hpp"
#include <algorithm>
#include <limits>
#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <regex>
bool AccInfo::operator==(const AccInfo &ac) const
{
    return (this->name == ac.name);
};

int User::search(const std::string &userName) const
{
    AccInfo tmp;
    int pos = -1;
    tmp.name = userName;
    auto st = std::find(accInfo.begin(), accInfo.end(), tmp);
    if (st != accInfo.end())
        pos = st - accInfo.begin();
    return pos;
}
void User::userRegister()
{
    if (search(name) != -1)
    {
        output << "该用户名已存在！\n1";
        server->sendMessage(output);
        return;
    }
    AccInfo tmp;
    std::string fullname, tel, addr;
    tmp.name = name;

    while (1)
    {
        output << "输入密码:\n2";
        server->sendMessage(output);
        std::string confirm;
        // confirmPwd(tmp.pwd);
        server->recvMessage(tmp.pwd);
        tmp.pwd.erase(tmp.pwd.end() - 1);
        output << "请再次输入密码确认:\n2";
        server->sendMessage(output);
        server->recvMessage(confirm);
        confirm.erase(confirm.end() - 1);
        // confirmPwd(confirm);
        if (tmp.pwd == confirm) // 直至两次密码相同才设定成功
            break;
    }
    tmp.t = type;
    tmp.bala = 0;
    output << "输入姓名:\n2";
    server->sendMessage(output);
    input(fullname);
    // server->recvMessage(fullname);
    tmp.fullname = fullname;
    output << "输入电话:\n2";
    server->sendMessage(output);
    input(tel);
    // server->recvMessage(tel);
    tmp.tel = tel;
    if (getUserType() == 1)
    {
        output << "输入地址:\n2";
        server->sendMessage(output);
        // std::cin >> addr;
        input(addr);
        // server->recvMessage(addr);
        tmp.addr = addr;
    }
    else
        tmp.addr = "无";
    accInfo.emplace_back(tmp);
    output << "注册成功!\n1";
    server->sendMessage(output);
}
bool User::login(const int userType)
{
    int pos = search(name);
    if (pos != -1)
    {
        if (accInfo[pos].t != userType)
        {
            output << "用户类型错误，请退出重新选择！\n1";
            server->sendMessage(output);
            return false;
        }
        std::string pwd;
        output << "请输入密码:\n2";
        server->sendMessage(output);
        // confirmPwd(pwd);
        server->recvMessage(pwd);
        pwd.erase(pwd.end() - 1);
        if (pwd != accInfo[pos].pwd)
        {
            output << "密码错误!\n1";
            server->sendMessage(output);
            return false;
        }
        else
        {
            output << "登录成功!\n1";
            server->sendMessage(output);
            password = pwd;
            this->type = userType;
            balance = accInfo[pos].bala;
            num = pos;
            return true;
        }
    }
    else
    {
        output << "用户不存在!\n1";
        server->sendMessage(output);
        return false;
    }
}
bool User::login(const int userType, const std::string &admin)
{
    int pos = search(admin);
    if (pos != -1)
    {

        std::string pwd = "admin888";
        // output << "请输入密码11:\n";
        // confirmPwd("admin888");
        if (pwd != accInfo[pos].pwd)
        {
            output << "密码错误!\n";
            server->sendMessage(output);
            return false;
        }
        else
        {
            // output << "登录成功!\n";
            password = pwd;
            this->type = userType;
            balance = accInfo[pos].bala;
            num = pos;
            return true;
        }
    }
    else
    {
        output << "用户不存在!\n1";
        server->sendMessage(output);
        return false;
    }
}

void User::changePwd()
{
    if (search(name) == -1)
    {
        output << "用户不存在！\n1";
        server->sendMessage(output);
        return;
    }
    std::string pwd;
    output << "输入旧密码:\n2";
    server->sendMessage(output);
    std::string confirm;
    // confirmPwd(confirm);
    server->recvMessage(confirm);
    confirm.erase(confirm.end() - 1);
    if (confirm != password)
    {
        output << "密码错误，已退出！\n1";
        server->sendMessage(output);
        return;
    }
    while (1)
    {
        output << "输入新密码:\n2";
        server->sendMessage(output);
        pwd.erase();
        server->recvMessage(pwd);
        pwd.erase(pwd.end() - 1);
        // confirmPwd(pwd);
        output << "请再次输入密码确认:\n2";
        server->sendMessage(output);
        confirm.erase();
        // confirmPwd(confirm);
        server->recvMessage(confirm);
        confirm.erase(confirm.end() - 1);
        if (pwd == confirm)
            break;
        else
            output << "两次密码不一致，请重新输入！\n";
    }
    password = pwd;
    accInfo[num].pwd = pwd;
    output << "修改成功！\n1";
    server->sendMessage(output);
}
double User::queryBalance() const
{
    return balance;
}
void User::topUpAndDown()
{
    output << name << "，您账户当前余额为" << queryBalance() << "元" << std::endl;
    double money;
    output << "请输入充值或消费金额:(正数表示充值，负数表示消费)\n";
    server->sendMessage(output);
    // while (1)
    // {
    //     input(money);
    //     if (balance + money < 0)
    //         output << "余额不能为负，请重新输入\n";
    //     else
    //         break;
    // }
    input(money);
    balance += money;
    accInfo[num].bala = balance;
    output << name << "，您账户当前余额为" << queryBalance() << "元" << std::endl
           << '1';
    server->sendMessage(output);
}
void User::topUpAndDown(const int price)
{
    if (getUserType() == 1)
    {
        output << name << "，您账户余额为" << queryBalance() << "元" << std::endl;
        if (queryBalance() >= -1 * price)
        {
            balance += price;
            accInfo[num].bala = balance;

            output << name << "，成功发送，您账户当前余额为" << queryBalance() << "元" << std::endl
                   << '1';
        }
        else
        {
            output << name << "，发送失败，您账户当前余额为" << queryBalance() << "元" << std::endl
                   << '1';
        }
    }
    else if (getUserType() == 2)
    {
        output << name << "，您账户余额为" << queryBalance() << "元" << std::endl;
        //  << '1';
        server->sendMessage(output);
        balance += price;
        accInfo[num].bala = balance;
        output << name << "，您账户当前余额为" << queryBalance() << "元" << std::endl;
        //    << '1';
        server->sendMessage(output);
        // if (queryBalance() >= -1 * price)
        // {
        //     balance += price;
        //     accInfo[num].bala = balance;

        //     output << name << "，成功发送，您账户当前余额为" << queryBalance() << "元" << std::endl;
        // }
        // else
        // {
        //     output << name << "，发送失败，您账户当前余额为" << queryBalance() << "元" << std::endl;
        // }
    }
    else
    {
        balance += price;
        accInfo[num].bala = balance;
        if (price > 0)
            output << name << "收到金额" << price << "元" << std::endl
                   << '1';
        else if (price < 0)
            output << name << "减少金额" << price << "元" << std::endl
                   << '1';
        else
            output << name << "金额不变" << std::endl
                   << '1';
    }
}

void User::changeItems(const std::string &expressman, const std::string &addr)
{
    name = expressman;
    this->addr = addr;
    AccInfo tmp;
    copyInfo(tmp); // 当前信息拷贝到tmp中，然后查找
    auto it = std::find(accInfo.begin(), accInfo.end(), tmp);
    if (it == accInfo.end())
    {
        output << "快递员不存在" << std::endl;
        server->sendMessage(output);
        return;
    }
    copyInfo(it);
    getchar(); // 读入上一次输入的回车，防止干扰下面的输入
    name = "无";
    password = "无";
    balance = 0;
    type = 2;
    fullname = "无";
    tel = "无";

    copyInfo(tmp); //将内容更新回数组
    *it = tmp;
    printf("删除成功\n");
}
void User::show(std::vector<AccInfo> &showUser)
{
    int flg = 0;
    for (auto st : accInfo)
    {

        if (!flg)
            output << std::setw(10) << std::left << "用户名" << std::setw(10) << std::left << "密码" << std::setw(10) << std::left << "用户类型"
                   << std::setw(10) << std::left << "账户余额"
                   << std::setw(10) << std::left << "姓名" << std::setw(15) << std::left << "电话" << std::setw(10) << std::left << "地址"
                   << std::endl;
        flg++;
        output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.pwd << std::setw(10) << std::left << st.t
               << std::setw(10) << std::left << st.bala
               << std::setw(10) << std::left << st.fullname << std::setw(15) << std::left << st.tel << std::setw(10) << std::left << st.addr
               << std::endl;
        showUser.emplace_back(st);
    }
    if (!flg)
        output << "没有满足要求的用户，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
std::string User::arrange(std::vector<AccInfo> &showUser)
{
    output << "分配\n";
    int flg = 0, select = 0, sign = 0;
    for (auto st : accInfo)
    {
        if (st.t == 2)
        {
            flg++;
            showUser.emplace_back(st);
        }
    }
    //生成指定范围的随机数 r = rand()%(n - m + 1) + m;
    // output << flg << std::endl;
    select = rand() % (flg) + 1;
    for (auto st : accInfo)
    {
        if (st.t == 2)
        {

            sign++;

            showUser.emplace_back(st);
        }
        if (st.t == 2 && sign == select)
        {
            return st.name;
        }
    }
    if (!flg)
    {
        // output << "没有满足要求的商品，请更换筛选条件。\n";
        return "没有满足要求的商品，请更换筛选条件。\n";
    }
    return "没有满足要求的商品，请更换筛选条件。\n";
}
bool User::isInt(const std::string &input) const
{
    std::regex rx("^\\-?\\d+$"); //+号表示多次匹配
    return std::regex_match(input, rx);
}
bool User::isDouble(const std::string &input) const
{
    std::regex rx("^\\-?\\d+(.\\d+)?$");
    return std::regex_match(input, rx);
}
void User::input(int &x)
{
    std::string tmp;
    server->recvMessage(tmp);
    tmp.erase(tmp.end() - 1);
    if (tmp.empty())
    {
        x = -1;
        return;
    }
    while (!isInt(tmp))
    {
        output << "输入不合法，请输入数字\n";
        server->sendMessage(output);
        server->recvMessage(tmp);
        tmp.erase(tmp.end() - 1);
        if (tmp.empty())
            break;
    }
    if (!tmp.empty())
        x = std::stoi(tmp);
}
void User::input(double &x)
{
    std::string tmp;
    server->recvMessage(tmp);
    tmp.erase(tmp.end() - 1);
    /* if (tmp.empty())
    {
        x = -1;
        return;
    } */
    while (!isDouble(tmp))
    {
        output << "输入不合法，请输入数字\n";
        server->sendMessage(output);
        server->recvMessage(tmp);
        tmp.erase(tmp.end() - 1);
    }
    if (!tmp.empty())
        x = std::stod(tmp);
}
void User::input(std::string &x)
{
    server->recvMessage(x);
    if (!x.empty())
        x.erase(x.end() - 1);
}
void User::copyInfo(AccInfo &des) const
{
    des.name = name;
    des.pwd = password;
    des.bala = balance;
    des.t = type;
    des.fullname = fullname;
    des.tel = tel;
    des.addr = addr;
}
void User::copyInfo(const std::vector<AccInfo>::iterator &sou)
{

    name = sou->name;
    password = sou->pwd;
    balance = sou->bala;
    type = sou->t;
    fullname = sou->fullname;
    tel = sou->tel;
    addr = sou->addr;
}
