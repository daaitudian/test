//#include "exam.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iniparser/iniparser.h>
#include <sys/types.h>

/**
 * 
 * -------------------------------------------------------------------------------------------------------------------
 * 题目
 * -------------------------------------------------------------------------------------------------------------------
 * 
 * 遵循 https://specifications.freedesktop.org/desktop-entry-spec/latest/ 标准实现一个desktop文件解析工具
 * 
 * 功能要求
 * - desktop文件格式无错误时进程退出码为0
 * - 实现对desktop文件Name(程序名称)、Exec(可执行文件)、Icon(图标文件)三个字段的解析
 * - 需要检测文件的格式，遇到错误时以退出码 1 退出进程
 * - 输入接收一个desktop文件的绝对路径，能够输出程序名称、图标文件（图标文件不考虑从系统图标主题中查找）
 * - 能够在指定一个desktop文件后启动进程，且允许为其指定参数
 * 
 * 其它要求
 * - 使用cmake构建工程
 * - 可执行文件位置和名称为：./src/freedesktop
 * - testHelper.sh有很少的测试用例，可以帮助做初步验证
 * 
 * 示例
 * - a.desktop文件内容为:
 *     [Desktop Entry]
 *     Name=A;
 *     Name[zh_CN]="我是A";
 *     Exec=cat %f
 *     Icon=/tmp/a.png
 * 
 * - 输入参数（-d指定desktop文件的绝对路径，-n表示要获取应用程序名称）：
 *     freedesktop -d /home/a/a.desktop -n
 * - 输出结果：
 *     A或我是A（中文环境下，注意应该去掉引号）
 * 
 * - 输入参数（-i表示要获取应用程序的图标）
 *     freedesktop -d /home/a/a.desktop -i
 * - 输出结果（如图标数据不是一个文件路径，则先将图标文件保存为文件后再返回此文件的绝对路径）：
 *     /tmp/a.png
 * 
 * - 输入参数（-e表示要启动此应用程序，其后可根一个或多个传入参数）
 *     freedesktop -d /home/a/a.desktop -e /home/a/test.txt
 * - 输出结果：
 *     输出结果为 /home/a/test.txt 文件的内容，也就是Exec命令执行时对标准输出通道所写入的全部数据
 * 
 */
int main(int argc, char* argv[])
{
    char filepath[1024];
    char param;
    char outputfile[1024];
    const char * processname;
    const char * pngpath;
    const char * cmdstring;
    FILE *curlang;
    char * currentlang;
    char * cmdstr;
    char cmdfull[128] = {0};
    char cmdfull1[128] = {0};
    int langflag  = 0;
    dictionary *ini;
    const char * optstr = "d:ine:";
    int u_d = 0, u_e = 0, u_i = 0, u_n = 0;

    if(argc < 4){
        printf("usage:\n");
        return 2;
    }

    while((param=getopt(argc, argv, optstr)) != -1){
        switch(param){
            case 'd':
                u_d = 1;
                strcpy(filepath, optarg);
                break;
            case 'i':
                u_i = 1;
                break;
            case 'n':
                u_n = 1;
                break;
            case 'e':
                u_e = 1;
                strcpy(outputfile, optarg);
                break;
            default:
                printf("error paramer\n");
                break;
        }
    }
    if(!u_d){
        printf("error paramer\n");
        return 2;
    }

    ini = iniparser_load(filepath);
    if(ini == NULL){
        return 1;
    }

    currentlang = getenv("LANG");
    
    if(u_n){
        if(currentlang){
            if(strstr(currentlang, "zh_CN")){
                processname = iniparser_getstring(ini, "Desktop Entry:Name[zh_CN]","null");
            }else{
                processname = iniparser_getstring(ini, "Desktop Entry:Name", "null");
            }
        }
        if(processname){
            printf("%s\n", processname);
        }
    }
    
    if(u_i){
        pngpath = iniparser_getstring(ini, "Desktop Entry:Icon", "null");
        if(pngpath){
            printf("%s\n", pngpath);
        }
    }

    if(u_e){
        cmdstring = iniparser_getstring(ini, "Desktop Entry:Exec", "null");
        strcpy(cmdfull, cmdstring);
        if(outputfile){
            cmdstr = strstr(cmdfull, "%f");
            if(cmdstr){
                *(cmdstr + 1) = 's';
                sprintf(cmdfull1, cmdfull, outputfile);
            }
            system(cmdfull1);
        }
    }
    return 0;
}
