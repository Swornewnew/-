#include <iostream>
#include<sstream>
#include<fstream>
#include<windows.h>
using namespace std;
#define HEAD1 " *************************Airplane Ticket Booking*********************************************\n"
#define HEAD2 " |  Flight  |Start City| Dest City|DepartureTime  |ArrivalTime  |   price  | discount |   number |\n"
#define HEAD3 " |----------|----------|----------|---------------|-------------|----------|----------|----------|\n"
#define FORMAT  " |%-10s|%-10s|%-10s|%-12s   |%-10s   |%-10d|%-10s|%-10d|\n"
struct Plane
{
	char Flight[10];   /*航班号*/
	char Origin[10];   /*出发地*/
	char Dest[10];     /*目的地*/
	char TakeOffTime[10];  /*起飞时间*/
	char ReceiveTime[10];  /*降落时间*/
	int Price;        /*票价*/
	char Discount[4];     /*折扣*/
	int Num;            /*剩余票数*/
};
struct Man{
    char Name[10];  /*姓名*/
    char Id[20];    /*证件号*/
    int Num;    /*订票数量*/
    char Flight[20];  /*航班号*/
};
/*定义机票信息节点的结构体*/
struct PlaneNode
{
    struct Plane data;
    struct PlaneNode *next; 
};
/*定义订票人信息节点的结构体*/
struct ManNode
{
    struct Man data;
    struct ManNode *next;
};
void PrintHead()  //打印表头
{
	printf(HEAD1);
	printf(HEAD2);
	printf(HEAD3);
}
void PrintData(struct PlaneNode*stLp) //打印数据
{
	struct PlaneNode *pst = stLp;
	printf(FORMAT,pst->data.Flight,pst->data.Origin,pst->data.Dest,pst->data.TakeOffTime,
	pst->data.ReceiveTime,pst->data.Price,pst->data.Discount,pst->data.Num);
}
void Menu()  //菜单
{
	puts("**************************************************");
	puts("* Welcome to the airplane tickets booking system *");
	puts("*------------------------------------------------*");
	puts("*    choose the following operations(0-9);       *");
	puts("*------------------------------------------------*");
	puts("*   1.Insert flights    2.Search flights         *");
	puts("*   3.Book tickets      4.Modify flight data     *");
	puts("*   5.Show flights      6.Refund tickets         *");
	puts("*   7.quit                                       *");
	puts("**************************************************");
}
void gotoxy(short x, short y) //移动光标
{  
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
void Insert(struct PlaneNode* pstPlaneNodeHead)  //添加航班
{
	struct PlaneNode *pstHead,*pstTail, *pstCur, *pstNew;
	char acFlight[10];
	pstHead = pstTail = pstPlaneNodeHead;
	while (pstTail->next != NULL)
	{
		pstTail = pstTail->next;  /*让ptail指向最后一个节点*/
	}
printf("Input the new flight number:");
		scanf("%s", acFlight);
		pstCur = pstPlaneNodeHead->next;
		while (pstCur != NULL)
		{
			if (strcmp(acFlight, pstCur->data.Flight) == 0)
			{
				printf("this flight %s exists!\n", acFlight);
				return;/*如果航班号已存在，则返回*/
			}
			pstCur = pstCur->next;
		}
		/*如果航班号没有和现有记录中的航班号重复，则新建一个链表节点*/
		pstNew = (struct PlaneNode*)malloc(sizeof(struct PlaneNode));
		strcpy(pstNew->data.Flight, acFlight);
		printf("Input the Start City:\n");
		scanf("%s", pstNew->data.Origin);
		printf("Input the Dest City:\n");
		scanf("%s", pstNew->data.Dest);
		printf("Input the Departure Time (Format MM.DD.TT:TT):\n");
		scanf("%s", pstNew->data.TakeOffTime);
		printf("Input the Arrival Time (Format 00:00):\n");
		scanf("%s", pstNew->data.ReceiveTime);
		printf("Input the price of ticket:\n");
		scanf("%d", &pstNew->data.Price);
		printf("Input the discount (Format 0.0):\n");
		scanf("%s", pstNew->data.Discount);
		printf("Input the number of the tickets:\n");
		scanf("%d", &pstNew->data.Num);
		pstNew->next = NULL;
		pstTail->next = pstNew;
		pstTail = pstNew;
		printf("success!\n");
		return;	
}
void Search(struct PlaneNode* PlaneNodeHead)  //查找机票
{
	struct PlaneNode *PlaneNode;
	int choice = 0, iCount = 0;
	char acFlight[10], acDest[10];
	PlaneNode = PlaneNodeHead->next;
	if (PlaneNode == NULL)
	{
		printf("No flight record!\n");
		return;
	}
	printf("Choose one way according to:\n1.flight number:\n2.Dest:\n");
	scanf("%d", &choice);
	if (choice == 1)
	{
		printf("Input the flight number:");
		scanf("%s", acFlight);
		PrintHead();
		while (PlaneNode != NULL)
		{
			if (strcmp(PlaneNode->data.Flight, acFlight) == 0)
			{
				PrintData(PlaneNode);
				break;/*由于航班号是唯一的，找到一条即可退出*/
			}
			else
				PlaneNode = PlaneNode->next;
		}
		/*遍历一遍，均没有中途break，则提示用户没有记录*/
		if (PlaneNode == NULL)
			printf("Sorry, no record!\n");
	}
	else if (choice == 2)
	{
		printf("Input the Dest City:");
		scanf("%s", acDest);
		PrintHead();
		while (PlaneNode != NULL)
		{
			if (strcmp(PlaneNode->data.Dest, acDest) == 0)
			{
				/*由于相同目的地的航班可能是多条，所以需要遍历完整个链表*/
				PrintData(PlaneNode);
				iCount++;
			}
			PlaneNode = PlaneNode->next;
		}
		if (iCount == 0)/*如果记录数仍为0，则提示用户没有记录*/
			printf("Sorry, no record!\n");
	}
	else
	{
		printf("Sorry, please input right number.\n");
	}
	return;
}
void Book(struct ManNode* pstManNodeHead, struct PlaneNode* pstPlaneNodeHead)  //订票
{
	struct PlaneNode *pstPlaneNodeCur, *astPlaneNode[10]; /*定义临时指针和指针数组*/
	struct ManNode *pstManNodeCur, *pstManNodeTemp = 0;
	char acDest[10], acID[20], acName[20], acDecision[2], acFlight[10];
	int acNum;
	int iRecord = 0;
	int iFlag = 0;
	/*acNum表示预定票数，iRecord记录符合要求的航班数*/
	pstManNodeCur = pstManNodeHead;/*接收订票人信息链表的头指针*/
	while (pstManNodeCur->next != NULL)
		pstManNodeCur = pstManNodeCur->next;
	printf("Input the Dest City:\n");
	scanf("%s", acDest);/*输入目的地*/
	pstPlaneNodeCur = pstPlaneNodeHead->next;
	while (pstPlaneNodeCur != NULL)/*循环条件为链表不为空*/
	{
		if (strcmp(pstPlaneNodeCur->data.Dest, acDest) == 0)/*比较目的地*/
		{
			astPlaneNode[iRecord++] = pstPlaneNodeCur;/*目的地吻合的节点地址存到指针数组中*/
		}
		pstPlaneNodeCur = pstPlaneNodeCur->next;/*指针后移*/
	}
	printf("\nthere are %d flight you can choose!\n", iRecord);
	PrintHead();/*输出表头*/
	for (int k = 0; k < iRecord; k++)/*循环输出指针数组中的指针指向的航班信息*/
		PrintData(astPlaneNode[k]);
	if (iRecord == 0)/*若记录为0，表示没有符合条件的航班*/
		printf("sorry,no flight you can book!\n");
	else
	{
		printf("do you want to book it<y(Y)/n(N)>?\n");/*提示用户是否预定*/
		scanf("%s", acDecision);
		getchar(); /*提取多余的回车符，否则下面输入姓名的gets函数将获得一个回车符*/
		if (strcmp(acDecision, "y") == 0 || strcmp(acDecision, "Y") == 0)
		{
			printf("Input your information.\n");/*输入订票的详细信息*/
			pstManNodeTemp = (struct ManNode*)malloc(sizeof(struct ManNode));
			printf("Input your name:");
			gets(acName);
			strcpy(pstManNodeTemp->data.Name, acName);
			printf("Input your id:");
			scanf("%s", acID);
			strcpy(pstManNodeTemp->data.Id, acID);
			printf("Input the flight number:");
			scanf("%s", acFlight);
			strcpy(pstManNodeTemp->data.Flight, acFlight);
			int k;
			for (k = 0; k<iRecord; k++)
			{
				if (strcmp(astPlaneNode[k]->data.Flight, acFlight) == 0)
				{
					if (astPlaneNode[k]->data.Num < 1)/*判断是否有剩余的票*/
					{
						printf("no ticket!\n");
						return;
					}
					printf("remain %d tickets\n", astPlaneNode[k]->data.Num);
					iFlag = 1;
					break;
				}
			}
			if (iFlag == 0)
			{
				printf("error\n");
				return;
			}
			printf("Input the book number:");
			scanf("%d", &acNum);/*输入预定的票数，剩余票数相应减少*/
			astPlaneNode[k]->data.Num = astPlaneNode[k]->data.Num - acNum;
			pstManNodeTemp->data.Num = acNum;
			pstManNodeCur->next = pstManNodeTemp;
			pstManNodeTemp->next = NULL;
			pstManNodeCur = pstManNodeTemp;
			printf("remain %d tickets\n", astPlaneNode[k]->data.Num);
			printf("success!\n");
		}
	}
	return;
}
struct ManNode *FindMan(struct ManNode* pstManNodeHead, char acID[20])  //根据id查找用户信息
{
	struct ManNode *pstManNodeCur;
	pstManNodeCur = pstManNodeHead->next;
	while (pstManNodeCur)
	{
		if (strcmp(pstManNodeCur->data.Id, acID) == 0)
		{
			return pstManNodeCur;
		}
		pstManNodeCur = pstManNodeCur->next;
	}
	return NULL;
}
struct PlaneNode *FindPlane(struct PlaneNode* pstPlaneNodeHead, char acFlight[10])  //根据航班号查找航班信息
{
	struct PlaneNode *pstPlaneNodeCur;
	pstPlaneNodeCur = pstPlaneNodeHead->next;
	while (pstPlaneNodeCur)
	{
		if (strcmp(pstPlaneNodeCur->data.Flight, acFlight) == 0)
		{
			return pstPlaneNodeCur;
		}
		pstPlaneNodeCur = pstPlaneNodeCur->next;
	}
	return NULL;
}
void Refund(struct ManNode* pstManNodeHead, struct PlaneNode* pstPlaneNodeHead)  //退票
{
	struct ManNode *pstManNodeCur, *pstManNodeFind = 0;
	struct PlaneNode *pstPlaneNodeFind = 0;
	char acID[20], acDecision[2];
	int Num, booknum;
	printf("\nInput your ID:");
	scanf("%s", acID);
	pstManNodeFind=FindMan(pstManNodeHead, acID);
	if (pstManNodeFind == NULL)
		printf("can't find!\n");
	else  /*找到了相应的旅客订票信息*/
	{
		printf("\t\tthis is your tickets:\n");
		printf("id number:%s\n", pstManNodeFind->data.Id);
		printf("name:%s\n", pstManNodeFind->data.Name);
		printf("book flight:%s\n",pstManNodeFind->data.Flight);
		printf("book number:%d\n", pstManNodeFind->data.Num);
		printf("do you want to cancel it?<y/n>");
		scanf("%s", acDecision);
		if (strcmp(acDecision, "y") == 0 || strcmp(acDecision, "Y") == 0)
		{
			/*将pstManNodeCur定位到指向pstManNodeFind前面那个节点*/
			pstManNodeCur = pstManNodeHead;
			while (pstManNodeCur->next != pstManNodeFind)
				pstManNodeCur = pstManNodeCur->next;
			/*找到该旅客订票信息中对应的航班记录*/
			pstPlaneNodeFind = FindPlane(pstPlaneNodeHead, pstManNodeFind->data.Flight);
			if (pstPlaneNodeFind != NULL)/*退票后，对应航班的剩余票数相应增加*/
			{
				Num = pstPlaneNodeFind->data.Num;
				booknum = pstManNodeFind->data.Num;
				pstPlaneNodeFind->data.Num = Num + booknum;
			}				
			pstManNodeCur->next = pstManNodeFind->next;
			free(pstManNodeFind);/*释放该乘客订票记录的链表节点空间*/
			printf("success!\n");
		}
	}
	return;
}
void Modify(struct PlaneNode* pstPlaneNodeHead)  //修改航班信息
{
	struct PlaneNode *pstPlaneNodeCur;
	char acFlight[10];
	pstPlaneNodeCur = pstPlaneNodeHead->next;
	if (pstPlaneNodeCur == NULL)
	{
		printf("no flight to modify!\n");
		return;
	}
	else
	{
		printf("Input the flight number you want to modify:");
		scanf("%s", acFlight);
		while (pstPlaneNodeCur != NULL)
		{
			if (strcmp(pstPlaneNodeCur->data.Flight, acFlight) == 0)
				break;
			else
				pstPlaneNodeCur = pstPlaneNodeCur->next;
		}
		if (pstPlaneNodeCur)
		{
			printf("Input new Start City:\n");
			scanf("%s", pstPlaneNodeCur->data.Origin);
			printf("Input new Dest City:\n");
			scanf("%s", pstPlaneNodeCur->data.Dest);
			printf("Input new Departure Time:\n");
			scanf("%s", pstPlaneNodeCur->data.TakeOffTime);
			printf("Input new Arrival Time:\n");
			scanf("%s", pstPlaneNodeCur->data.ReceiveTime);
			printf("Input new price of ticket:\n");
			scanf("%d", &pstPlaneNodeCur->data.Price);
			printf("Input new discount:\n");
			scanf("%s", pstPlaneNodeCur->data.Discount);
			printf("Input new number of the tickets:\n");
			scanf("%d", &pstPlaneNodeCur->data.Num);
			printf("successful!\n");
			return;
		}
		else
			printf("\tcan't find your ticket!\n");
	}
	return;
}
void Show(struct PlaneNode* pstPlaneNodeHead)  //显示机票信息
{
	struct PlaneNode *pstPlaneNodeCur;
	pstPlaneNodeCur = pstPlaneNodeHead->next;
	PrintHead();
	if (pstPlaneNodeHead->next == NULL)
	{
		printf("no flight ticket!\n");
	}
	else
	{
		while (pstPlaneNodeCur != NULL)
		{
			PrintData(pstPlaneNodeCur);
			pstPlaneNodeCur = pstPlaneNodeCur->next;
		}
	}
	return;
}
int main(){
    string s;
    int choice;
    struct PlaneNode *PlaneNodeTemp, *PlaneNodeHead, *PlaneNodeCur;
	struct ManNode  *ManNodeTemp, *ManNodeHead, *ManNodeCur;
    PlaneNodeHead=(PlaneNode*)malloc(sizeof(PlaneNode));
	PlaneNodeHead->next=NULL;
	PlaneNodeCur=PlaneNodeHead;
	ManNodeHead=(ManNode*)malloc(sizeof(ManNode));
	ManNodeHead->next=NULL;
	ManNodeCur=ManNodeHead;
    ifstream  plane_file("plane.txt");
    ifstream man_file("man.txt");
    while(getline(plane_file,s)){
		int price,num;
        PlaneNodeTemp=(PlaneNode*)malloc(sizeof(PlaneNode));
        istringstream Planedata(s);
        Planedata>>PlaneNodeTemp->data.Flight>>PlaneNodeTemp->data.Origin>>PlaneNodeTemp->data.Dest>>
		PlaneNodeTemp->data.TakeOffTime>>PlaneNodeTemp->data.ReceiveTime;
		Planedata>>price;
		PlaneNodeTemp->data.Price=price;
		Planedata>>PlaneNodeTemp->data.Discount;
		Planedata>>num;
		PlaneNodeTemp->data.Num=num;
        PlaneNodeTemp->next=NULL;
		PlaneNodeCur->next=PlaneNodeTemp;
		PlaneNodeCur =PlaneNodeTemp;
    }
    while(getline(man_file,s)){
        ManNodeTemp=(ManNode*)malloc(sizeof(ManNode));
        istringstream Mandata(s);
        Mandata>>ManNodeTemp->data.Name>>ManNodeTemp->data.Id>>ManNodeTemp->data.Num>>ManNodeTemp->data.Flight;
        ManNodeTemp->next=NULL;
		ManNodeCur->next=ManNodeTemp;
		ManNodeCur =ManNodeTemp;
    }
	while(1){
		system("cls");
		gotoxy(0,0);
        Menu();
        cin>>choice;
		system("cls");
		gotoxy(0,0);
        if (choice==7){
			break;
		}
		switch (choice){
		case 1:{
			gotoxy(0,0);
			Insert(PlaneNodeHead);
			printf("do you want to quit?<y/n>");
			char t[2];
			scanf("%s",t);
			if(strcmp(t, "n") == 0 || strcmp(t, "N") == 0){
				break;
			} 
			return 0;
		}
		case 2: {
			gotoxy(0,0);
			Search(PlaneNodeHead);
			printf("do you want to quit?<y/n>");
			char t[2];
			scanf("%s",t);
			if(strcmp(t, "n") == 0 || strcmp(t, "N") == 0){
				break;
			} 
			return 0;
		}
		case 3: {
			gotoxy(0,0);
			Book(ManNodeHead, PlaneNodeHead);
			char t[2];
			printf("do you want to quit?<y/n>"); 
			scanf("%s",t);
			if(strcmp(t, "n") == 0 || strcmp(t, "N") == 0){
				break;
			}
			return 0; 
		}
		case 4:{
			gotoxy(0,0);
			Modify(PlaneNodeHead);
			char t[2];
			printf("do you want to quit?<y/n>");
			scanf("%s",t);
			if(strcmp(t, "n") == 0 || strcmp(t, "N") == 0){
				break;
			}
			return 0; 
		}
		case 5:{
			Show(PlaneNodeHead);
			char t[2];
			printf("do you want to quit?<y/n>");
			scanf("%s",t);
			if(strcmp(t, "n") == 0 || strcmp(t, "N") == 0){
				break;
			}
			return 0; 
		}
		 case 6:{
			gotoxy(0,0);
			Refund(ManNodeHead, PlaneNodeHead);
			char t[2];
			printf("do you want to quit?<y/n>");
			scanf("%s",t);
			if(strcmp(t, "n") == 0 || strcmp(t, "N") == 0){
				break;
			}
			return 0; 
		}
		}
	}
    system("pause");
}
