#include "Graph_adj_list.h"
#include <queue>
using namespace std;

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.

	// ***�� �Լ��� �ۼ��Ѵ�
	int i;
	pool.SLL_cnt = 0;
	for (i = 0; i < Vnum; i++) {
		V[i].name = i;
		V[i].flag = 0;
	}
	for (i = 0; i < Enum; i++) {
		E[i].name = i;
		E[i].flag = 0;
		scanf_s("%d %d %d", &E[i].vf, &E[i].vr, &E[i].cost);
		
		SLL *p;
		p = pool.allocSLL();
		p->i = i;
		V[E[i].vf].S.push(p);
		pool.SLL_cnt += 1;
		SLL *q;
		q = pool.allocSLL();
		q->i = i;
		V[E[i].vr].S.push(q);
		pool.SLL_cnt += 1;
		
	}

}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�

	int k;
	for (int i = 0; i < Vnum; i++) {
		while (V[i].S.top()!=NULL) {
			SLL *p = V[i].S.pop();
			pool.freeSLL(p);
		}
	}
	pool.freeSLL_pool();
	// ***�� �Լ��� �ۼ��Ѵ�

}

int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	// BFS�� ����Ͽ� BFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// BFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// BFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (not unique but ��� �� edge cost�� 1�̸� unique)
	// -- �Լ��� ��� parameter�� ���� �ʿ�� ����.
	// -- BFS�� ���� queue�� �ʿ��ϸ� STL�� queue�� ����ص� ����

	// ***�� �Լ��� �ۼ��Ѵ�
	vertex p = V[src];
	int c = 0;
	queue<vertex> q;
	q.push(p);
	V[src].flag = 1;
	int j, k;
	int i = src;
	while (!q.empty()) {
		vertex f = q.front();
		q.pop();
	
		i = f.name;
		SLL *b = f.S.top();
		while (b != NULL) {
			if (E[b->i].vf == i && E[b->i].flag != 1 && V[E[b->i].vr].flag != 1) {
				q.push(V[E[b->i].vr]);
				V[E[b->i].vr].flag = 1;
				c += E[b->i].cost;
				E[b->i].flag = 1;
		
			}
			else if(E[b->i].vr == i && E[b->i].flag != 1 && V[E[b->i].vf].flag != 1){
				q.push(V[E[b->i].vf]);
				V[E[b->i].vf].flag = 1;
				c += E[b->i].cost;
				E[b->i].flag = 1;
			
			}
			
			b = b->p;
			j = 0;
			for (k = 0; k < Vnum; k++) {
				if (V[k].flag == 1) j++;
			
			}
			if (j == Vnum) {
				break;
			}
		}
		if (j == Vnum)	break;
	
	}
	
	return c;
	
}

// SLL management functions
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;

}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST

	// ***�� �Լ��� �ۼ��Ѵ�
	SLL *p = ST;
	if (ST->p == NULL) {
		ST = NULL;
	}
	else {
		ST = ST->p;
	}
	return p;

}

SLL *sllStack2::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack2::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLList2 member functions
SLL *SLList2::allocSLL(void) {	// allocate an SLL element
	SLL *p ;
	if (SLL_pool == NULL) {
		p = (SLL*)malloc(sizeof(SLL));
		if (p == NULL) {
			errorExit("Memory allocation error(allocSLL)");
		}
		UsedMemoryForSLLs += sizeof(SLL);
		p->i = NONE;

		
	}
	else {
		p = SLL_pool;
		SLL_pool = p->p;
	}
	p->p = NULL;
	
	// ***�� �Լ��� �ۼ��Ѵ�
	return(p);
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}
	p->i = NONE;
	p->p = SLL_pool;
	SLL_pool = p;
	--SLL_cnt;
	// ***�� �Լ��� �ۼ��Ѵ�

}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	// ***�� �Լ��� �ۼ��Ѵ�

	while (p != NULL) {
		SLL_pool = p->p;
		free(p);
		UsedMemoryForSLLs -= sizeof(SLL);
		p = SLL_pool;
	}

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}

	
}