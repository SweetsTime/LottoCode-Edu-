#pragma once
#ifndef ADMIN_H
#define ADMIN_H
#include"lottery.h"

struct publish {	//管理员发布彩票信息
	int number;			//彩票期号
	int price;			//彩票单价
	int state;			//开奖状态（1表示已开奖，0表示未开奖）
	int issueNumber[7];		//本期开奖号
	int total;			//本期售出总数
	float  prize;			//本期奖池金额
};
typedef struct publish Publish;

struct admin {	//管理员发布
	Publish* data;
	struct admin* next;
};
typedef struct admin Admin;

void saveAdmin();
void readAdmin();
void addAdminNode(Publish* publish);
void adminPublishLottery();
void displayadminPublishLottery();
int userCanBuyLottery();
void ModifyLotteryState(int number);
int lotterySoldTotal(int number);
void addLotterPrize(int number, int userSpend);
int findLotterPrize(int number);
void reduceLotterPrize(int number, int userSpend);
void PrizePoolUpdates(int number, float userWin);
float PreviousPrize(Admin* newNode);
void UpdateDrawNumberAfterTheDraw(int number, int issueNumber[7]);
#endif
