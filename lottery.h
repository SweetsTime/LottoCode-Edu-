#ifndef LOTTERY_H
#define LOTTERY_H   
#include"admin.h"
struct lottery {
	int number;		//彩票期号
	long uid;		//彩票UID
	int drawNumber[7];	//彩票开奖号,6+1玩法
	int bets;		//购买注数
	char account[100];	//购买彩民帐号
	int winStatus;		//中奖状态(1为中奖，0为未中奖)
	float winAmount;		//中奖金额
};
typedef struct lottery Lottery;

struct userBuy {
	Lottery* data;
	struct userBuy* next;
};
typedef struct userBuy UserBuy;


void saveUserBuy();
void readUserBuy();
void addUserBuyNode(Lottery* data);
void userBuyLottery(char account[100]);
int doubleBull();
int findLotteryBets(int bets);
int userSurplusDecide(char account[100]);
void ModifyUserSurplus(char account[100], int spend);
void userSurplusRecharge(char account[100]);
void addUserBuyLotterToLink(char account[100], int number, long uid, int drawNumber[7], int bets);
void UpdateUserBalancesAfterDraw(int number, int drawNum[7]);
void addUserSurplus(char account[100], float amount);
void userSurplusAdd(char account[100], int surplus);
void displayLotteryPurchaseHistory(char account[100]);
void findUserInfo();
void addUserCumulativeWinnAmount(char account[100], float amount);
void AIchoose(char account[100]);
void batchAIchoose(char account[100]);
#endif  
