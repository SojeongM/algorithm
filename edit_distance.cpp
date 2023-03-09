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
	int S = strlen(SS);	//SS�� ����
	int T = strlen(TS); //TS�� ����
	int i, j;
	int **sub = new int*[S+1]; //sub cost�� �߻��ϴ��� ���θ� ����

	Table[0][0] = 0;
	sub[0] = new int[T+1];
	for (i = 1; i <= S; i++) {
		Table[i][0] = Table[i-1][0]+del_cost;
		sub[i] = new int[T+1];
	}
	for (i = 1; i <= T; i++) {
		Table[0][i] = Table[0][i-1]+ins_cost;
	}	//Table�� 1��, 1���� ���� ����
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
	} //Table ������ ����
	int k = 0;
	i = S;
	j = T;
	while (1) {
		if (i == 0&&j!=0) {
			j = j - 1;
		}//1�࿡ ��ġ�� ���
		else if (j == 0&& i != 0) {
			i = i - 1;
		}//1���� ��ġ�� ���
		else {
			int c = min(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub[i - 1][j - 1]);
			if (c == Table[i][j - 1] + ins_cost &&  Table[i][j - 1] + ins_cost == Table[i - 1][j] + del_cost) {
				if (del_cost < ins_cost) {
					i = i - 1;
				}//inset�� delete ���� ������ ��� delete ���� �� ������ delete�� ��Ʈ��ŷ���ְ�
				else {
					j = j - 1;
				}//�ƴ� ��� insert�� ��Ʈ��ŷ
			}
			else if (c == Table[i][j - 1] + ins_cost) {
				j = j - 1;
			}//insert�� delete�� ���� �ʰ� �ּ� ���� insert�� ���
			else if (c == Table[i-1][j] + del_cost){
				i = i - 1;
			}//insert�� delete�� ���� �ʰ� �ּ� ���� delete�� ���
			else if (c == Table[i - 1][j - 1] + sub[i - 1][j - 1]) {
				i = i - 1;
				j = j - 1;
			}//insert�� delete�� ���� �ʰ� �ּ� ���� subtraction�� ���
		}
		k++;
		if (i == 0 && j == 0)	break;
	}	//backtracking - backtracking �� �ҿ�Ǵ� �Ÿ� ���ϱ�
	*SR = new char[k+1];
	*OP = new char[k+1];
	*TR = new char[k+1];
	(*Mem_Allocated) = k + k + k + 3;
	(*SR)[k] = '\0';
	(*OP)[k] = '\0';
	(*TR)[k] = '\0';	// SR, OP, TR �Ҵ�
	i = S;
	j = T;
	while (1) {
		if (i == 0&&j!=0) {
			(*OP)[k - 1] = 'i';
			(*SR)[k - 1] = '*';
			(*TR)[k - 1] = TS[j-1];
			j = j - 1;
		}	//1�࿡ ��ġ�� ���
		else if (j == 0&&i!=0) {
			(*OP)[k - 1] = 'd';
			(*SR)[k - 1] = SS[i-1];
			(*TR)[k - 1] = '*';
			i = i - 1;
		}	//1���� ��ġ�� ���
		else {
			int c = min(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub[i - 1][j - 1]);

			if (c == Table[i][j-1]+ins_cost && Table[i][j - 1] + ins_cost == Table[i - 1][j] + del_cost) {
				if (del_cost < ins_cost) {
					(*OP)[k - 1] = 'd';
					(*SR)[k - 1] = SS[i-1];
					(*TR)[k - 1] = '*';
					i = i - 1;
				}	//inset�� delete ���� ������ ��� delete ���� �� ������ delete ������, 
				else {
					(*OP)[k - 1] = 'i';
					(*SR)[k - 1] = '*';
					(*TR)[k - 1] = TS[j-1];
					j = j - 1;
				}	//�� ��찡 �ƴ� ��� insert ������ OP, SR, TR�� �����Ѵ�.

			}
			else if (c == Table[i][j - 1] + ins_cost) {
				(*OP)[k - 1] = 'i';
				(*SR)[k - 1] = '*';
				(*TR)[k - 1] = TS[j-1];
				j = j - 1;
			}	//insert�� delete�� ���� �ʰ� �ּ� ���� inset�� ���
			else if (c == Table[i-1][j] + del_cost) {
				(*OP)[k - 1] = 'd';
				(*SR)[k - 1] = SS[i-1];
				(*TR)[k - 1] = '*';
				i = i - 1;
			}	//insert�� delete�� ���� �ʰ� �ּ� ���� delete�� ���
			else if (c == Table[i - 1][j - 1] + sub[i - 1][j - 1]) {
				if (sub[i - 1][j - 1] == 0) {
					(*OP)[k - 1] = '.';
					(*SR)[k - 1] = SS[i - 1];
					(*TR)[k - 1] = TS[j - 1];
				}	//insert�� delete�� ���� �ʰ� �ּ� ���� sub�� ��� �� sub�� 0�� ��� OP�� .���� ����
				else {
					(*OP)[k - 1] = 's';
					(*SR)[k - 1] = SS[i - 1];
					(*TR)[k - 1] = TS[j - 1];
				}	//insert�� delete�� ���� �ʰ� �ּ� ���� sub�� ��� �� sub�� 0�� ��� OP�� �ش� ���ڷ� ����
				i = i - 1;
				j = j - 1;
			}	
		}
		k--;
		if (i == 0 && j == 0)	break;
	}	//backtracking �� ���� k ������ �ٽ� backtracking�ϸ� OP, SR, TR �� �����ϱ�
	for (int i = 0; i <= S; i++) {
		delete[] sub[i];
	}
	delete[] sub;	//sub �Ҵ� ����
}