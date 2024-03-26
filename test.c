/*
    文件：测试功能
    说明：掌管实现功能测试效果，程序执行顺序优于正式功能
        1.
*/

#include "general.h"
#include <stdio.h>
#include <stdlib.h>

void test() {

}

void function_test() {
    cls();
    int choice;
    int c;
    int shouldContinue;
    char id[10];
    int pet[7];
    int bets;
    char type;
    char name[10];
    char password[10];

    cls();
    sprintf(title, "功能测试");
    sprintf(content, "01. 文本框架\n");
    sprintf(content, "%s02. 用户注册\n", content);
    sprintf(content, "%s03. 隐藏密码\n", content);
    sprintf(content, "%s04. 超时函数\n", content);
    sprintf(content, "%s05. 查找目标\n", content);
    sprintf(content, "%s06. 查找全体\n", content);
    sprintf(content, "%s07. 彩票发行\n", content);
    sprintf(content, "%s08. 单体彩票\n", content);
    sprintf(content, "%s09. 全体彩票\n", content);
    sprintf(content, "%s10. 用户钱包\n", content);
    sprintf(content, "%s11. 用户购彩\n", content);
    //sprintf(content, "%s12. ABCDEFGH\n", content);
    //sprintf(content, "%s13. IJKLMNOP\n", content);
    //sprintf(content, "%s14. QRSTUVWX\n", content);
    //sprintf(content, "%s15. YZabcdef\n", content);
    //sprintf(content, "%s16. ghijklmn\n", content);
    //sprintf(content, "%s17. opqrstuv\n", content);
    //sprintf(content, "%s18. wxyz1234\n", content);
    sprintf(content, "%s00. 退出测试\n", content);
    sprintf(content, "%s当前系统时间: %s", content, __TIME__);
    print_box(title, content);
    printf("请选择操作：");
    scanf("%d", &choice);
    while ((c = getchar()) != '\n' && c != EOF) {}

}