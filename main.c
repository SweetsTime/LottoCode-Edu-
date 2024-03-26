/*
    文件：主函数
    说明：
        1.程序main函数
*/

#include "general.h"
#include "node.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>

char title[100];
char content[5000];
extern int flag;

int main() {
    cls();
    LotterySysArtword();      //动画
    cls();
    test();     //测试函数

	ReadAllData();
	int choose;
	do {
		choose = home();		//首页
		switch (choose) {
		case 1:
			login();	//登录界面
			break;
		case 2:
			addNewUser();	//注册
			SaveAllData();
			break;
		case 3:
			break;
		case 4:
			//function_test();
			//standby_mode();
			timeddl();
			pause();
			break;
		case 5:
			printf("谢谢使用，再见！\n");
			//free(users);
			GoodbeyArtword();
			exit(0);
		default:
			printf("无效的选择，请重新输入。\n");
			pause();
			break;
			/*
			printf("\t是否保存(Y/N)？");
			char ch;
			getchar();
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y') {
				SaveAllData();
				return 0;
			}
			else {
				return 0;
			}*/

		}
	} while (choose >= 1 || choose <= 5);

		/*head = loadUsers(head);
	Lotto* lottoHead = readAllLotto();
	system_page();
	// TODO: 在这里进行彩票管理系统的其他操作*/

	return 0;
}



