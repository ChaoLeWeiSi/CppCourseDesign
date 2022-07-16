#include "Platform.hpp"
#include <cstring>
#include <iostream>
#include <string.h>
#include <string>
#include <limits>
#include <regex>
void Platform::show()
{
    int action;
    do
    {
        output << "选择操作：\n"
               << "1. 用户中心\n"
               << "其他数字 退出平台" << std::endl;
        server.sendMessage(output);
        input(action);
        switch (action)
        {
        case 1:
            userCenter();
            break;
        default:
            break;
        }
        // output << std::endl;
    } while (action > 0 && action < 2);
    if (action != -1)
    {
        output << "感谢使用！" << std::endl
               << "0";
        server.sendMessage(output);
    }
}
void Platform::userCenter()
{
    if (name.empty()) // 未登录情况下只能注册或登录
        userRegisterOrLog();
    else // 已登录
    {
        int choice;
        std::string expressman, expressName;
        if (user->getUserType() == 1)
        {

            do
            {
                output << "选择操作：\n"
                       << "1. 用户信息查询与修改\n"
                       << "2. 注销登录\n"
                       << "3. 操作快递\n"
                       << "其他数字 退出" << std::endl;
                server.sendMessage(output);
                input(choice);
                switch (choice)
                {
                case 1:
                    userInformationChange();
                    break;
                case 2:
                    userQuit();
                    return;
                case 3:
                    actExpress();
                    break;
                default:
                    break;
                }
            } while (choice > 0 && choice < 4);
        }
        else if (user->getUserType() == 2)
        {

            do
            {
                output << "选择操作：\n"
                       << "1. 快递员信息查询与修改\n"
                       << "2. 注销登录\n"
                       << "3. 快递任务查询\n"
                       << "4. 快递员揽收\n"
                       << "其他数字 退出" << std::endl;
                server.sendMessage(output);
                input(choice);
                switch (choice)
                {
                case 1:
                    userInformationChange();
                    break;
                case 2:
                    userQuit();
                    return;
                case 3:
                    expressTask();
                    break;
                case 4:
                    definiteType();
                    output << "输入快递名称\n";
                    server.sendMessage(output);
                    input(expressName);
                    // std::cin >> expressName;
                    // output << name;
                    express->collectItems(expressName, name);
                    user->topUpAndDown((express->getPrice()) * 0.5);
                    user1 = new Admin("admin", server);
                    if (!user1->login(0, "admin")) // 登录失败情况下
                    {
                        freeUser();
                        name.clear();
                    }
                    user1->topUpAndDown(express->getPrice() * -0.5);
                    delete user1;
                    user1 = nullptr;
                default:
                    break;
                }
            } while (choice > 0 && choice < 5);
        }
        else
        {

            do
            {
                output << "选择操作：\n"
                       << "1. 用户信息查询与修改\n"
                       << "2. 注销登录\n"
                       << "3. 管理员查看快递信息\n"
                       << "4. 管理员查看用户信息\n"
                       << "5. 添加快递员\n"
                       << "6. 删除快递员\n"
                       << "其他数字 退出" << std::endl;
                server.sendMessage(output);
                input(choice);
                switch (choice)
                {
                case 1:
                    userInformationChange();
                    break;
                case 2:
                    userQuit();
                    return;
                case 3:
                    expressInformation();
                    break;
                case 4:
                    userInformation();
                    break;
                case 5:
                    output << "输入快递员用户名\n";
                    server.sendMessage(output);
                    // std::cin >> name;
                    input(name);
                    user2 = new Expressman(name, server);
                    user2->userRegister();
                    delete user2;
                    user2 = nullptr;
                    name.clear();
                    break;
                case 6:
                    // freeUser();

                    output << "输入快递员用户名\n";
                    server.sendMessage(output);
                    std::cin >> name;
                    user->changeItems(name, "无");
                    freeUser();
                    user = new Admin("admin", server);
                    if (!user->login(0, "admin")) // 登录失败情况下
                    {
                        freeUser();
                        name.clear();
                    }
                    break;
                default:
                    break;
                }
            } while (choice > 0 && choice < 7);
        }
    }
}
void Platform::userRegisterOrLog()
{
    output << "选择操作：\n"
           << "1. 用户注册\n"
           << "2. 用户登录\n"
           << "其他数字 退出" << std::endl;
    server.sendMessage(output);
    int choice, type;
    input(choice);
    std::string operation[2] = {"注册", "登录"};
    if (choice == 1 || choice == 2)
    {
        output << "请输入" << operation[choice - 1] << "的用户类型：\n"
               << "1表示用户 2表示快递员\n";
        server.sendMessage(output);
        input(type);
        if (type > 2 || type < 0 || (type == 0 && choice == 1)) // 只有登录时能选择管理员类型，快递员不能注册，需由管理员添加
        {
            output << "没有该类型用户，已退出\n";
            server.sendMessage(output);
            return;
        }
        output << "输入用户名：\n";
        server.sendMessage(output);
        // std::cin >> name;
        input(name);
        if (name.empty())
            return;
        freeUser();
        switch (type)
        {
        case 0:
            user = new Admin(name, server);
            break;
        case 1:
            user = new Subscriber(name, server);
            break;
        case 2:
            user = new Expressman(name, server);
            break;
        default:
            break;
        }
    }
    switch (choice)
    {
    case 1:
        user->userRegister();
        freeUser();
        name.clear();
        break;
    case 2:
        if (!user->login(type)) // 登录失败情况下
        {
            freeUser();
            name.clear();
        }
        break;
    default:
        break;
    }
}
void Platform::userInformationChange()
{
    int choice;
    std::string expressName;
    do
    {
        output << name << ", 请选择操作：\n"
               << "1. 修改密码\n"
               << "2. 余额查询\n"
               << "3. 余额充值与消费\n"
               << "其他数字 退出" << std::endl;
        server.sendMessage(output);
        input(choice);
        switch (choice)
        {
        case 1:
            user->changePwd();
            break;
        case 2:
            output << name << "，您当前账户余额为" << user->queryBalance() << "元\n1";
            server.sendMessage(output);
            break;
        case 3:
            user->topUpAndDown();
            break;

        default:
            break;
        }
    } while (choice > 0 && choice < 4);
}
void Platform::userQuit()
{

    output << "已退出！\n1";
    server.sendMessage(output);
    name.clear();
    showExpress.clear();
    freeUser();
}
void Platform::expressInformation()
{
    int choice;
    output << "请选择筛选条件\n"
           << "0. 展示某类商品\n"
           << "1. 按名称筛选\n"
           << "2. 按发送用户筛选\n"
           << "3. 按收用户筛选\n"
           << "4. 发送时间筛选\n"
           << "5. 按状态筛选\n"
           << "6. 按单号筛选\n"
           << "其他数字 退出\n";
    server.sendMessage(output);
    int number, state;
    // double priceLow, priceHigh;
    input(choice);
    std::string expressName;
    std::string sendName;
    std::string getName;
    std::string send_time;
    if (choice >= 0 && choice <= 6)
        definiteType();
    else
        return;
    express->getExpress(allExpress);
    switch (choice)
    {
    case 0:
        express->search(showExpress);
        break;
    case 1:
        output << "请输入快递名称\n";
        server.sendMessage(output);
        // std::cin >> expressName;
        input(expressName);
        express->search(expressName, showExpress);
        break;
    case 2:
        output << "请输入发送用户名称\n";
        server.sendMessage(output);
        // std::cin >> sendName;
        input(sendName);
        express->search_send(sendName, showExpress);
        break;
    case 3:
        output << "请输入接收用户名称\n";
        server.sendMessage(output);
        // std::cin >> getName;
        input(getName);
        express->search_get(getName, showExpress);
        break;
    case 4:
        output << "请输入发送时间\n";
        server.sendMessage(output);
        // std::cin >> send_time;
        input(send_time);
        express->search_time(send_time, showExpress);
        break;
    case 5:
        output << "请输入状态\n";
        server.sendMessage(output);
        // std::cin >> state;
        input(state);
        express->search(state, showExpress);
        break;
    case 6:
        output << "请输入单号\n";
        server.sendMessage(output);
        // std::cin >> number;
        input(number);
        express->search_num(number, showExpress);
        break;

    default:
        break;
    }
    //          !
    // output << "1";
    // server.sendMessage(output);
    freeExpress();
    //清空vector
    std::vector<ExpressInfo> tmp;
    showExpress.swap(tmp);
}
void Platform::userInformation()
{
    user->show(showUser);
    output << "1";
    server.sendMessage(output);
    freeUser();
    //清空vector
    std::vector<AccInfo> tmp;
    showUser.swap(tmp);
}
void Platform::actExpress()
{
    // 未登录情况下
    if (name.empty())
    {
        output << "请登入账号后进行该操作！\n1";
        server.sendMessage(output);
        return;
    }
    int choice, act, number, cost;
    do
    {
        output << "请选择要进行的操作\n"
               << "1. 发送快递\n"
               << "2. 接收快递\n"
               << "3. 查询快递\n"
               << "其他数字 退出\n";
        server.sendMessage(output);
        input(choice);
        std::string expressName, send_time, expressman; // if ((choice > 0 && choice < 4) || (choice == 0 && user->getUserType() == 0))
        if ((choice > 0 && choice < 8) || (choice == 0 && user->getUserType() == 0))
        {
            definiteType();
            if (choice > 0 && choice < 3 || choice == 7)
            {
                output << "输入快递名称\n";
                server.sendMessage(output);
                input(expressName);
                // std::cin >> expressName;
            }
        }
        switch (choice)
        {
        case 1:
            expressman = user->arrange(showUser);
            // output << test << std::endl;
            express->addItems(expressName, expressman);

            // // output << "请输入个数\n";
            // // std::cin >> num;
            user->topUpAndDown((express->getPrice()) * -1);
            if (user->queryBalance() >= (express->getPrice()))
            {
                user1 = new Admin("admin", server);
                if (!user1->login(0, "admin")) // 登录失败情况下
                {
                    freeUser();
                    name.clear();
                }
                user1->topUpAndDown(express->getPrice());
                delete user1;
                user1 = nullptr;
            }
            else
                output << "admin未收到金额\n";
            break;
        case 2:
            //     express->getExpress(expressName, user1);

            express->changeItems(expressName, name);
            break;
        case 3:
            // express->queryExpress)(expressName);
            // expressInformation();
            output << "请选择要进行的操作\n"
                   << "1. 查询发送快递\n"
                   << "2. 查询接收快递\n"
                   << "3. 根据时间查询快递\n"
                   << "4. 根据单号查询快递\n"
                   << "其他数字 退出\n";
            server.sendMessage(output);
            input(act);
            switch (act)
            {
            case 1:
                express->search_send(name, showExpress);
                break;
            case 2:
                express->search_get(name, showExpress);
                break;
            case 3:
                output << "请输入时间\n";
                server.sendMessage(output);
                input(send_time);
                // std::cin >> send_time;
                express->search_time_user(send_time, name, showExpress);
                break;
            case 4:
                output << "请输入单号\n";
                server.sendMessage(output);
                input(number);
                // std::cin >> number;
                express->search_num_user(number, name, showExpress);
                break;
            default:
                break;
            }

            break;

        default:
            break;
        }
    } while (choice > 0 && choice < 4);
    freeExpress();
}
void Platform::expressTask()
{
    int choice, number, state;
    std::string expressName, send_time, sendName, getName;
    output << "请选择要进行的操作\n"
           << "1. 查询揽收快递信息\n"
           << "2. 根据发送人查询快递\n"
           << "3. 根据接受人查询快递\n"
           << "4. 根据时间查询快递\n"
           << "5. 根据单号查询快递\n"
           << "6. 根据状态查询快递\n"
           << "其他数字 退出\n";
    server.sendMessage(output);
    input(choice);
    if (choice >= 1 && choice < 7)
        definiteType();
    else
        return;
    switch (choice)
    {
    case 1:

        output << "查看揽收信息\n1";
        server.sendMessage(output);
        // definiteType();
        express->search_collect(name, showExpress);
        break;
    case 2:
        // definiteType();
        output << "请输入发送用户名称\n";
        server.sendMessage(output);
        // std::cin >> sendName;
        input(sendName);
        express->search_send(name, sendName, showExpress);
        break;
    case 3:
        // definiteType();
        output << "请输入接收用户名称\n";
        server.sendMessage(output);
        // std::cin >> getName;
        input(getName);
        express->search_get(name, getName, showExpress);
        break;
    case 4:
        // definiteType();
        output << "请输入时间\n";
        server.sendMessage(output);
        // std::cin >> send_time;
        input(send_time);
        express->search_time(name, send_time, showExpress);
        break;
    case 5:
        // definiteType();
        output << "请输入单号\n";
        server.sendMessage(output);
        // std::cin >> number;
        input(number);
        express->search_num(name, number, showExpress);
        break;
    case 6:
        // definiteType();
        output << "请输入状态\n";
        server.sendMessage(output);
        // std::cin >> state;
        input(state);
        express->search(name, state, showExpress);
        break;
    default:
        break;
    }
}
void Platform::definiteType()
{
    int type;
    output << "请选择商品类型\n"
           << "1表示普通快递，2表示易碎品，3表示图书\n";
    server.sendMessage(output);
    input(type);
    freeExpress();
    while (type < 1 || type > 3)
    {
        output << "没有该类型商品，请重新输入\n";
        input(type);
    }
    switch (type)
    {
    case 1:
        express = new Ordinary(name, server);
        break;
    case 2:
        express = new Fragile(name, server);
        break;
    case 3:
        express = new Books(name, server);
        break;
    default:
        break;
    }
}
void Platform::input(int &x)
{
    /* std::cin >> x;

    while (std::cin.fail() || std::cin.get() != '\n')
    {
        std::cin.clear();
        std::cin.ignore(LLONG_MAX, '\n');
        output << "输入不合法，请输入数字\n";
        std::cin >> x;
        continue;
    }
    inputStream >> x; */
    std::string tmp;
    server.recvMessage(tmp);
    tmp.erase(tmp.end() - 1);
    if (tmp.empty())
    {
        x = -1;
        return;
    }
    while (!isInt(tmp))
    {
        output << "输入不合法，请输入数字\n";
        server.sendMessage(output);
        server.recvMessage(tmp);
        tmp.erase(tmp.end() - 1);
        if (tmp.empty())
            break;
    }
    if (!tmp.empty())
        x = std::stoi(tmp);
}
void Platform::input(double &x)
{
    std::string tmp;
    server.recvMessage(tmp);
    tmp.erase(tmp.end() - 1);
    while (!isDouble(tmp))
    {
        output << "输入不合法，请输入数字\n";
        server.sendMessage(output);
        server.recvMessage(tmp);
        tmp.erase(tmp.end() - 1);
        if (tmp.empty())
            break;
    }
    if (!tmp.empty())
        x = std::stod(tmp);
}
void Platform::input(std::string &x)
{
    server.recvMessage(x);
    if (!x.empty())
        x.erase(x.end() - 1);
}
bool Platform::isInt(const std::string &input) const
{
    std::regex rx("^\\-?\\d+$"); //+号表示多次匹配
    return std::regex_match(input, rx);
}
bool Platform::isDouble(const std::string &input) const
{
    std::regex rx("^\\d+(.\\d+)?$");
    return std::regex_match(input, rx);
}
void Platform::freeUser()
{
    if (user)
    {
        delete user;
        user = nullptr;
    }
}
void Platform::freeExpress()
{
    if (express)
    {
        delete express;
        express = nullptr;
    }
}
