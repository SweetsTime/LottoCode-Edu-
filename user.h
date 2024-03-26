#ifndef USER_H
#define USER_H
#include"lottery.h"
#include<stdio.h>
#include<stdlib.h>

struct userInfo {
	char userAccount[100];		//彩民帐号
	char userPassword[100];		//彩民密码
	char name[100];				//彩民姓名
	int status;         // 用户状态     0: 正常 1: 锁定
	float userSurplus;		//彩民余额
	float CumulativeWinnAmount;	//累计中奖金额
};
typedef struct userInfo UserInfo;

struct userInfoNode {
	UserInfo* data;
	struct userInfoNode* next;
};
typedef struct userInfoNode UserInfoNode;

void saveUserInfoNode();
void readUserInfoNode();
void addUserInfoNode(UserInfo* data);
void addNewUserToNode(UserInfo* data);
void addNewUser();
int findUser(char userAccount[100]);
void userPasswordModify();
void displayMyselfInfo();
int userSurplusDecide(char account[100]);
void ModifyUserSurplus(char account[100], int spend);
void userSurplusAdd(char account[100], int surplus);
void findUserInfoByAccount();
void deleteUserInfo(char account[100]);
UserInfoNode* findPreUserInfoNode(UserInfoNode* temp);
void addUserCumulativeWinnAmount(char account[100], float amount);
#endif
