#include "PlatformShow.hpp"
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

void PlatformShow::show()
{
    // int action = 1;
    bool skip = false, encryption = false, aBlankLine = false;
    while (1)
    {
        /* std::cout << "选择操作：\n"
                  << "1. 用户中心\n"
                  << "2. 商品展示\n"
                  << "3. 商品信息修改（针对商家）\n"
                  << "其他数字 退出平台" << std::endl; */

        std::string buff;
        client.recvMessage(buff);
        std::string tmp;

        /* if (*(buff.end() - 1) == '1')
        {
            buff.erase(buff.end() - 1);
            flg = 1;
        }
        else if (*(buff.end() - 1) == '0')
        {
            break;
        } */
        if (*(buff.end() - 1) == '0')
            break;
        switch (*(buff.end() - 1))
        {
        case '1':
            skip = true;
            break;
        case '2':
            encryption = true;
            break;
        case '3':
            aBlankLine = true;
            break;
        default:
            break;
        }
        if (buff.size() && *(buff.end() - 1) != '\n')
            buff.erase(buff.end() - 1);
        std::cout << buff;
        if (skip)
        {
            skip = false;
            continue;
        }
        if (encryption)
        {
            encryption = false;
            inputPwd(tmp);
        }
        else
        {
            while (1)
            {
                std::getline(std::cin, tmp);
                if (!aBlankLine && tmp.empty())
                    std::cout << "不能输入空行，请重新输入！\n";
                else
                {
                    aBlankLine = false;
                    break;
                }
            }
        }
        client.sendMessage(tmp);
        /* switch (action)
        {
            case 1:
                userCenter();
                break;
            case 2:
                goodsInformation();
                break;
            case 3:
                changeGoods();
                break;
            default:
                break;
        } */
    }
    std::cout << "感谢使用！" << std::endl;
}
void PlatformShow::inputPwd(std::string &tmpPwd) const
{
    while (1)
    {
        char ch;
        ch = _getch();     //读取一个字符并且不显示到屏幕上
        if (ch == VK_BACK) //输入backspace
        {
            if (!tmpPwd.empty())
            {
                std::cout << "\b \b" << std::flush;
                tmpPwd.erase(tmpPwd.length() - 1); // 清除掉读入的上一个字符
            }
        }
        else if (ch == VK_RETURN) // 输入回车
        {
            std::cout << std::endl;
            // tmpPwd += '\n';  // 客户端会删除回车
            break;
        }
        else
        {
            tmpPwd += ch; // 读入字符
            _putch('*');  // 输出*
        }
    }
}
