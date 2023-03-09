#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int min(int a, int b, int c) {
	if (a <= b && a <= c) {
		return a;
	}
	else if (b <= a && b <= c) {
		return b;
	}
	else if (c <= b && c <= a) {
		return c;
	}
}



void Edit_Distance(
	char *SS,		// Source string array
	char *TS,		// Edited string array
	int ins_cost, int del_cost, int sub_cost,
int **Table,    // Cost Table Calculated (2-d array (|SS|+1) x (|TS|+1) will be set)
char **SR,		// Source string with insertion to T marks '*'
char **OP,		// Operation sequence('i', 'd', 's', or '.' (nop))
char **TR,       // Edited string with deletion form S mark '*'
	int  *Mem_Allocated
) {
	int S = strlen(SS);	//SS의 길이
	int T = strlen(TS); //TS의 길이
	int i, j;
	int **sub = new int*[S+1]; //sub cost가 발생하는지 여부를 저장

	Table[0][0] = 0;
	sub[0] = new int[T+1];
	for (i = 1; i <= S; i++) {
		Table[i][0] = Table[i-1][0]+del_cost;
		sub[i] = new int[T+1];
	}
	for (i = 1; i <= T; i++) {
		Table[0][i] = Table[0][i-1]+ins_cost;
	}	//Table의 1행, 1열들 값을 지정
	for (i = 1; i <= S; i++) {
		for(j = 1; j <= T; j++) {
			if (SS[i-1] == TS[j-1]) {
				sub[i-1][j-1] = 0;
			}
			else {
				sub[i-1][j-1] = sub_cost;
			}
			Table[i][j] = min(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub[i-1][j-1]);
		}
	} //Table 값들을 설정
	int k = 0;
	i = S;
	j = T;
	while (1) {
		if (i == 0&&j!=0) {
			j = j - 1;
		}//1행에 위치할 경우
		else if (j == 0&& i != 0) {
			i = i - 1;
		}//1열에 위치할 경우
		else {
			int c = min(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub[i - 1][j - 1]);
			if (c == Table[i][j - 1] + ins_cost &&  Table[i][j - 1] + ins_cost == Table[i - 1][j] + del_cost) {
				if (del_cost < ins_cost) {
					i = i - 1;
				}//inset와 delete 값이 동일할 경우 delete 값이 더 작으면 delete로 백트레킹해주고
				else {
					j = j - 1;
				}//아닐 경우 insert로 백트레킹
			}
			else if (c == Table[i][j - 1] + ins_cost) {
				j = j - 1;
			}//insert와 delete가 같지 않고 최소 값이 insert일 경우
			else if (c == Table[i-1][j] + del_cost){
				i = i - 1;
			}//insert와 delete가 같지 않고 최소 값이 delete일 경우
			else if (c == Table[i - 1][j - 1] + sub[i - 1][j - 1]) {
				i = i - 1;
				j = j - 1;
			}//insert와 delete가 같지 않고 최소 값이 subtraction일 경우
		}
		k++;
		if (i == 0 && j == 0)	break;
	}	//backtracking - backtracking 시 소요되는 거리 구하기
	*SR = new char[k+1];
	*OP = new char[k+1];
	*TR = new char[k+1];
	(*Mem_Allocated) = k + k + k + 3;
	(*SR)[k] = '\0';
	(*OP)[k] = '\0';
	(*TR)[k] = '\0';	// SR, OP, TR 할당
	i = S;
	j = T;
	while (1) {
		if (i == 0&&j!=0) {
			(*OP)[k - 1] = 'i';
			(*SR)[k - 1] = '*';
			(*TR)[k - 1] = TS[j-1];
			j = j - 1;
		}	//1행에 위치할 경우
		else if (j == 0&&i!=0) {
			(*OP)[k - 1] = 'd';
			(*SR)[k - 1] = SS[i-1];
			(*TR)[k - 1] = '*';
			i = i - 1;
		}	//1열에 위치할 경우
		else {
			int c = min(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub[i - 1][j - 1]);

			if (c == Table[i][j-1]+ins_cost && Table[i][j - 1] + ins_cost == Table[i - 1][j] + del_cost) {
				if (del_cost < ins_cost) {
					(*OP)[k - 1] = 'd';
					(*SR)[k - 1] = SS[i-1];
					(*TR)[k - 1] = '*';
					i = i - 1;
				}	//inset와 delete 값이 동일할 경우 delete 값이 더 작으면 delete 값으로, 
				else {
					(*OP)[k - 1] = 'i';
					(*SR)[k - 1] = '*';
					(*TR)[k - 1] = TS[j-1];
					j = j - 1;
				}	//그 경우가 아닐 경우 insert 값으로 OP, SR, TR을 설정한다.

			}
			else if (c == Table[i][j - 1] + ins_cost) {
				(*OP)[k - 1] = 'i';
				(*SR)[k - 1] = '*';
				(*TR)[k - 1] = TS[j-1];
				j = j - 1;
			}	//insert와 delete가 같지 않고 최소 값이 inset일 경우
			else if (c == Table[i-1][j] + del_cost) {
				(*OP)[k - 1] = 'd';
				(*SR)[k - 1] = SS[i-1];
				(*TR)[k - 1] = '*';
				i = i - 1;
			}	//insert와 delete가 같지 않고 최소 값이 delete일 경우
			else if (c == Table[i - 1][j - 1] + sub[i - 1][j - 1]) {
				if (sub[i - 1][j - 1] == 0) {
					(*OP)[k - 1] = '.';
					(*SR)[k - 1] = SS[i - 1];
					(*TR)[k - 1] = TS[j - 1];
				}	//insert와 delete가 같지 않고 최소 값이 sub일 경우 중 sub가 0인 경우 OP를 .으로 설정
				else {
					(*OP)[k - 1] = 's';
					(*SR)[k - 1] = SS[i - 1];
					(*TR)[k - 1] = TS[j - 1];
				}	//insert와 delete가 같지 않고 최소 값이 sub일 경우 중 sub가 0인 경우 OP를 해당 문자로 설정
				i = i - 1;
				j = j - 1;
			}	
		}
		k--;
		if (i == 0 && j == 0)	break;
	}	//backtracking 후 구한 k 값으로 다시 backtracking하며 OP, SR, TR 값 지정하기
	for (int i = 0; i <= S; i++) {
		delete[] sub[i];
	}
	delete[] sub;	//sub 할당 해제
}