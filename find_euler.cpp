#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <time.h>
#include <stack>
#include "DBL.h"

//#define NO_PATH_OUT   // comment out this line for path output
void Error_Exit(const char *s);

typedef struct _Vertex {
	dblStack S;		// adj list contains edge index
	int degree;
} Vertex;

typedef struct _Edge {
	int v1, v2;
} Edge;

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN);
void adjListGenerate(Vertex *V, Edge *E, int VN, int EN);
void deallocGraph(Vertex *V, Edge *E, int VN);
int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN);

DBList pool;	// DBL storage pool

int main() {
	int T, VN, EN;
	Vertex *V;
	Edge   *E;
	int *path;	// Euler cycle or path
	int pathN;  // path length
	int  flag;	// 0: cycle, 1: path, 2: none
	clock_t start_time, finish_time;

	scanf("%d", &T);	// read # of tests
	for (int t = 1; t <= T; t++) {	// for each test
		graphGeneration(&V, &E, &VN, &EN);

		start_time = clock(); // set the start time

		path = Find_Euler(V, E, VN, EN, &flag, &pathN); // find an Euler path or cycle

		finish_time = clock(); // set finish time

		double cmpt = (((double)(finish_time - start_time)) / CLK_TCK) * 1000; // compute the time passed
		printf("Test= %d flag= %d VnumInCycle/Path)= %d ", t, flag, pathN);

		if (flag == 0)
			printf("Euler_cycle(exec_time= %.2f msec)\n", cmpt);
		else if (flag == 1)
			printf("Euler_path(exec_time= %.2f msec)\n", cmpt);
		else
			printf("not_Eulerian(exec_time= %.2f msec)\n", cmpt);

#ifndef NO_PATH_OUT
		if (flag != 2) {
			for (int i = 0; i < EN + 1; i++) {
				printf("%d\n", path[i]);
			}
		}

#endif
		if (flag != 2)
			delete[] path;
		deallocGraph(V, E, VN);
	}
	pool.freeDBL_pool();	// clear all the DBL elements

	return 0;
}

int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN) {
	// input V, VN, E, EN
	// output through paramters
	//   *flag = 0 (Euler cycle), 1 (Euler path), 2 (not Eulerian)
	//   *pathN = size of path[] array
	// output by return
	//   *path = list of vertex ids found(Euler cycle or path)
	stack<int> ST;		// use STL stack as explained in the class
	int *path = NULL;
	path = new int[EN+1];
	
	// *** 이 부분을 작성하세요
	int s = 0;
	int d;
	int w;
	int j = 0;
	*pathN = 0;
	int y = 0;
	int v;
	
	for (int i = 0; i < VN; i++) {
		if (V[i].degree % 2 == 1) {
			if (y == 0) {
				s = i;
			}
			y++;
		}
	}
	
	if (y == 0) {
		s = 0;
		*flag = 0;
	}
	else if (y == 2) {
		*flag = 1;
	}
	else {
		*flag = 2;
		return path;
	}
	ST.push(s);
	while (!ST.empty()) {
		s = ST.top();
		if (V[s].degree == 0) {
			v = ST.top();
			path[*pathN] = v;
			*pathN += 1;
			ST.pop();
		}
		else {
			DBL *p = V[s].S.top();
			d = p->d;
			if (s == E[d].v1)	w = E[d].v2;
			else if (s == E[d].v2) {
				w = E[d].v1;
			}
			DBL *q;
			if (p->twin != NULL) {
				q = p->twin;
				int k;
				if (E[q->d].v1 == s) {
					k = E[q->d].v2;
				}
				else if (E[q->d].v2 == s)
				{
					k = E[q->d].v1;
				}
				V[k].S.remove(q);
				V[k].degree--;
				pool.DBL_cnt--;
			}
				V[s].S.remove(p);
				V[s].degree--;
				pool.DBL_cnt--;
				ST.push(w);
				s = w;
			}
		}

	return path;
}

void deallocGraph(Vertex *V, Edge *E, int VN) {
	DBL *p;
	// *** 여기에 adj list를 삭제하는 routine을 작성하세요
	for (int i = 0; i < VN; i++) {
		p = V[i].S.top();
		
		while (p != NULL) {
			DBL *q = p;
			p = p->rp;
			pool.freeDBL(q);
		}
	}
	delete[] V;
	delete[] E;
}

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN) {
	scanf("%d %d", VN, EN);	// read # of vertices and edges
	//allocVandEarray(V, E, *VN, *EN);	// vertex and edge array allocation

	*V = new Vertex[*VN];
	*E = new Edge[*EN];
	if (*V == NULL || *E == NULL) {
		Error_Exit("Memory Allocation Error!");
	}

	for (int v = 0; v < *VN; v++) {
		(*V)[v].degree = 0;
	}
	for (int e = 0; e < *EN; e++) {
		scanf("%d %d", &((*E)[e].v1), &((*E)[e].v2));	// read edge information
		++((*V)[(*E)[e].v1].degree);
		++((*V)[(*E)[e].v2].degree);
	}
	adjListGenerate(*V, *E, *VN, *EN);	// create adj lust in G=(V,E)
}

void adjListGenerate(Vertex *V, Edge *E, int VN, int EN) {
	// Construct adjacent list in V array
	int v1, v2;
	DBL *adjE1, *adjE2;
	int i;
	for (i = 0; i < EN; i++) {
		v1 = E[i].v1;
		v2 = E[i].v2;
		adjE1 = pool.allocDBL();
		adjE1->d=i;
		V[v1].S.push(adjE1);
		V[v1].degree++;
		adjE2 = pool.allocDBL();
		adjE2->d=i;
		V[v2].S.push(adjE2);
		V[v2].degree++;
	}
	for (i = 0; i < VN; i++) {
		V[i].degree /= 2;
		DBL *p = V[i].S.top();
		while (p != NULL) {
			for (int j = 0; j < VN; j++) {
				if (i != j) {
					DBL *q = V[j].S.top();
					while (q != NULL) {
						if (q->d == p->d) {
							p->twin = q;
						}
						q = q->rp;
					}
				}
				
			}
			p = p->rp;
		}
		
	}
	// *** 이 부분을 작성하세요
}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {		// allocate a DBL element
	DBL *p;

	// *** 이 부분을 작성하세요
	if (DBL_pool == NULL) {
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL)");
		}
		UsedMemoryForDBLs += sizeof(DBL);
		p->d = NONE;
	}
	else {
		p = DBL_pool;
		DBL_pool = p->rp;
	}
	p->rp = p->lp = p->twin = NULL;
	++DBL_cnt;

	return(p);
}

void DBList::freeDBL(DBL *p) {
	// move p to pool

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}

	// *** 이 부분을 작성하세요
	p->d = NONE;
	p->rp = DBL_pool;
	DBL_pool = p;
	--DBL_cnt;		// reduce # of active DBL elements by one 
}

void DBList::freeDBL_pool(void) {
	DBL *p = DBL_pool;

	while (p != NULL) {
		DBL_pool = p->rp;	// get next pointer
		delete p;
		p = DBL_pool;
		UsedMemoryForDBLs -= sizeof(DBL);
	}
	if (DBL_cnt != 0) {
		printf("%lu lu \n", DBL_cnt);
		Error_Exit("Non-zero DBL_cnt after cleanup.");
	}
}

void dblStack::push(DBL *p) {

	// *** 이 부분을 작성하세요
	if (ST != NULL) {
		ST->lp = p;
	}
	p->rp = ST;
	p->lp = NULL;
	ST = p;
}

DBL *dblStack::pop(void) {	// remove and return p in front of Q
	DBL *p;

	// *** 이 부분을 작성하세요
	p = ST;
	if (ST->rp == NULL)	ST = NULL;
	else {
		ST = ST->rp;
		ST->lp = NULL;
	}
	return p;
}

void dblStack::remove(DBL *p) {	// delete p from ST

	// *** 이 부분을 작성하세요
	if (ST == p) {
		ST = p->rp;
		if (p->rp != NULL) {
			p->rp->lp = NULL;
		}
	}
	else {
		p->lp->rp = p->rp;
		if (p->rp != NULL) {
			p->rp->lp = p->lp;
		}
	}
}


DBL *dblStack::top(void) {
	return ST;
}

bool dblStack::empty(void) {
	if (ST == NULL)
		return true;
	else
		return false;
}
