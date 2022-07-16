#include "Express.hpp"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <regex>
#include <ctime>
#include <utility>
bool ExpressInfo::operator==(const ExpressInfo &exp) const
{
    return ((name == exp.name) && (type == exp.type));
};
void Express::addItems(const std::string &expressName, const std::string &expressMan)
{
    name = expressName;
    this->send_name = send_name;
    this->get_name = get_name;
    ExpressInfo tmp;
    copyInfo(tmp);
    output << "请输入个数\n";
    server->sendMessage(output);
    // server->recvMessage(count);
    // std::cin >> count;
    input(count);
    output << "请输入发送时间\n";
    server->sendMessage(output);
    input(send_time);
    // server->recvMessage(send_time);
    //  std::cin >> send_time;
    output << "接收时间为待定\n1";
    server->sendMessage(output);
    // input(get_time);
    get_time = "待定";
    // // server->recvMessage(get_time);
    // // get_time.erase(get_time.end() - 1);
    srand((unsigned)time(NULL));
    //生成指定范围的随机数 r = rand()%(n - m + 1) + m;
    output << "单号已分配\n1";
    server->sendMessage(output);
    number = rand() % (99999999 - 9999999) + 10000000;
    output << "状态为待揽收\n1";
    server->sendMessage(output);
    state = 0;
    // input1(state);
    output << "快递描述\n";
    server->sendMessage(output);
    // std::cin >> discription;
    // server->recvMessage(discription);
    input(discription);
    output << "请输入接收用户\n";
    server->sendMessage(output);
    // std::cin >> get_name;
    // server->recvMessage(get_name);
    input(get_name);
    output << "请输入发送用户\n";
    server->sendMessage(output);
    // std::cin >> send_name;
    // server->recvMessage(send_name);
    input(send_name);
    output << "系统分配快递员\n1";
    server->sendMessage(output);
    expressman = expressMan;
    // server->recvMessage(expressman);
    // expressman.erase(expressman.end() - 1);
    output << "添加成功！\n1";
    server->sendMessage(output);
    copyInfo(tmp);
    expressInfo.emplace_back(tmp);
}

void Express::changeItems(const std::string &expressName, const std::string &get_name)
{
    name = expressName;
    this->get_name = get_name;
    ExpressInfo tmp;
    copyInfo(tmp); // 当前信息拷贝到tmp中，然后查找
    auto it = std::find(expressInfo.begin(), expressInfo.end(), tmp);
    if (it == expressInfo.end())
    {
        output << "该快递不存在" << std::endl
               << '1';
        server->sendMessage(output);
        return;
    }

    copyInfo(it);
    state = 1;

    copyInfo(tmp); //将内容更新回数组
    *it = tmp;
    output << "接收完成\n1";
    server->sendMessage(output);
}

void Express::collectItems(const std::string &expressName, const std::string &expressman)
{
    name = expressName;
    this->expressman = expressman;
    ExpressInfo tmp;
    copyInfo(tmp); // 当前信息拷贝到tmp中，然后查找
    auto it = std::find(expressInfo.begin(), expressInfo.end(), tmp);

    if (it == expressInfo.end())
    {
        output << "该商品不存在" << std::endl
               << '1';
        server->sendMessage(output);
        return;
    }
    if (it->expressman != expressman)
    {
        output << "该商品不属于快递员" << expressman << std::endl
               << '1';
        server->sendMessage(output);
        return;
    }
    copyInfo(it);
    state = 2;

    copyInfo(tmp); //将内容更新回数组
    *it = tmp;
    output << "揽收完成\n1";
    server->sendMessage(output);
}
void Express::search(std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::setw(10) << std::left << "快递员" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name << std::setw(10) << std::left << st.expressman
                   << std::endl;
            auto vt = std::find(showExpress.begin(), showExpress.end(), st);
            if (vt == showExpress.end())
                showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}

void Express::search(const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.name == name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::setw(10) << std::left << "快递员" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name << std::setw(10) << std::left << st.expressman
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search(const int state, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.state == state && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search(const std::string &name, const int state, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.expressman == name && st.state == state && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_num(const int number, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.number == number && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_num(const std::string &name, const int number, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.expressman == name && st.number == number && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_num_user(const int number, const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.number == number && st.send_name == name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_send(const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.send_name == name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_send(const std::string &name, const std::string &send_name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.expressman == name && st.send_name == send_name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_get(const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.get_name == name && st.type == type && st.state == 2)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_get(const std::string &name, const std::string &get_name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.expressman == name && st.get_name == get_name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_time(const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.send_time == name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_time(const std::string &name, const std::string &time, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.expressman == name && st.send_time == time && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_time_user(const std::string &time, const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.send_time == time && st.send_name == name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name
                   << std::endl;
            showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}
void Express::search_collect(const std::string &name, std::vector<ExpressInfo> &showExpress)
{
    int flg = 0;
    for (auto st : expressInfo)
    {
        if (st.expressman == name && st.type == type)
        {
            if (!flg)
                output << std::setw(10) << std::left << "名称" << std::setw(10) << std::left << "发送时间"
                       << std::setw(10) << std::left << "接收时间"
                       << std::setw(10) << std::left << "单号" << std::setw(10) << std::left << "状态" << std::setw(10) << std::left << "快递描述"
                       << std::setw(10) << std::left << "接收用户" << std::setw(10) << std::left << "发送用户" << std::setw(10) << std::left << "快递员" << std::endl;
            flg++;
            output << std::setw(10) << std::left << st.name << std::setw(10) << std::left << st.send_time
                   << std::setw(10) << std::left << st.get_time
                   << std::setw(10) << std::left << st.number << std::setw(10) << std::left << st.state << std::setw(10) << std::left << st.discription
                   << std::setw(10) << std::left << st.get_name << std::setw(10) << std::left << st.send_name << std::setw(10) << std::left << st.expressman
                   << std::endl;
            auto vt = std::find(showExpress.begin(), showExpress.end(), st);
            if (vt == showExpress.end())
                showExpress.emplace_back(st);
        }
    }
    if (!flg)
        output << "没有满足要求的商品，请更换筛选条件。\n";
    output << '1';
    server->sendMessage(output);
}

bool Express::isInt(const std::string &input) const
{
    std::regex rx("^\\-?\\d+$"); //+号表示多次匹配
    return std::regex_match(input, rx);
}
bool Express::isDouble(const std::string &input) const
{
    std::regex rx("^\\d+(.\\d+)?$");
    return std::regex_match(input, rx);
}
void Express::input(int &x)
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
void Express::input(double &x)
{
    std::string tmp;
    server->recvMessage(tmp);
    tmp.erase(tmp.end() - 1);
    if (tmp.empty())
    {
        x = -1;
        return;
    }
    while (!isDouble(tmp))
    {
        output << "输入不合法，请输入数字\n";
        server->sendMessage(output);
        server->recvMessage(tmp);
        tmp.erase(tmp.end() - 1);
        if (tmp.empty())
            break;
    }
    if (!tmp.empty())
        x = std::stod(tmp);
}
void Express::input(std::string &x)
{
    server->recvMessage(x);
    if (!x.empty())
        x.erase(x.end() - 1);
}
void Express::getExpress(std::vector<ExpressInfo> &des) const
{
    des = expressInfo;
}
void Express::copyInfo(ExpressInfo &des) const
{
    des.name = name;
    des.send_name = send_name;
    des.get_name = get_name;
    des.send_time = send_time;
    des.get_time = get_time;
    des.discription = discription;
    des.type = type;
    des.state = state;
    des.number = number;
    des.expressman = expressman;
    des.count = count;
}
void Express::copyInfo(const std::vector<ExpressInfo>::iterator &sou)
{

    number = sou->number;
    state = sou->state;
    get_name = sou->get_name;
    send_time = sou->send_time;
    get_time = sou->get_time;
    discription = sou->discription;
    send_name = sou->send_name;
    count = sou->count;
}
double Ordinary::getPrice() const
{
    return price * count;
}
double Fragile::getPrice() const
{
    return price * count;
}
double Books::getPrice() const
{
    return price * count;
}
bool Express::changeInt(int &number)
{
    std::string tmp;
    // std::getline(std::cin, tmp);
    server->recvMessage(tmp);
    if (tmp.size() == 1)
        return true;
    tmp.erase(tmp.end() - 1);
    if (isInt(tmp))
    {
        int t = std::stoi(tmp);
        /* if (t < 0)
        {
            output << "输入不合法，请输入正数！\n";
            return false;
        } */
        number = t;
        return true;
    }
    else
    {
        output << "输入不合法，请输入正数！\n";
        server->sendMessage(output);
        return false;
    }
}
