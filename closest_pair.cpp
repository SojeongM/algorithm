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
}//�� �� ������ �Ÿ��� ����ϴ� �Լ�

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
	} //���� �� ����
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
	}//������ ���� �κ��� TMP�� ����
	for (int p = left; p <= right; p++) {
		Xid[p] = TMP[p];
	}//TMP�� Xid��  ����
}//merge �� ����

void partition(int left, int right, unsigned *Xid, unsigned *TMP, double *X) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		partition(left, mid, Xid, TMP, X);
		partition(mid + 1, right, Xid, TMP, X);
		merge(left, right, Xid, TMP, X);
	}
}//merge sort���� divide�� ����

void   sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {
	partition(0, N - 1, Xid, TMP, X);
}//Xid�� ������������ sort�Ѵ�

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
		copy(Xid + L, Xid + R, Yid); //Xid ���� Yid ���� ����
		partition(0, k - 1, Yid, TMP, Y); //Yid ����
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

		}//Brute Force ���
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
		} //d_l�� d_r �� ���� ���� ������ d�� *pt1, *pt2�� ����
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
		}//d ��� ���� �ȿ� �ִ� X�� Xid���� Yid�� ����
		partition(0, k/2, Yid, TMP, Y);
		partition(k/2 + 1, k-1, Yid, TMP, Y); //Yid ����
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
		}//���� ���� �Ÿ����� d�� ���Ͽ� �ִ� �Ÿ� Ž��
	}
	return d;
}