#pragma once
#ifndef NODE_H
#define NODE_H
#include<stdio.h>
#include<stdlib.h>
#include"lottery.h"
#include"user.h"
void login();
int adminMenu();
int notaryMenu();
int userMenu(char account[100]);
int userLogin(char account[100], char password[100]);
void adminPublishLottery();
void displayadminPublishLottery();
int userCanBuyLottery();
void displayMyInfo(char account[100]);
void userModifyPassword(char account[100]);
int findLotteryBets(int bets);
void userSurplusRecharge(char account[100]);
void displayLotteryPurchaseHistory(char account[100]);
void findUserInfo();
void deleteUserInfo(char account[100]);
void userBalanceFindUserInfo();
void AccountSortingUserInfo();
void SurplusSortingUserInfo();
void displayAllUserInfo();
void findUserMean();
void sortUserMean();
void findLotterInfo();
void SaveAllData();
void ReadAllData();
void userBuyLotterMean(char account[100]);
void SpecifyTheDrawNumber();
void addPrize();
#endif
