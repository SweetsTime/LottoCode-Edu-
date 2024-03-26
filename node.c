#include<stdio.h>
#include"node.h"
#include<string.h>
#include "general.h"
int flag = 0;
void login() {		//登录
	char account[100];
	char password[100];
	char admin[100] = "admin";	//管理员帐号及密码
	char notary[100] = "notary";	//公证员帐号及密码
	int cnt = 0;
	int choice;

	do {
		cls();
		sprintf(title, "用户登录");
		sprintf(content, "\n");
		sprintf(content, "%s  登录             [1]\n", content);
		sprintf(content, "%s  退出             [2]\n", content);
		sprintf(content, "%s当前系统日期: %s", content, __DATE__);
		print_box(title, content);
		printf("请输入选择：");
		scanf("%d", &choice);
		while (getchar() != '\n');

		switch (choice)
		{
		case 1:
			printf("\n请输入您的账号：");
			scanf("%s", account);
			printf("\n请输入用户密码：");
			stealth_password(password);

			int userLog = userLogin(account, password);
			if (strcmp(account, admin) == 0 && strcmp(password, admin) == 0) {	//管理员界面	
				int choose = adminMenu();
				if (choose == 0) {
					return;
				}
			}
			else if (strcmp(account, notary) == 0 && strcmp(password, notary) == 0) {//公证员界面	
				int choose = notaryMenu();
				if (choose == 0) {
					return;
				}
			}
			else if (userLog != 0) {	//彩民界面
				int choose = userMenu(account);
				if (choose == 0) {
					return;
				}
			}
			else {
				cnt++;
				if (cnt < 3) {
					printf("\n\t第%d次帐号或密码输入错误，请重新输入！\n", cnt);
				}
				else {
					printf("\n\t第%d次帐号或密码输入错误，程序退出！\n", cnt);
				}
			}
		case 2:
			printf("\n正在退出...\n");
			crosstime(500);
			return;
		default:
			printf("无效的选择，请重新输入。\n");
			pause();
			break;
		}
	} while (cnt < 3);
}

int adminMenu() {		//管理员操作界面
	cls();

	int choose;
	do {
		sprintf(title, "管理员主页");
		sprintf(content, "\n");
		sprintf(content, "%s  彩票发行    [1]\n", content);
		sprintf(content, "%s  奖池加额    [2]\n", content);
		sprintf(content, "%s  彩票查询    [3]\n", content);
		sprintf(content, "%s  全员查询    [4]\n", content);
		sprintf(content, "%s  单户查询    [5]\n", content);
		sprintf(content, "%s  用户排序    [6]\n", content);
		sprintf(content, "%s  数据保存    [7]\n", content);
		sprintf(content, "%s  账号退出    [0]\n", content);
		print_box(title, content);
		printf("请输入选择：");
		scanf("%d", &choose);
		while (getchar() != '\n');

		switch (choose) {
		case 1:
			adminPublishLottery();
			flag = 1;
			break;
		case 2:
			addPrize();
			flag = 1;
			break;
		case 3:
			displayadminPublishLottery();
			break;
		case 4:
			displayAllUserInfo();
			break;
		case 5:
			findUserMean();
			break;
		case 6:
			sortUserMean();
			break;
		case 7:
			SaveAllData();
			flag = 0;
			cls();
			break;
		default:
			if (0 > choose || 7 < choose) {
				printf("\n\t\t输入错误，请重新输入！\n");
				continue;
			}
		}
	} while (choose != 0);
	if (flag == 1) {
		printf("\n\t是否保存数据(Y/N)？");
		char ch;
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y') {
			flush();
			SaveAllData();
			flag = 0;
			return 0;
		}
	}
}
int notaryMenu() {		//公证员操作界面
	int choose;
	cls();
	do {
		sprintf(title, "公证员主页");
		sprintf(content, "\n");
		sprintf(content, "%s  随机开奖    [1]\n", content);
		sprintf(content, "%s  指定开奖    [2]\n", content);
		sprintf(content, "%s  彩票查询    [3]\n", content);
		sprintf(content, "%s  中奖查询    [4]\n", content);
		sprintf(content, "%s  数据保存    [5]\n", content);
		sprintf(content, "%s  账号退出    [0]\n", content);
		print_box(title, content);
		printf("请输入选择：");
		scanf("%d", &choose);
		flush();

		switch (choose) {
		case 1:
			doubleBull();
			flag = 1;
			cls();
			break;
		case 2:
			SpecifyTheDrawNumber();
			flag = 1;
			cls();
			break;
		case 3:
			displayadminPublishLottery();
			cls();
			break;
		case 4:
			findLotterInfo();
			cls();
			break;
		case 5:
			SaveAllData();
			flag = 0;
			cls();
			break;
		default:
			if (0 > choose || 5 < choose) {
				printf("\n\t\t输入错误，请重新输入！\n");
				continue;;
			}

		}
	} while (choose != 0);
	if (flag == 1) {
		printf("\n\t检测到数据修改，是否保存(Y/N)？");
		char ch;
		getchar();
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y') {
			flush();
			SaveAllData();
			flag = 0;
			return 0;
		}
	}
}
int userMenu(char account[100]) {		//用户操作界面
	cls();
	int choose;
	do {
	sprintf(title, "用户主页");
	sprintf(content, "\n");
	sprintf(content, "%s  个人信息    [1]\n", content);
	sprintf(content, "%s  购彩数据    [2]\n", content);
	sprintf(content, "%s  账号充值    [3]\n", content);
	sprintf(content, "%s  购买彩票    [4]\n", content);
	sprintf(content, "%s  修改密码    [5]\n", content);
	sprintf(content, "%s  注销账户    [6]\n", content);
	sprintf(content, "%s  账号退出    [0]\n", content);
	print_box(title, content);
	printf("请输入选择：");
	scanf("%d", &choose);
	while (getchar() != '\n');

		switch (choose) {
		case 1:
			displayMyInfo(account);
			cls();
			break;
		case 2:
			displayLotteryPurchaseHistory(account);
			cls();
			break;
		case 3:
			userSurplusRecharge(account);
			cls();
			flag = 1;
			break;
		case 4:
			userBuyLotterMean(account);
			cls();
			break;
		case 5:
			userModifyPassword(account);
			cls();
			flag = 1;
			break;
		case 6:
			printf("\n\t账户注销后，将无法恢复，请您再次确认是否注销%s该帐号(Y/N)？", account);
			char choose;
			getchar();
			scanf("%c", &choose);
			if (choose == 'Y' || choose == 'y') {
				deleteUserInfo(account);
				cls();
				return 0;
			}
			else {
				printf("\n\t已取消注销该帐号\n");
				cls();
				break;
			}
		default:
			if (0 > choose || 6 < choose) {
				printf("\n\t\t输入错误，请重新输入！\n");
				continue;;
			}
		}
	} while (choose != 0);
	if (flag == 1) {
		printf("\n\t检测到数据修改，是否保存(Y/N)？");
		char ch;
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y') {
			flush();
			SaveAllData();
			flag = 0;
			return 0;
		}
	}
}

void findUserMean() {
	int choose;
	cls();
	do {
		printf("\n\t1.按用户帐号查询\n");
		printf("\t2.按余额区间查询\n");
		printf("\t请选择(输入0返回上一页)：");
		scanf("%d", &choose);
		while (getchar() != '\n');
		switch (choose) {
		case 1:
			findUserInfoByAccount();
			break;
		case 2:
			userBalanceFindUserInfo();
			break;
		default:
			if (0 > choose || 2 < choose) {
				printf("\n输入错误，请重新输入！\n");
				continue;
			}
		}
	} while (choose != 0);
	cls();
}
void sortUserMean() {
	int choose;
	cls();
	do {
		printf("\n\t\t1.按帐号排序彩民信息\n");
		printf("\t\t2.按余额排序彩民信息\n");
		printf("\t\t请选择(输入0返回上一页)：");
		scanf("%d", &choose);
		flush();
		switch (choose) {
		case 1:
			AccountSortingUserInfo();
			displayAllUserInfo();
			break;
		case 2:
			SurplusSortingUserInfo();
			displayAllUserInfo();
			break;
		default:
			if (0 > choose || 2 < choose) {
				printf("\n\t\t输入错误，请重新输入！\n");
				continue;
			}
		}
	} while (choose != 0);
	cls();
}

void userBuyLotterMean(char account[100]) {
	int choose;
	do {
		printf("\n\t请选择购彩方式：\n");
		printf("\n\t1.随机选彩\n");
		printf("\t2.用户自选\n");
		printf("\t3.批量帮选\n");
		printf("\t请选择(输入0返回上一页)：");
		scanf("%d", &choose);
		switch (choose) {
		case 1:
			AIchoose(account);
			flag = 1;
			break;
		case 2:
			userBuyLottery(account);
			flag = 1;
			break;
		case 3:
			batchAIchoose(account);
			flag = 1;
			break;
		default:
			if (0 > choose || 3 < choose) {
				printf("\n\t\t输入错误，请重新输入！\n");
				continue;
			}
		}
	} while (choose != 0);
}

void SaveAllData() {
	saveAdmin();
	saveUserInfoNode();
	saveUserBuy();
	printf("\n\t数据保存成功！\n");
	pause();
}

void ReadAllData() {
	readAdmin();
	readUserInfoNode();
	readUserBuy();
}
