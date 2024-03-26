#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"user.h"
#include<string.h>
#include "general.h"
#include"lottery.h"
#include <ctype.h>
#include <fcntl.h>

UserInfoNode* head = NULL;
UserInfoNode* current = NULL;
extern UserBuy* UserBuyhead;

/*void saveUserInfoNode() {
	FILE* fp = fopen("user.bat", "w");
	UserInfoNode* temp = head;
	while (temp != NULL) {
		fwrite(temp->data, sizeof(UserInfo), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
}*/

/*void readUserInfoNode() {
	int len;
	FILE* fp = fopen("user.bat", "r");
	if (fp == NULL) {
		printf("\t载入用户数据失败！\n");
		return;
	}
	UserInfo* user = malloc(sizeof(UserInfo));
	len = fread(user, sizeof(UserInfo), 1, fp);
	while (len > 0) {
		addUserInfoNode(user);
		user = malloc(sizeof(UserInfo));
		len = fread(user, sizeof(UserInfo), 1, fp);
	}
	fclose(fp);
}*/
void saveUserInfoNode() {
	FILE* fp = fopen("user.txt", "w");
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* user = temp->data;
		fprintf(fp, "%s %s %s %d %f %f\n", user->userAccount, user->userPassword, user->name, user->status, user->userSurplus, user->CumulativeWinnAmount);
		temp = temp->next;
	}
	fclose(fp);
}

void readUserInfoNode() {
	FILE* fp = fopen("user.txt", "r");
	if (fp == NULL) {
		printf("\t载入用户数据失败！\n");
		return;
	}
	UserInfo* user = malloc(sizeof(UserInfo));
	while (fscanf(fp, "%s %s %s %d %f %f", user->userAccount, user->userPassword, user->name, &user->status, &user->userSurplus, &user->CumulativeWinnAmount) == 6) {
		addUserInfoNode(user);
		user = malloc(sizeof(UserInfo));
	}
	free(user); // 释放最后分配的未使用的内存
	fclose(fp);
}


void addUserInfoNode(UserInfo* data) {
	UserInfoNode* newNode = malloc(sizeof(UserInfoNode));
	newNode->data = data;
	newNode->next = NULL;
	if (head == NULL) {
		head = newNode;
		current = newNode;
		return;
	}
	current->next = newNode;
	current = newNode;

}

// 将字符转换为数字
int char_to_num(char c) {
	if ('A' <= c && c <= 'Z') {
		return c - 'A' + 1;
	}
	else if ('a' <= c && c <= 'z') {
		return c - 'a' + 27;
	}
	else {
		return 0;
	}
}

UserInfo* find_idn3(UserInfoNode* head, char* id_prefix) {
	UserInfoNode* current = head;
	UserInfo* last_same_prefix_user = NULL;
	while (current != NULL) {
		if (strncmp(current->data->userAccount, id_prefix, 3) == 0) {
			if (last_same_prefix_user == NULL || atoi(current->data->userAccount + 3) > atoi(last_same_prefix_user->userAccount + 3)) {
				last_same_prefix_user = current->data;
			}
		}
		current = current->next;
	}
	return last_same_prefix_user;
}

void addNewUser() {	//彩民注册帐号
	char name[100];
	char userAccount[100];
	char userPassword1[100];
	char userPassword2[200];
	char admin[100] = "admin";
	char notary[100] = "notary";
	do {
		printf("\n请输入您的姓名：");
		scanf("%s", name);
		while (getchar() != '\n');
		for (int i = 0; name[i]; i++) {
			if (!isalpha(name[i])) {
				printf("无效姓名。姓名仅支持英文。\n");
				pause();
				return;
			}
		}
		UserInfo* newUser = (UserInfo*)malloc(sizeof(UserInfo));

		// 使用参数设置用户信息
		strcpy(newUser->name, name);
		// 生成账号
		int sum = 0;
		for (int i = 0; i < strlen(name); i++) {
			sum += char_to_num(name[i]);
		}

		char id_prefix[4];
		sprintf(id_prefix, "%03d", sum);
		UserInfo* user = find_idn3(head, id_prefix);
		if (user == NULL) {
			// 新用户
			sprintf(newUser->userAccount, "%03d001", sum);
		}
		else {
			// 同名用户
			int id = atoi(user->userAccount + 3);
			id++;
			sprintf(newUser->userAccount, "%03d%03d", sum, id);
		}

		// 检查账号是否已被注册
		int userExists = findUser(newUser->userAccount);
		if (userExists == 1) {
			printf("\n\t%s该帐号已被注册，请重新输入！\n", newUser->userAccount);
			break;
		}
		printf("请输入用户密码：");
		scanf("%s", userPassword1);

		for (int i = 0; userPassword1[i]; i++) {
			if (!isalnum(userPassword1[i]) && !ispunct(userPassword1[i])) {
				printf("无效密码。密码仅支持数字、字母和标点符号。\n");
				pause();
				return;
			}
		}

		printf("请确认您的密码：");
		scanf("%s", userPassword2);
		if (strcmp(newUser->userAccount, admin) != 0 && strcmp(newUser->userAccount, notary) != 0) {
			if (strcmp(userPassword1, userPassword2) == 0) {
				strcpy(newUser->userPassword, userPassword1);
				strcpy(newUser->name, name);
				newUser->userSurplus = 0;
				newUser->status = 0;
				addNewUserToNode(newUser);
				newUser->CumulativeWinnAmount = 0;
				while (getchar() != '\n');
				rubbit(newUser->userAccount, newUser->userPassword);
				printf("\n\t帐号%s注册成功，请牢记您的密码！\n", newUser->userAccount);
				pause();
			}
			else {
				printf("\n\t两次输入密码不一致，请重新输入！\n");
			}
		}
		else {
			printf("\n\t该帐号禁止注册！\n");
		}
	} while (strcmp(userPassword1, userPassword2) != 0);
}

void addNewUserToNode(UserInfo* data) {	//添加彩民注册帐号至链表中
	UserInfoNode* newNode = malloc(sizeof(UserInfoNode));
	newNode->data = data;
	newNode->next = NULL;
	if (head == NULL) {
		head = newNode;
		current = newNode;
		return;
	}
	current->next = newNode;
	current = newNode;

}

int userLogin(char account[100], char password[100]) {	//判断用户输入帐号及密码与注册帐号密码是否一致
	UserInfoNode* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->data->userAccount, account) == 0 && strcmp(temp->data->userPassword, password) == 0) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int findUser(char userAccount[100]) {
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, userAccount) == 0) {
			return 1;		//该帐号已注册
		}
		temp = temp->next;
	}
	return 0;				//该帐号未注册
}
void displayMyInfo(char account[100]) {		//显示用户个人信息
	UserInfoNode* temp = head;
	UserBuy* temp2 = UserBuyhead;
	cls();
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			sprintf(title, "用户数据库");
			sprintf(content, "\n");
			sprintf(content, "%s用户账号：%s\n", content, userInfoData->userAccount);
			sprintf(content, "%s用户姓名：%s\n", content, userInfoData->name);
			sprintf(content, "%s用户密码：%s\n", content, userInfoData->userPassword);
			sprintf(content, "%s用户状态：%s\n", content, userInfoData->status == 0 ? "正常" : "冻结");
			sprintf(content, "%s用户余额：%.2f\n", content, userInfoData->userSurplus);
			sprintf(content, "%s累计奖金：%.2f\n", content, userInfoData->CumulativeWinnAmount);
			sprintf(content, "%s账号状态：%s\n", content, "正常");
			sprintf(content, "%s----------------------------------------------------------------------------------------------------\n", content);
			sprintf(content, "%s--------------------------------------------用户持有彩票--------------------------------------------\n", content);
			sprintf(content, "%s----------------------------------------------------------------------------------------------------\n", content);
			sprintf(content, "%s彩票期号  彩票ID      单注金额  购买注数  购买金额  彩票状态     用户购买号码           中奖金额\n", content);
			while (temp2 != NULL) {
				if (strcmp(temp2->data->account, account) == 0) {
					int lotterBets = findLotteryBets(temp2->data->number);
					int spend = lotterBets * temp2->data->bets;
					sprintf(content, "%s%-10d%-12d%-10d%-10d%-10d%-14s%02d-%02d-%02d-%02d-%02d-%02d-- -%02d%-2s%.2f\n",
						content,
						temp2->data->number,
						temp2->data->uid,
						lotterBets,
						temp2->data->bets,
						spend,
						temp2->data->winStatus == 0 ? "未中奖" : "已中奖",
						temp2->data->drawNumber[0], temp2->data->drawNumber[1], temp2->data->drawNumber[2], temp2->data->drawNumber[3], temp2->data->drawNumber[4], temp2->data->drawNumber[5], temp2->data->drawNumber[6],
						" ",
						temp2->data->winAmount);
				}
				temp2 = temp2->next;
			}

			print_box(title, content);
			printf("\n");
			pause();
			return;
		}
		temp = temp->next;
	}
}

void userModifyPassword(char account[100]) {	//用户修改密码
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			char oldPassword[100];
			printf("\n\t请输入原密码：");
			scanf("%s", oldPassword);
			if (strcmp(oldPassword, userInfoData->userPassword) == 0) {
				char newpassword1[100];
				char newpassword2[100];
				printf("\n\t请输入新密码：");
				scanf("%s", newpassword1);
				printf("\n\t请确认新密码：");
				scanf("%s", newpassword2);
				if (strcmp(newpassword1, newpassword2) != 0) {
					printf("\n\t两次密码输入不一致，请重试！\n");
					flush();
					pause();
					return;
				}
				strcpy(userInfoData->userPassword, newpassword1);
				printf("\n\t新密码修改成功，请牢记您的密码！\n");
				flush();
				pause();
				return;
			}
			else {
				printf("\n\t旧密码输入错误，请重试！\n");
			}
		}
		temp = temp->next;
	}
}
void userSurplusRecharge(char account[100]) {		//用户账户充值
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			int surplus;
			printf("\n\t请输入充值金额：");
			scanf("%d", &surplus);
			userInfoData->userSurplus += surplus;
			printf("\n\t恭喜您，账户充值成功!\n");
			//saveUserInfoNode();
			//saveUserBuy();
			flush();
			pause();
			return;
		}
		temp = temp->next;
	}
}
int userSurplusDecide(char account[100]) {		//查询用户账户余额
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			return 	userInfoData->userSurplus;
		}
		temp = temp->next;
	}
}
void ModifyUserSurplus(char account[100], int spend) {	//用户购买彩票后减少余额
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			userInfoData->userSurplus -= spend;
			return;
		}
		temp = temp->next;
	}
}

void userSurplusAdd(char account[100], int surplus) {		//中奖后增加用户余额
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			userInfoData->userSurplus += surplus;
			return;
		}
		temp = temp->next;
	}
}
void addUserCumulativeWinnAmount(char account[100], float amount) {
	UserInfoNode* temp = head;
	while (temp != NULL) {
		UserInfo* userInfoData = malloc(sizeof(UserInfo));
		userInfoData = temp->data;
		if (strcmp(userInfoData->userAccount, account) == 0) {
			userInfoData->CumulativeWinnAmount += amount;
			return;
		}
		temp = temp->next;
	}


}
void findUserInfoByAccount() {			//管理员查看某一用户信息
	char account[100];
	printf("\n\t请输入账户帐号：");
	scanf("%s", account);
	displayMyInfo(account);
	//displayLotteryPurchaseHistory(account);
}

void deleteUserInfo(char account[100]) {		//用户注销帐号	
	UserInfoNode* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->data->userAccount, account) == 0) {
			UserInfoNode* preTemp = malloc(sizeof(UserInfoNode));
			preTemp = findPreUserInfoNode(temp);
			preTemp->next = temp->next;
			printf("\n\t%s用户您好，您的帐号已注销，非常感谢您的使用，期待与您再次相遇！^v^\n", account);
			return;
		}
		temp = temp->next;
	}
}

UserInfoNode* findPreUserInfoNode(UserInfoNode* userInfo) {	//查找链表中某一用户前一个用户
	UserInfoNode* temp = head;
	if (userInfo == head) {
		return NULL;
	}
	while (temp != NULL) {
		if (temp->next != NULL) {
			if (temp->next == userInfo) {
				return temp;
			}
		}
		temp = temp->next;
	}
}

void userBalanceFindUserInfo() {			//按余额查询彩民信息
	float min, max;
	cls();
	printf("\n\t请输入查询余额最低值：");
	scanf("%f", &min);
	printf("\t请输入查询余额最高值：");
	scanf("%f", &max);
	cls();
	sprintf(title, "用户数据库");
	sprintf(content, "\n");
	sprintf(content, "用户帐号  用户密码  账号余额  累计奖金\n");
	UserInfoNode* temp = head;
	while (temp != NULL) {
		if (temp->data->userSurplus >= min && temp->data->userSurplus <= max) {
			sprintf(content, "%s%-10s%-10s%8.2f%8.2f\n",
				content,
				temp->data->userAccount, 
				temp->data->userPassword, 
				temp->data->userSurplus, 
				temp->data->CumulativeWinnAmount);
		}
		temp = temp->next;
	}
	print_box(title, content);
}

void AccountSortingUserInfo() {		//按帐号排序彩民信息
	if (head == NULL || head->next == NULL) {
		return;
	}
	for (UserInfoNode* t = head->next; t != NULL; t = t->next) {
		for (UserInfoNode* temp = head; temp != t; temp = temp->next) {
			if (strcmp(t->data->userAccount, temp->data->userAccount) < 0) {
				UserInfoNode* prevT = findPreUserInfoNode(t);
				prevT->next = t->next;
				if (temp == head) {
					t->next = head;
					head = t;
					break;
				}
				else {
					UserInfoNode* prevTemp = findPreUserInfoNode(temp);
					prevTemp->next = t;
					t->next = temp;
					break;
				}
			}
		}
	}
	current = head;
	while (current->next != NULL) {
		current = current->next;
	}
}

void SurplusSortingUserInfo() {	//按余额排序彩民信息
	if (head == NULL || head->next == NULL) {
		return;
	}
	for (UserInfoNode* t = head->next; t != NULL; t = t->next) {
		for (UserInfoNode* temp = head; temp != t; temp = temp->next) {
			if (t->data->userSurplus < temp->data->userSurplus) {
				UserInfoNode* prevT = findPreUserInfoNode(t);
				prevT->next = t->next;
				if (temp == head) {
					t->next = head;
					head = t;
					break;
				}
				else {
					UserInfoNode* prevTemp = findPreUserInfoNode(temp);
					prevTemp->next = t;
					t->next = temp;
					break;
				}
			}
		}
	}
	current = head;
	while (current->next != NULL) {
		current = current->next;
	}
}

void displayAllUserInfo() {		//打印所有用户信息
	UserInfoNode* temp = head;
	cls();
	sprintf(title, "用户数据库");
	sprintf(content, "\n");
	sprintf(content, "帐号      姓名      密码      状态      余额\n");
	while (temp != NULL) {

		sprintf(content, "%s%-10s%-10s%-10s%-12s%-8.2f\n", content,temp->data->userAccount, temp->data->name, temp->data->userPassword, temp->data->status == 0 ? "正常" : "冻结", temp->data->userSurplus );/*, temp->data->CumulativeWinnAmount */
		temp = temp->next;
	}
	print_box(title, content);
	pause();
	cls();
}
