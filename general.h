#pragma once
#ifndef LOTTO_H
#define LOTTO_H
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/*下注规则：
注数范围为1-5
A型彩票：每期99个号码，每号每注5元，下注为单号下注。
    1注=1倍奖金
    2注=2倍奖金
    3注=3倍奖金
    4注=4倍奖金
    5注=5倍奖金

B型彩票：每期495个号码，中奖号码为五个组合，例如“01 56 77 24 96”，不能给相同号码重复下注，但每人能买1-5个号码，重复购买为多注。
    1注=1倍奖金
    2注=3倍奖金
    3注=5倍奖金
    4注=8倍奖金
    5注=12倍奖金

*/

/*彩票信息定义*/
typedef struct Lotto {
    char timeid[7];            // 发布期号，五位数，例如10025/
    char lid[10];             // 彩票唯一标识编号，timeid+001自增,例如10025099，每期发布99张彩票=/
    int pet[7];             // 投注号码，用户买的号码，范围为01-99
    int bets;              // 投注次数，最高五次

    char type;           // 彩票类型     //共三种型号，分别为A、B每种型号中奖规则不同，价格和奖金倍率不同=
    double price;          // 彩票单价*
    int num[15];         // 中奖号码
    double wining;      // 中奖金额     0.00为未中奖
    char userId[10];         // 购彩用户
    int status;         // 彩票状态     0: 未开奖 1: 开奖未中奖 2: 已中奖 3: 已兑奖

    double total;         // 奖金总额
    int sale;            // 彩票销量
    double volm;        //销售额
    struct Lotto* next; // 下一张彩票
} Lotto;

/*彩民信息定义*/
typedef struct User {
    char id[10];             // 用户唯一标识编号
    char name[20];      // 姓名
    char password[50];  // 用户密码
    double balance;     // 用户余额     存取款功能
    int status;         // 用户状态     0: 正常 1: 锁定

    struct Lotto* ownedLottos; // 用户拥有的彩票
    char purchaseHistory[10][70]; // 购彩记录
    /*可以存储10条购彩记录，每条记录最多包含70个字符。*/
    char winrecord[10][70]; // 中奖记录
    /*可以存储10条中奖记录，每条记录最多包含70个字符。"*/

    struct User* next;  // 下一个用户
} User;

typedef struct UserBlock {
    char id[10];             // 用户唯一标识编号
    char purchaseHistory[10][70]; // 购彩记录
    char winrecord[10][70]; // 中奖记录
    char type;           // 彩票类型
} UserBlock;


extern User admin;
extern User notary;

/*测试函数*/
void test();
/*功能测试*/
void function_test();
/*测试超时案例*/
void timeddl();

/*自适应输出文本框架*/
void print_box(char* title, char* content);
extern char title[100];
extern char content[5000];


/*清屏函数*/
void cls();
/*间歇函数*/
void pause();
/*断时函数*/
void crosstime(int seconds);
/*隐藏密码*/
void stealth_password(char* password);
/*主页*/
int home();
/*清空缓存*/
void flush();
/*待机测试*/
void standby_mode();
/*超时待机测试*/
void timeddl();

/*ASCII Artwork*/
void ASCIIArtLux();
void LotterySysArtword();
void GoodbeyArtword();
void rubbit(char* id, char* password);
#endif // !LOTTO_H