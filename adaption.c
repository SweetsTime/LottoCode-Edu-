/*
    文件：文本框
    说明：掌管实现自适应文本输出框功能
*/

#include "general.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//实现自适应文本框的关键在于计算每行文本的显示长度。
// 这里使用了一个函数 get_display_length 来计算字符串的长度。
// 这个函数会遍历字符串中的每个字符，如果字符是一个三字节的UTF-8字符（假设为中文），则长度加2；
// 如果字符是一个单字节字符（假设为英文），则长度加1。

int get_display_length(char* str) {
    int length = 0;
    while (*str) {
        if (*str == '\t') {  // 制表符
            length += 4;  // 假设一个制表符等于4个空格
            str += 1;
        }
        else if ((*str & 0x80) == 0) {  // 单字节UTF-8字符
            length += 1;
            str += 1;
        }
        else if ((*str & 0xE0) == 0xC0) {  // 双字节UTF-8字符
            length += 2;
            str += 2;
        }
        else if ((*str & 0xF0) == 0xE0) {  // 三字节UTF-8字符
            length += 2;
            str += 3;
        }
        /*else if ((*str & 0xF8) == 0xF0) {  // 四字节UTF-8字符
            length += 2;
            str += 4;
        }*/


        /*while (*str) {
        if ((*str & 0xE0) == 0xE0) {  // 三字节UTF-8字符（假设为中文）
            length += 2;
            str += 3;
        }
        else {  // 单字节字符（假设为英文）
            length += 1;
            str += 1;
        }*/
    }
    return length;
}


//然后，使用 print_with_padding 和 print_with_padding2 函数来打印内容和标题。
// 这两个函数会根据文本框的宽度和当前行的长度，计算出需要添加的空格数量，以确保文本在文本框中居中显示。
void print_with_padding(char* str, int padding) {
    int length = get_display_length(str);
    printf("* %s", str);
    for (int i = 0; i < padding - length + 3; i++) {  //可修改这个+3改变内容右侧*的缩进
        printf(" ");
    }
    printf("*\n");
}

void print_with_padding2(char* str, int padding) {
    int length = get_display_length(str);
    printf("* ");
    for (int i = 0; i < (padding - length) / 2 + 1; i++) {
        printf(" ");
    }
    printf("%s", str);
    for (int i = 0; i < (padding - length + 1) / 2 + 2; i++) {  //可修改这个+2改变标题右侧*的缩进
        printf(" ");
    }
    printf("*\n");
}

//在 print_box 函数中，首先计算标题的长度，然后遍历内容中的每一行，找出最长的一行。这样就可以得到文本框的宽度。
//最后，print_box 函数会打印出上边框、标题、分隔线、内容和下边框，从而形成一个完整的文本框。
// 这个文本框的宽度会自动适应标题和内容中最长的一行，实现了自适应的效果。
void print_box(char* title, char* content) {
    int max_line_length = get_display_length(title);
    char* content_copy = strdup(content);  // 创建内容的副本
    char* line = strtok(content_copy, "\n");

    // 计算最长行的长度
    while (line) {
        int line_length = get_display_length(line);
        if (line_length > max_line_length) {
            max_line_length = line_length;
        }
        line = strtok(NULL, "\n");
    }

    free(content_copy);  // 释放副本的内存
    content_copy = _strdup(content);  // 创建一个新的副本
    line = strtok(content_copy, "\n");

    // 打印上边框
    for (int i = 0; i < max_line_length + 6; i++) {
        printf("*");
    }
    printf("\n");

    // 打印标题
    print_with_padding2(title, max_line_length);  // 使用print_with_padding函数打印标题

    // 打印分隔线
    for (int i = 0; i < max_line_length + 6; i++) {
        printf("=");
    }
    printf("\n");

    // 打印内容
    while (line) {
        print_with_padding(line, max_line_length);
        line = strtok(NULL, "\n");
    }

    // 打印下边框
    for (int i = 0; i < max_line_length + 6; i++) {
        printf("*");
    }
    printf("\n");

    free(content_copy);  // 释放副本的内存
}

//举例来说
//sprintf(title, "管理员主页"); 这行代码将字符串 “管理员主页” 赋值给 title 变量，这将作为文本框的标题。
//sprintf(content, "1.修改管理员信息\n"); 这行代码将字符串 “1.修改管理员信息\n” 赋值给 content 变量，这是文本框的第一行内容。
//sprintf(content, "%s2.手动注册学生\n", content); 这行代码将字符串 “2.手动注册学生\n” 添加到 content 变量的末尾，这是文本框的第二行内容。
    // %s 是一个占位符，它会被 content 变量的当前值替换，这样就可以在 content 的原有内容后面添加新的内容。
//print_box(title, content); 这行代码调用 print_box 函数，将 title 和 content 作为参数传入。这个函数会根据标题和内容的长度创建一个自适应的文本框，并将其打印出来。
    //  这个文本框的宽度会自动适应标题和内容中最长的一行。



/*使用方法1：
void print_all_students() {
    int i;
    char title[100];
    char content[1000];
    if (user_count == 0) {
        printf("暂无学生信息。\n");
        system("pause");
        return;
    }

    sprintf(title, "学生基本信息");
    sprintf(content, "姓名    班级    学号      性别    年龄    状态    密码\n");//空四个空格，学号/性别空六个空格
    for (i = 0; i < user_count; i++) {
        char ageStr[10];
        if (users[i].age == -1) {
            strcpy(ageStr, "未知");
        } else {
            sprintf(ageStr, "%d", users[i].age);
        }

        sprintf(content, "%s%-8s%-8s%-10s%-10s%-10s%-10s%s\n", content,users[i].name, users[i].class, users[i].id ,users[i].gender, ageStr, users[i].frozen == 0 ? "正常" : "冻结", users[i].password);
    }
    print_box(title, content);
    system("pause");
}
*/


/*使用方法2：
system("cls");
        sprintf(title, "学生管理系统");
        sprintf(content, "1.登录账户\n");
        sprintf(content, "%s2.注册账户\n", content);
        sprintf(content, "%s3.忘记密码\n", content);
        sprintf(content, "%s4.访客申请\n", content);
        sprintf(content, "%s5.产品说明\n", content);
        sprintf(content, "%s6.功能测试\n", content);
        sprintf(content, "%s7.退出系统\n", content);
        sprintf(content, "%s当前系统时间: %s", content, __TIME__);
        print_box(title, content);
*/