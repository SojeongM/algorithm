#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <algorithm>

using namespace std;

double dist(double x1, double y1, double x2, double y2) {
	double d;
	d = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	return sqrt(d);
}//두 점 사이의 거리를 계산하는 함수

void merge(int left, int right, unsigned *Xid, unsigned *TMP, double *X) {
	int mid = (left + right) / 2;
	int i = left;
	int j = mid + 1;
	int k = left;
	while (i <= mid && j <= right) {
		if (X[Xid[i]] <= X[Xid[j]]) {
			TMP[k] = Xid[i];
			i++;
		}
		else {
			TMP[k] = Xid[j];
			j++;
		}
		k++;
	} //분할 후 정렬
	if (i > mid) {
		for (int p = j; p <= right; p++) {
			TMP[k] = Xid[p];
			k++;
		}
	}
	else {
		for (int p = i; p <= mid; p++) {
			TMP[k] = Xid[p];
			k++;
		}
	}//나머지 남는 부분을 TMP에 복사
	for (int p = left; p <= right; p++) {
		Xid[p] = TMP[p];
	}//TMP를 Xid에  복사
}//merge 를 실행

void partition(int left, int right, unsigned *Xid, unsigned *TMP, double *X) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		partition(left, mid, Xid, TMP, X);
		partition(mid + 1, right, Xid, TMP, X);
		merge(left, right, Xid, TMP, X);
	}
}//merge sort에서 divide를 실행

void   sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {
	partition(0, N - 1, Xid, TMP, X);
}//Xid를 오름차순으로 sort한다

double closestPairDC(
	unsigned L, unsigned R,	  // current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2, // closest pair points indices
	double *X, double *Y,         // (x,y) positions array(input)
	unsigned *Xid,  // point index array(sorted by x coordinates)
	unsigned *Yid,  // point index array(gradually sorted by y coordinates)
	unsigned *TMP,  // temporal index array
	unsigned THR	// threshold value
) {
	double d = DBL_MAX;
	double d_lr;

	if (R - L + 1 <= THR) {
		d = DBL_MAX;
		int k = 0;
		copy(Xid + L, Xid + R, Yid); //Xid 값을 Yid 값에 복사
		partition(0, k - 1, Yid, TMP, Y); //Yid 정렬
		for (int i = 0; i < k - 1; i++) {
			for (int j = i + 1; j < k; j++) {
				if (Y[Yid[j]] - Y[Yid[i]] >= d)	break;
				double d_i = dist(X[Yid[i]], Y[Yid[i]], X[Yid[j]], Y[Yid[j]]);
				if (d_i < d) {
					d = d_i;
					*pt1 = Yid[i];
					*pt2 = Yid[j];
				}
			}

		}//Brute Force 사용
		return d;
	}
	else {
		double d_l, d_r, d_c;
		unsigned M = (L + R) / 2;
		d_l = closestPairDC(L, M, pt1, pt2, X, Y, Xid, Yid, TMP, THR);
		unsigned t1 = *pt1, t2 = *pt2;
		d_r = closestPairDC(M + 1, R, pt1, pt2, X, Y, Xid, Yid, TMP, THR);
		d = d_r;
		if (d > d_l) {
			*pt1 = t1;
			*pt2 = t2;
			d = d_l;
		} //d_l과 d_r 중 작은 값의 정보를 d와 *pt1, *pt2에 저장
		int k = 0;
		unsigned mid = (X[Xid[M]] + X[Xid[M]]) / 2;
		for (int i = L; i <= M; i++) {
			if (X[Xid[i]] >= X[Xid[M+1]] - d) {
				Yid[k++] = Xid[i];
			}
		}
		for (int i = M + 1; i <= R; i++) {
			if (X[Xid[i]] <= X[Xid[M]] + d) {
				Yid[k++] = Xid[i];
			}
		}//d 라는 범위 안에 있는 X의 Xid값을 Yid에 복사
		partition(0, k/2, Yid, TMP, Y);
		partition(k/2 + 1, k-1, Yid, TMP, Y); //Yid 정렬
		for (int i = 0; i < k - 1; i++) {
			for (int j = i + 1; j < k; j++) {
				if (Y[Yid[j]] - Y[Yid[i]] >= d)	break;
				d_c = dist(X[Yid[i]], Y[Yid[i]], X[Yid[j]], Y[Yid[j]]);
				if (d_c < d) {
					d = d_c;
					*pt1 = Yid[i];
					*pt2 = Yid[j];
				}

			}
		}//범위 내의 거리들을 d와 비교하여 최단 거리 탐색
	}
	return d;
}