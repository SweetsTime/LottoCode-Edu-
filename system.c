/*
    文件：系统功能
    说明：掌管实现系统功能效果
        1.清屏功能
        2.程序暂停功能（任意键继续）
        3.程序暂停功能（持续x毫秒后继续）
        4.密码隐藏功能（先明文显示等待n秒后隐藏）
*/

#include  "general.h"
#include <stdio.h>
#include <conio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#endif
#include<string.h>
User admin;
User notary;

/*清屏函数*/
void cls() {
    printf("\033[H\033[J");
}

/*间歇函数*/
void pause() {
    printf("按任意键继续...\n");
    getchar();
}

/*断时函数*/
void crosstime(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

//隐藏密码
void stealth_password(char* password) {
    char ch;
    int i = 0;

    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && i > 0) { // 如果输入了删除符，并且密码不为空
            printf("\b \b"); // 删除屏幕上的一个 *
            i--; // 删除密码中的一个字符
        }
        else if (ch != '\b') { // 如果输入的不是删除符
            password[i++] = ch;
            //printf("*");

            printf("%c", ch);
            crosstime(200); // 等待0.2秒后隐藏
            printf("\b*");
        }
    }
    password[i] = '\0'; // 添加字符串结束符
    printf("\n"); // 在密码输入结束后打印一个换行符
}

void flush() {        //清空缓存区
    while (getchar() != '\n');
}

/*超时函数*/
int timeout_input(char* str, int size, int timeout) {
#ifdef _WIN32
    int i = 0;
    DWORD start_time, current_time;

    /* 获取开始时间 */
    start_time = GetTickCount();

    while (1) {
        /* 检查是否有键被按下 */
        if (_kbhit()) {
            /* 有键被按下，读取字符 */
            str[i] = _getch();
            if (str[i] == '\r') {
                /* 回车键被按下，结束输入 */
                str[i] = '\0';
                return 0;
            }
            else {
                /* 其他键被按下，继续输入 */
                i++;
            }
        }

        /* 获取当前时间 */
        current_time = GetTickCount();

        /* 检查是否超时 */
        if (current_time - start_time > timeout * 1000) {
            /* 超时，结束输入 */
            str[i] = '\0';
            return -1;
        }
    }
#else
    fd_set readfds;
    struct timeval tv;
    int result;

    /* 初始化文件描述符集合 */
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    /* 设置超时时间 */
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    /* 等待输入 */
    result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    if (result > 0) {
        /* 有输入，读取输入 */
        fgets(str, size, stdin);
        return 0;
    }
    else if (result == 0) {
        /* 超时，返回错误码 */
        return -1;
    }
    else {
        /* 出错，返回错误码 */
        return -2;
    }
#endif
}

/*超时待机*/
void standby_mode() {
    cls();
    printf("\t\t\t系统待机中...\n");
    ASCIIArtLux();
    getchar();
}
/*测试超时*/
void timeddl() {
    char input[100];
    int result;

    printf("请输入一个字符串（在3秒内）：\n");
    result = timeout_input(input, sizeof(input), 3);
    if (result == -1) {
        /* 输入超时，进入待机状态 */
        standby_mode();
    }
    else if (result == 0) {
        /* 有输入，打印输入 */
        printf("你输入的是：%s\n", input);
        getchar();
    }
    else {
        /* 出错，打印错误信息 */
        printf("输入错误\n");
        getchar();
    }
}

int home() {		//首页

    int choose;
    cls();
    sprintf(title, "系统主页");
    sprintf(content, "1. 账户登录\n");
    sprintf(content, "%s2. 用户注册\n", content);
    sprintf(content, "%s3. 忘记密码\n", content);
    sprintf(content, "%s4. 系统待机\n", content);
    sprintf(content, "%s5. 退出系统\n", content);
    sprintf(content, "%s当前系统时间: %s", content, __TIME__);
    print_box(title, content);
    printf("请选择操作：");
    scanf("%d", &choose);

    return choose;
}