
#include "Graph_adj_array.h"

void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {
	// read graph information
	// V와 E의 name field는 각각 자신의 인덱스를 저장한다
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.

	// ***이 함수를 작성한다
	int i;
	for (i = 0; i < Vnum; i++) {
		V[i].name = i;
		V[i].flag = false;
		V[i].f_hd = -1;
		V[i].r_hd = -1;
	}
	for (i = 0; i < Enum; i++) {
		E[i].name = i;
		E[i].flag = false;
		E[i].fp = -1;
		E[i].rp = -1;
	}

	for (i = 0; i < Enum; i++) {
		scanf_s("%d %d %d", &E[i].vf, &E[i].vr, &E[i].cost);
		int flag = 0;
		int k;
		int k = V[E[i].vf].f_hd;
		E[i].fp = k;
		V[E[i].vf].f_hd = i;
		k = V[E[i].vr].r_hd;
		E[i].rp = k;
		V[E[i].vr].r_hd = i;
	//	while (1) {
		/*	if (V[E[i].vf].f_hd != -1) {
				int k = V[E[i].vf].f_hd;
				E[i].fp = k;
				V[E[i].vf].f_hd = i;
			}
			else {
				V[E[i].vf].f_hd = i;
			//	break;
			}
			if (V[E[i].vr].r_hd != -1) {
				int k = V[E[i].vr].r_hd;
				E[i].rp = k;
				V[E[i].vr].r_hd = i;

			}
			else {
				V[E[i].vr].r_hd = i;
			}
			*/

	}

}

int DFS_Tree_adj_array(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0) 
) {
	// DFS를 사용하여 DFS tree에 속하는 에지의 flag을 true로 설정한다.
	// DFS 시작 vertex는 입력 파일에서 지정된다(src).
	// DFS tree에 속한 에지의 cost 함을 return 한다(모든 각 edge cost가 1이면 unique)
	// recursive 함수로 작성해도 무방하다.

	// ***이 함수를 작성한다***
	int c = 0;
	int f, r, i;
	int n = 0;;
	int p = 0;
	int k;
	

	while (1) {
		if (V[src].f_hd != -1) {
			if(V[E[V[src].f_hd]])
		}
	}

	for (i = 0; i < Enum; i++) {
		V[src].flag = true;
		if (p == 0) {
			f = V[src].f_hd;
			r = V[src].r_hd;
		}
		if (f != -1 && E[f].flag == false&& V[E[f].vr].flag == false) {
			if (V[E[f].vr].flag == false) {
				p = 1;
				V[E[f].vr].flag = true;
				E[f].flag = true;
				c += E[f].cost;
				c += DFS_Tree_adj_array(E[f].vr, Vnum, V, Enum, E);
			}
		}
		else if (r != -1 && E[r].flag == false&& V[E[r].vf].flag == false) {
			if (V[E[r].vf].flag == false) {
				p = 1;
				V[E[f].vf].flag = true;
				E[r].flag = true;
			//	printf("__%d ", r);
				c += E[r].flag;
				c += DFS_Tree_adj_array(E[r].vf, Vnum, V, Enum, E);
			}
		}
		else {
				if (f != -1) {
					if (E[f].fp != -1)	n = E[f].fp;
					else {
						n = E[f].rp;
					}
					
					if (E[n].vf == src && E[n].flag == false && n != -1&& V[E[n].vr].flag==false) {
						E[n].flag = true;
						V[E[n].vr].flag = true;
						c += E[n].flag;
						f = V[E[n].vr].f_hd;
						r = V[E[n].vr].r_hd;
						c += DFS_Tree_adj_array(E[n].vr, Vnum, V, Enum, E);
						
					}
					else if (E[n].vr == src && E[n].flag != 1 && n != -1 && V[E[n].vf].flag == false) {
						E[n].flag = true;
						V[E[n].vf].flag = true;
						c += E[n].flag;
						f = V[E[n].vf].f_hd;
						r = V[E[n].vf].r_hd;
						c += DFS_Tree_adj_array(E[n].vf, Vnum, V, Enum, E);
					}


				}
				else if (r != -1) {
					if (E[r].rp != -1) {
						n = E[r].rp;
						
					}
					else {
						n = E[r].fp;
					}
					
					if (E[n].vr == src && E[n].flag != 1 && n != -1 && V[E[n].vf].flag == false) {
						E[n].flag = true; 
						V[E[n].vf].flag = true;
						c += E[n].flag;
						f = V[E[n].vr].f_hd;
						r = V[E[n].vr].r_hd;
						c += DFS_Tree_adj_array(E[n].vf, Vnum, V, Enum, E);
						
					}
					else if (E[n].vf == src && E[n].flag != 1 && n != -1 && V[E[n].vr].flag == false) {
						E[n].flag = true;
						V[E[n].vr].flag = true;
						c += E[n].flag;
						f = V[E[n].vr].f_hd;
						r = V[E[n].vr].r_hd;
						c += DFS_Tree_adj_array(E[n].vr, Vnum, V, Enum, E);
						
					}
				

			}
			
			
		}
	
	}
	
	return c;
}
