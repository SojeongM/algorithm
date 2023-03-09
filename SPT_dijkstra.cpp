#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define NONE -1

typedef struct elm_edge {
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if the edge is in the SP spanning tree, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct elm_vertex {
	int  f_hd, r_hd;   // adj list header(-1 if none)
	int  distance;      // distance from src to this vertex
	int  heap_idx;      // minHeap index
	bool inS;         // true if SP length found(true when move from V-S to S) 
} vertex;

void swap(int *a, int *b) {
	int  temp;
	temp = *a;
	*a = *b;
	*b = temp;
}  //두 정수의 값을 바꾸는 swap 함수

void adjust(int *heap, int p, int n, vertex *V) {
	int c = p * 2;

	int t = heap[p];
	while (c <= n) {
		if (c < n&&V[heap[c]].distance < V[heap[c + 1]].distance) { //좌 우 자식 노드 중 더 큰 거리를 가진 것을 선택
			c++;
		}
		if (c > n) {
			break;
		}
		if (V[heap[c]].distance < V[t].distance)   break;
		else {
			heap[c / 2] = heap[c];  //자식 노드와 바꿔준다
			c *= 2;
		}
	}
	heap[c / 2] = t;
} //heap 다시 정렬하는 함수

int del(int *heap, int *h, vertex *V, int Vnum) {
	V[heap[1]].heap_idx = *h;
	V[heap[*h]].heap_idx = 1;  //index 다시 설정
	int t = heap[1];
	swap(&heap[1], &heap[*h]);   //첫 번째 원소와 마지막 원소 바꿔주기(첫 원소가 삭제될 것)
	(*h) -= 1;
	for (int i = 1; i <= *h;) {
		int l = i * 2;
		int r = i * 2 + 1;
		int n = 0;
		if (l <= *h) {
			n = l;
		}
		if (r <= *h && V[heap[r]].distance < V[heap[n]].distance) { 
			n = r;
		}
		if (n == 0 || V[heap[i]].distance <= V[heap[n]].distance)   break;
		if (V[heap[i]].distance > V[heap[n]].distance) {  //distance가 하위 원소보다 더 크다면
			swap(&V[heap[i]].heap_idx, &V[heap[n]].heap_idx);
			swap(&heap[i], &heap[n]);
		}
		i = n;
	}
	return t;
}//heap delete 함수

void push(int *heap, int n, int *h, vertex *V) {
	heap[++(*h)] = n; //마지막에 삽입
	V[heap[*h]].heap_idx = *h; //heap index 설정
	for (int i = *h; i != 1 && V[heap[i]].distance < V[heap[i / 2]].distance; i /= 2) {
		swap(&V[heap[i]].heap_idx, &V[heap[i / 2]].heap_idx);
		swap(&heap[i], &heap[i / 2]);
	} //만약 자식 원소가 부모 보다 더 작다면 두 원소를 바꾸어 정렬해준다.
}   //최소 힙에 원소를 삽입하는 push 함수



int SPT_Dijkstra(
	int src,   // source vertex index
	int Vnum, vertex *V,   // Vertex array size and the array
	int Enum, edge *E,      // Edge array size and the array

	int *minHeap   // array for min heap (array size = Vnum+1)
) {
	int treeCost = 0;
	int size_h = 0;
	for (int i = 0; i < Vnum; i++) {
		V[i].distance = INT_MAX;
		V[i].inS = false;
	} //초기화 - V에 대하여

	V[src].distance = 0; //초기화
	for (int i = 0; i < Enum; i++) {
		if (E[i].vf == src) {
			int k = E[i].vr;
			V[k].distance = V[src].distance + E[i].cost;
		} 
		else if (E[i].vr == src) {
			int k = E[i].vf;
			V[k].distance = V[src].distance + E[i].cost;
		}

	} //src와 인접한 vertex들의 distance를 설정해준다.

	int j = 0;
	for (int i = 0; i < Vnum; i++) {
		if (i != src) {
			push(minHeap, i, &size_h, V);
		}
	} //construct minHeap(V-src) - 최소 힙 초기 구성

	int k, h = 0;
	int u, w;
	for (int i = 0; i < Vnum - 2; i++) {
		for (u = 0; u < Vnum; u++) {
			if (V[u].inS == false)   break;
		}
		for (j = 1; j < Vnum; j++) {
			if (V[u].distance > V[j].distance&&V[j].inS == false) {
				u = j;
			}
		} //choose vertex u such that distacne[u] is minimum
		V[u].inS = true;  //방문 여부 표시
		del(minHeap, &size_h, V, Vnum);
		for (j = 0; j < Enum; j++) {
			if (E[j].vf == u) {
				w = E[j].vr;
				if (V[w].inS == false && V[u].distance + E[j].cost < V[w].distance) {
					V[w].distance = V[u].distance + E[j].cost;
					adjust(minHeap, V[w].heap_idx, size_h, V);
				}
			} //v와 인접한 w를 찾아 대소비쇼를 통해 distance 값 설정해준 후 트리 정렬을 다시 해준다.
			if (E[j].vr == u) {
				w = E[j].vf;
				if (V[w].inS == false && V[u].distance + E[j].cost < V[w].distance) {
					V[w].distance = V[u].distance + E[j].cost;
					adjust(minHeap, V[w].heap_idx, size_h, V);
				}
			} //v와 인접한 w를 찾아 대소비쇼를 통해 distance 값 설정해준 후 트리 정렬을 다시 해준다.
		}
	}
	int f;
	for (int i = 0; i < Vnum; i++) {
		if (i != src) {
			f = -1; //인접한 리스트 중 가장 cost 값이 적은 edge의 정보를 저장
			int min = INT_MAX;
			for (j = 0; j < Enum; j++) {
				if (E[j].vf == i) {
					w = E[j].vr;
					if (V[i].distance == V[w].distance + E[j].cost&&E[j].flag == false) {
						if (min > E[j].cost) {
							f = j;
							min = E[j].cost;
						} //i와 인접한 w를 찾아 대소비교를 통해 distance 값 설정해준 후 트리 정렬을 다시 해준다.
					}
				} //E의 vr이 i값인 경우
				if (E[j].vr == i) {
					w = E[j].vf;
					if (V[i].distance == V[w].distance + E[j].cost&&E[j].flag == false) {
						if (min > E[j].cost) {
							f = j;
							min = E[j].cost;
						}
					} //i와 인접한 w를 찾아 대소비교를 통해 distance 값 설정해준 후 트리 정렬을 다시 해준다.
				} //E의 vf가 i값인 경우
			}
			if (f >= 0 && f < Enum && E[f].flag == false) {
				treeCost += E[f].cost;
				E[f].flag = true;
			} //최종적으로 얻은 최소 cost를 treeCost에 더하고, 해당 엣지의 flag를 true로 설정해 방문했음을 나타내준다
		}
	} //반복
	V[src].inS = true;
	return treeCost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	int i, j;
	for (j = 0; j < Vnum; j++) {
		V[j].f_hd = -1; V[j].r_hd = -1;
		V[j].inS = false;
	} //V 원소들 초기화
	for (i = 0; i < Enum; i++) {
		E[i].fp = -1; E[i].rp = -1; E[i].vf = -1; E[i].vr = -1;      //초기화
		scanf_s("%d %d %d", &E[i].vf, &E[i].vr, &E[i].cost);      //입력 받기
		E[i].flag = false;                                 //E[].flag 초기ㅘ
		E[i].fp = V[E[i].vf].f_hd;                          
		E[i].rp = V[E[i].vr].r_hd;    //V[E[i].vr].f_hd r_hd 값 설정
		V[E[i].vf].f_hd = i;                              
		V[E[i].vr].r_hd = i;    //V[E[i].vr].f_hd, r_hd 값 입력

	} //E 원소들 초기화

}

// the following functions are for testing if the submitted program is correct.
int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);
void Error_Exit(const char *s);

int main(void) {
	int      src;
	vertex *V;      int Vnum;
	edge   *E;      int Enum;
	int    *minHeap;
	int    Tree_cost;
	int    Tnum;       // # of test cases
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);      // read # of tests

	for (int t = 0; t < Tnum; t++) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex[Vnum];
		E = new edge[Enum];
		minHeap = new int[Vnum + 1];   // heap array allocation
		if (V == NULL || E == NULL || minHeap == NULL) {
			Error_Exit("Memory Allocation Error");
		}
		Read_Graph(Vnum, V, Enum, E);

		/**/start = clock();   // start timer

		Tree_cost = SPT_Dijkstra(src, Vnum, V, Enum, E, minHeap);   // code by students

		/**/finish = clock();   // stop timer
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

		// 아래 Tree_Check와 SPT_test 함수는 첨부한 SPT_test.obj 파일에 있다.
		// 이 테스트에서 오류로 인하여 프로그램이 정지하면 뭔가 잘못된 것이다(제출해도 0점)
		if (Tree_Check(Vnum, V, Enum, E, minHeap) == 0) {
			Error_Exit("   ERROR The result is not a spanning tree");
		}
		if (SPT_test(src, Vnum, V, Enum, E, minHeap) == false) {
			Error_Exit("** Something wrong in applying Dijkstra's");
		}
		if (t != 0)
			printf("\n");

		printf("**T%d (Dijkstra) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d\n",
			t + 1, Vnum, Enum, cmpt, Tree_cost);

		delete[] minHeap; delete[] V; delete[] E;
	}
	return 0;
}

void Error_Exit(const char *s) {
	printf("%s\n", s);
	exit(-1);
}