#include<stdio.h>
#include"lottery.h"
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include "general.h"

UserBuy* UserBuyhead = NULL;
UserBuy* UserBuycurrent = NULL;

void saveUserBuy() {
	FILE* fp = fopen("lottery.bat", "w");
	UserBuy* temp = UserBuyhead;
	while (temp != NULL) {
		fwrite(temp->data, sizeof(Lottery), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
}
/*void saveUserBuy() {
	FILE* fp = fopen("lottery.txt", "w");
	UserBuy* temp = UserBuyhead;
	while (temp != NULL) {
		Lottery* lottery = temp->data;
		fprintf(fp, "%d %ld ", lottery->number, lottery->uid);
		for (int i = 0; i < 7; i++) {
			fprintf(fp, "%d ", lottery->drawNumber[i]);
		}
		fprintf(fp, "%d %s %d %f\n", lottery->bets, lottery->account, lottery->winStatus, lottery->winAmount);
		temp = temp->next;
	}
	fclose(fp);
}*/

void readUserBuy() {
	int len;
	FILE* fp = fopen("lottery.bat", "r");
	if (fp == NULL) {
		printf("\t载入彩票数据失败！\n");
		return;
	}
	Lottery* lottery = malloc(sizeof(Lottery));
	len = fread(lottery, sizeof(Lottery), 1, fp);
	while (len > 0) {
		addUserBuyNode(lottery);
		lottery = malloc(sizeof(Lottery));
		len = fread(lottery, sizeof(Lottery), 1, fp);
	}
	fclose(fp);
}
/*void readUserBuy() {
	FILE* fp = fopen("lottery.txt", "r");
	if (fp == NULL) {
		printf("\t载入彩票数据失败！\n");
		return;
	}
	Lottery* lottery = malloc(sizeof(Lottery));
	while (fscanf(fp, "%d %ld", &lottery->number, &lottery->uid) == 2) {
		for (int i = 0; i < 7; i++) {
			fscanf(fp, "%d", &lottery->drawNumber[i]);
		}
		fscanf(fp, "%d %s %d %f", &lottery->bets, lottery->account, &lottery->winStatus, &lottery->winAmount);
		addUserBuyNode(lottery);
		lottery = malloc(sizeof(Lottery));
	}
	free(lottery); // Free the last allocated memory which is not used
	fclose(fp);
}*/

void addUserBuyNode(Lottery* data) {
	UserBuy* newNode = malloc(sizeof(UserBuy));
	newNode->data = data;
	newNode->next = NULL;
	if (UserBuyhead == NULL) {
		UserBuyhead = newNode;
		UserBuycurrent = newNode;
		return;
	}
	UserBuycurrent->next = newNode;
	UserBuycurrent = newNode;

}

int doubleBull() {	//开奖号生成
	int canBuy = userCanBuyLottery();
	if (canBuy != 1) {
		srand(time(NULL));
		int redBull[6] = { 0 };
		int blueBull = rand() % 16 + 1;
		int i = 0, j, num;
		while (i < 6) {
			num = rand() % 33 + 1;
			for (j = i - 1; j >= 0; j--) {
				if (num == redBull[j]) {
					break;
				}
			}
			if (j < 0) {
				redBull[i] = num;
				i++;
			}
		}
		int drawNum[7] = { redBull[0],redBull[1],redBull[2],redBull[3],redBull[4],redBull[5],blueBull };
		printf("\n\t本期开奖号：");
		printf("%02d-%02d-%02d-%02d-%02d-%02d---%02d", redBull[0], redBull[1], redBull[2], redBull[3], redBull[4], redBull[5], blueBull);
		flush();
		pause();
		UpdateUserBalancesAfterDraw(canBuy, drawNum);	//开奖后更新用户余额
		ModifyLotteryState(canBuy);			//修改彩票开奖状态为1
		UpdateDrawNumberAfterTheDraw(canBuy, drawNum);	//开奖后更新开奖号
	}
	else {
		printf("\n\t当前无需开奖彩票期号！\n");
		pause();
	}
}
void SpecifyTheDrawNumber() {
	int canBuy = userCanBuyLottery();
	if (canBuy != 1) {
		int red[6];
		int blue;
		for (int i = 0; i < 6; i++) {
			printf("\t请输入第%d个红球号码（输入1～33范围内不重复的号码）：", i + 1);
			scanf("%d", &red[i]);
			flush();
			if (red[i] < 1 || red[i]>33) {
				printf("\n\t%d不在红球号码范围内，请重新输入！\n", red[i]);
				pause();
				return;
			}
		}
		printf("\t请输入蓝球号码（输入1～16范围的号码）：");
		scanf("%d", &blue);
		if (blue < 1 || blue>16) {
			printf("\n\t%d不在蓝球号码范围内，请重新输入！\n", blue);
			pause();
			return;
		}
		int drawNum[7] = { red[0],red[1],red[2],red[3],red[4],red[5],blue };
		printf("\n\t本期开奖号：");
		printf("%02d-%02d-%02d-%02d-%02d-%02d---%02d\n", red[0], red[1], red[2], red[3], red[4], red[5], blue);
		flush();
		pause();
		UpdateUserBalancesAfterDraw(canBuy, drawNum);	//开奖后更新用户余额
		ModifyLotteryState(canBuy);			//修改彩票开奖状态为1
		UpdateDrawNumberAfterTheDraw(canBuy, drawNum);	//开奖后更新开奖号
	}
	else {
		printf("\n\t当前无需开奖彩票期号！\n");
		pause();
	}
}
void AIchoose(char account[100]) {
	int canBuy = userCanBuyLottery();
	if (canBuy != 1) {
		int red[6];
		int blue;
		printf("\n\t当前可购买期号：%d\n", canBuy);
		int lotterBets = findLotteryBets(canBuy);
		printf("\t当前期号单注金额：%d\n", lotterBets);
		printf("\n\t请确认是否使用机选号码(Y/N)？");
		char choose;
		getchar();
		scanf("%c", &choose);
		if (choose == 'y' || choose == 'Y') {
			int redBull[6] = { 0 };
			int blueBull = rand() % 16 + 1;
			int i = 0, j, num;
			while (i < 6) {
				num = rand() % 33 + 1;
				for (j = i - 1; j >= 0; j--) {
					if (num == redBull[j]) {
						break;
					}
				}
				if (j < 0) {
					redBull[i] = num;
					i++;
				}
			}
			int drawNum[7] = { redBull[0],redBull[1],redBull[2],redBull[3],redBull[4],redBull[5],blueBull };
			printf("\n\t您的开奖号：");
			printf("%02d-%02d-%02d-%02d-%02d-%02d---%02d", drawNum[0], drawNum[1], drawNum[2], drawNum[3], drawNum[4], drawNum[5], drawNum[6]);
			int userBets;
			printf("\n\t请输入您的下注数量(不超过5注)：");
			scanf("%d", &userBets);
			if (userBets < 0 || userBets>5) {
				printf("\n\t下注数量输入错误，请重新输入！\n");
				return;
			}
			int userSurplus = userSurplusDecide(account);
			int userSpend = userBets * lotterBets;
			if (userSpend > userSurplus) {
				printf("\n\t账户余额不足，是否充值？(Y/N):");
				char choose;
				getchar();
				scanf("%c", &choose);
				if (choose == 'Y' || choose == 'y') {
					userSurplusRecharge(account);
				}
				else {
					printf("\n\t取消充值，返回上一页！\n");
					return;
				}
			}
			ModifyUserSurplus(account, userSpend);
			int total = lotterySoldTotal(canBuy);
			long CanBuy = canBuy;
			long UID = CanBuy * 10000 + total;
			addLotterPrize(canBuy, userSpend);
			addUserBuyLotterToLink(account, canBuy, UID, drawNum, userBets);

			cls();
			sprintf(title, "感谢购彩");
			sprintf(content, "\n");
			sprintf(content, "%s彩票购买成功！详细信息如下：\n", content);
			sprintf(content, "%s购买期号：%d\n", content, canBuy);
			sprintf(content, "%s单注金额：%d\n", content, lotterBets);
			sprintf(content, "%s开奖号：", content);
				for (int i = 0; i < 6; i++) {
					if (redBull[i] < 10) {
						sprintf(content, "%s%02d-", content, redBull[i]);
					}
					else {
						sprintf(content, "%s%d-", content, redBull[i]);
					}
				}
				if (blueBull < 10) {
					sprintf(content, "%s--%02d\n", content, blueBull);
				}
				else {
					sprintf(content, "%s--%d\n", content, blueBull);
				}
			sprintf(content, "%s购买注数：%d\n", content, userBets);
			sprintf(content, "%s购买金额：%d\n", content, userSpend);
			sprintf(content, "%s唯一UID：%ld\n", content, UID);
			print_box(title, content);
			pause();
			return;

			/*
			printf("\n\t彩票购买成功！详细信息如下：\n");
			printf("\n\t购买期号：%d\n", canBuy);
			printf("\t单注金额：%d\n", lotterBets);
			printf("\t开奖号：");
			for (int i = 0; i < 6; i++) {
				if (redBull[i] < 10) {
					printf("%02d-", redBull[i]);
				}
				else {
					printf("%d-", redBull[i]);
				}
			}
			if (blueBull < 10) {
				printf("--%02d\n", blueBull);
			}
			else {
				printf("--%d\n", blueBull);
			}
			printf("\t购买注数：%d\n", userBets);
			printf("\t购买金额：%d\n", userSpend);
			printf("\t唯一UID：%ld\n\n", UID);
			*/
		}
		else {
			return;
		}
	}
	else {
		printf("\n\t当前无可购买彩票期号！\n");
		pause();
		return;
	}
}

void batchAIchoose(char account[100]) {
	int canBuy = userCanBuyLottery();
	if (canBuy != 1) {
		int red[6];
		int blue;
		printf("\n\t当前可购买期号：%d\n", canBuy);
		int lotterBets = findLotteryBets(canBuy);
		printf("\t当前期号单注金额：%d\n", lotterBets);
		printf("\n\t请确认是否使用批量机选号码(Y/N)？");
		char choose;
		getchar();
		scanf("%c", &choose);
		if (choose == 'y' || choose == 'Y') {
			int val;
			printf("\n\t请输入批量购买数量(默认每张彩票购买5注)：");
			scanf("%d", &val);
			int userNowSurplus = userSurplusDecide(account);
			int userNowSpend = 5 * lotterBets * val;
			while (userNowSpend > userNowSurplus) {
				printf("\n\t账户余额不足，是否充值？(Y/N)\n");
				char choose2;
				getchar();
				scanf("%c", &choose2);
				if (choose2 == 'Y' || choose2 == 'y') {
					userSurplusRecharge(account);
					userNowSurplus = userSurplusDecide(account);
				}
				else {
					printf("\n\t取消充值，返回上一页！\n");
					return;
				}
			}

			for (int i = 0; i < val; i++) {
				int redBull[6] = { 0 };
				int blueBull = rand() % 16 + 1;
				int i = 0, j, num;
				while (i < 6) {
					num = rand() % 33 + 1;
					for (j = i - 1; j >= 0; j--) {
						if (num == redBull[j]) {
							break;
						}
					}
					if (j < 0) {
						redBull[i] = num;
						i++;
					}
				}
				int drawNum[7] = { redBull[0],redBull[1],redBull[2],redBull[3],redBull[4],redBull[5],blueBull };
				int userSpend = 5 * lotterBets;
				ModifyUserSurplus(account, userSpend);
				int total = lotterySoldTotal(canBuy);
				long CanBuy = canBuy;
				long UID = CanBuy * 10000 + total;
				addLotterPrize(canBuy, userSpend);
				addUserBuyLotterToLink(account, canBuy, UID, drawNum, 5);
			}
			cls();
			printf("\n\t恭喜您已成功购买%d期彩票%d张，每张彩票均为5注！\n", canBuy, val);
			printf("\t请到个人购买历史查看详细信息！\n");
		}
		else {
			return;
		}
	}
	else {
		printf("\n\t当前无可购买彩票期号！\n");
		return;
	}

}
void userBuyLottery(char account[100]) {		//用户购买彩票
	int canBuy = userCanBuyLottery();
	if (canBuy != 1) {
		int red[6];
		int blue;
		printf("\n\t当前可购买期号：%d\n", canBuy);
		int lotterBets = findLotteryBets(canBuy);
		printf("\t当前期号单注金额：%d\n", lotterBets);
		for (int i = 0; i < 6; i++) {
			printf("\t请输入第%d个红球号码（输入1～33范围内不重复的号码）：", i + 1);
			scanf("%d", &red[i]);
			if (red[i] < 1 || red[i]>33) {
				printf("\n\t%d不在红球号码范围内，请重新输入！\n", red[i]);
				return;
			}
		}
		printf("\t请输入蓝球号码（输入1～16范围的号码）：");
		scanf("%d", &blue);
		if (blue < 1 || blue>16) {
			printf("\n\t%d不在蓝球号码范围内，请重新输入！\n", blue);
			return;
		}
		int userBets;
		printf("\t请输入你的下注数量(不超过5注)：");
		scanf("%d", &userBets);
		if (userBets < 0 || userBets>5) {
			printf("\n\t下注数量输入错误，请重新输入！\n");
			return;
		}
		int userSurplus = userSurplusDecide(account);
		int userSpend = userBets * lotterBets;
		if (userSpend > userSurplus) {
			printf("\n\t账户余额不足，是否充值？(Y/N)\n");
			char choose;
			getchar();
			scanf("%c", &choose);
			if (choose == 'Y' || choose == 'y') {
				userSurplusRecharge(account);
			}
			else {
				printf("\n\t取消充值，返回上一页！\n");
				return;
			}
		}
		ModifyUserSurplus(account, userSpend);
		int total = lotterySoldTotal(canBuy);
		long CanBuy = canBuy;
		long UID = CanBuy * 10000 + total;
		int drawNum[7] = { red[0],red[1],red[2],red[3],red[4],red[5],blue };
		addLotterPrize(canBuy, userSpend);
		addUserBuyLotterToLink(account, canBuy, UID, drawNum, userBets);

		cls();
		sprintf(title, "感谢购彩");
		sprintf(content, "\n");
		sprintf(content, "%s彩票购买成功！详细信息如下：\n", content);
		sprintf(content, "%s购买期号：%d\n", content, canBuy);
		sprintf(content, "%s单注金额：%d\n", content, lotterBets);
		sprintf(content, "%s开奖号：", content);
		for (int i = 0; i < 6; i++) {
			if (red[i] < 10) {
				sprintf(content, "%s%02d-", content, red[i]);
			}
			else {
				sprintf(content, "%s%d-", content, red[i]);
			}
		}
		if (blue < 10) {
			sprintf(content, "%s--%02d\n", content, blue);
		}
		else {
			sprintf(content, "%s--%d\n", content, blue);
		}
		sprintf(content, "%s购买注数：%d\n", content, userBets);
		sprintf(content, "%s购买金额：%d\n", content, userSpend);
		sprintf(content, "%s唯一UID：%ld\n", content, UID);
		print_box(title, content);
		pause();
		return;
		
		
		/*printf("\n\t彩票购买成功！详细信息如下：\n");
		printf("\n\t购买期号：%d\n", canBuy);
		printf("\t单注金额：%d\n", lotterBets);
		printf("\t开奖号：");
		for (int i = 0; i < 6; i++) {
			if (red[i] < 10) {
				printf("%02d-", red[i]);
			}
			else {
				printf("%d-", red[i]);
			}
		}
		if (blue < 10) {
			printf("--%02d\n", blue);
		}
		else {
			printf("--%d\n", blue);
		}
		printf("\t购买注数：%d\n", userBets);
		printf("\t购买金额：%d\n", userSpend);
		printf("\t唯一UID：%ld\n\n", UID);*/

	}
	else {
		printf("\n\t当前无可购买彩票期号！\n");
		return;
	}

}

void addUserBuyLotterToLink(char account[100], int number, long uid, int drawNumber[7], int bets) {
	Lottery* data = malloc(sizeof(Lottery));
	data->number = number;
	data->uid = uid;
	for (int i = 0; i < 7; i++) {
		data->drawNumber[i] = drawNumber[i];
	}
	data->bets = bets;
	strcpy(data->account, account);
	data->winStatus = 0;
	data->winAmount = 0;

	UserBuy* newUserBuyNode = malloc(sizeof(UserBuy));
	newUserBuyNode->data = data;
	newUserBuyNode->next = NULL;
	if (UserBuyhead == NULL) {
		UserBuyhead = newUserBuyNode;
		UserBuycurrent = newUserBuyNode;
	}
	UserBuycurrent->next = newUserBuyNode;
	UserBuycurrent = newUserBuyNode;
}

void displayLotteryPurchaseHistory(char account[100]) {
	UserBuy* temp = UserBuyhead;
	cls();
	sprintf(title, "彩票数据库");
	sprintf(content, "\n");
	sprintf(content, "彩票期号  彩票ID      单注金额  购买注数  购买金额  彩票状态     用户购买号码           中奖金额\n");
	while (temp != NULL) {
		if (strcmp(temp->data->account, account) == 0) {
			int lotterBets = findLotteryBets(temp->data->number);
			int spend = lotterBets * temp->data->bets;
			sprintf(content, "%s%-10d%-12ld%-10d%-10d%-10d%-14s%02d-%02d-%02d-%02d-%02d-%02d-- -%02d%-2s%.2f\n", 
				content, 
				temp->data->number, 
				temp->data->uid,
				lotterBets, 
				temp->data->bets, 
				spend,
				temp->data->winStatus == 0 ? "未中奖" : "已中奖",
				temp->data->drawNumber[0], temp->data->drawNumber[1], temp->data->drawNumber[2], temp->data->drawNumber[3], temp->data->drawNumber[4], temp->data->drawNumber[5], temp->data->drawNumber[6],
				" ",
				temp->data->winAmount);
		}
		temp = temp->next;
	}
	print_box(title, content);
	pause();
}

void UpdateUserBalancesAfterDraw(int number, int drawNum[7]) {
	UserBuy* temp = UserBuyhead;
	while (temp != NULL) {
		if (temp->data->number == number) {
			int prize = findLotterPrize(number);
			if (drawNum[6] == temp->data->drawNumber[6]) {	//蓝球中
				int cnt = 0;
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						if (drawNum[i] == temp->data->drawNumber[j]) {
							cnt++;
						}
					}
				}
				if (cnt == 1 || cnt == 2) {					//1+1或2+1
					temp->data->winAmount = 5.0 * temp->data->bets;
					temp->data->winStatus = 1;
				}
				else if (cnt == 3) {					//3+1
					temp->data->winAmount = 10.0 * temp->data->bets;
					temp->data->winStatus = 1;
				}
				else if (cnt == 4) {					//4+1
					temp->data->winAmount = 200.0 * temp->data->bets;
					temp->data->winStatus = 1;
				}
				else if (cnt == 5) {					//5+1
					temp->data->winAmount = 3000.0 * temp->data->bets;
					temp->data->winStatus = 1;
				}
				else if (cnt == 6) {
					if (prize < 100000000.0) {
						temp->data->winAmount = 5000000 * 0.75 * temp->data->bets;
						temp->data->winStatus = 1;
					}
					else {
						temp->data->winAmount = 5000000.0 * temp->data->bets;
						temp->data->winStatus = 1;
					}
				}
				userSurplusAdd(temp->data->account, temp->data->winAmount);
				addUserCumulativeWinnAmount(temp->data->account, temp->data->winAmount);
				PrizePoolUpdates(number, temp->data->winAmount);
			}
			else {						//蓝球没中
				int cnt = 0;
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						if (drawNum[i] == temp->data->drawNumber[j]) {
							cnt++;
						}
					}
				}

				if (cnt == 4) {						//4+0
					temp->data->winAmount = 200.0 * temp->data->bets;
					temp->data->winStatus = 1;
				}
				else if (cnt == 5) {					//5+0
					temp->data->winAmount = 3000.0 * temp->data->bets;
					temp->data->winStatus = 1;
				}
				else if (cnt == 6) {					//6+0
					if (prize < 100000000.0) {
						temp->data->winAmount = 5000000.0 * 0.75 * temp->data->bets;
						temp->data->winStatus = 1;
					}
					else {
						temp->data->winAmount = 5000000.0 * temp->data->bets;
						temp->data->winStatus = 1;
					}
				}
				userSurplusAdd(temp->data->account, temp->data->winAmount);
				addUserCumulativeWinnAmount(temp->data->account, temp->data->winAmount);
				PrizePoolUpdates(number, temp->data->winAmount);
			}
		}
		temp = temp->next;
	}
}

void findLotterInfo() {			//查询某一期彩票中奖信息
	int number;
	printf("\n\t请输入查询彩票期号：");
	scanf("%d", &number);
	cls();
	flush();
	UserBuy* temp = UserBuyhead;
	sprintf(title, "感谢购彩");
	sprintf(content, "\n");
	sprintf(content, "%s彩票购买成功！详细信息如下：\n", content);
	sprintf(content,"%s彩票期号    UID     中奖用户  单注金额  购买注数  购买金额       用户开奖号         中奖金额\n", content);
	while (temp != NULL) {
		if (temp->data->number == number && temp->data->winStatus == 1) {
			int lotterBets = findLotteryBets(temp->data->number);
			int spend = lotterBets * temp->data->bets;
			sprintf(content,"%s%-10d%-10ld%-10s%-10d%-10d%-10d%02d-%02d-%02d-%02d-%02d-%02d---%02d%-2s%-8.2f\n",
				content,
				temp->data->number, 
				temp->data->uid,
				temp->data->account,
				lotterBets, 
				temp->data->bets,
				spend,
				temp->data->drawNumber[0], temp->data->drawNumber[1], temp->data->drawNumber[2], temp->data->drawNumber[3], temp->data->drawNumber[4], temp->data->drawNumber[5], temp->data->drawNumber[6], 
				" ",
				temp->data->winAmount);
		}
		temp = temp->next;
	}
	print_box(title, content);
	pause();
}
