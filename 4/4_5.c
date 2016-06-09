#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define DICFN "/Users/yuka/SampleData/dataBB/ave.dic"

void loadDic(double ave[46][64]);
int search(double ave[46][64], double features[64]);

int main(int argc, char *argv[])
{
	double features[64]
	     ,  ave[46][64];
	char readline[1024];
	int i, j, buf, windex, result, sum, count
	  , pos=0
	;

	FILE *fdr;

	loadDic(ave);
	
	for (i=1; 1<argc && i<argc; i++) {
		if ((fdr = fopen(argv[i], "rb")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}

		j=0;
		while (!isdigit(argv[i][j])) j++;
		windex = (argv[i][j]-48)*10 + (argv[i][j+1]-48);

		sum = 0;
		count = 0;

		// printf("%d\n", windex);
		while (fgets(readline, sizeof(readline), fdr) != NULL) {
			j=0;
			pos=0;
			buf=0;
			while (sscanf(readline+pos, "%lf%n", &features[j], &buf) != EOF)  { j++; pos += buf; }

			result = search(ave, features);
			if (result == windex) sum++;
			count++;
			printf("%d,", result);
		}
		printf("\n");
		// printf("No.%dの認識率 %d / %d\n", windex, sum, count);
		// printf("%f\n", sum/(double)count*100);

		fclose(fdr);
	}
}

void loadDic(double ave[46][64])
{
	FILE *dic;
	char readline[1024];
	int row=0, col, pos, buf;

	dic = fopen(DICFN, "r");
	if (dic==NULL) {
		printf("can't open a file\n");
		exit(1);
	}

	while (fgets(readline, sizeof(readline), dic) != NULL) {
		col=0;
		pos=0;
		buf=0;
		while (sscanf(readline+pos, "%lf%n", &ave[row][col], &buf) != EOF) { col++; pos += buf; }
		row++;
	}
}

int search(double ave[46][64], double features[64])
{
	double distance, nearest=0;
	int index;
	int i, j;

	for (i=0; i<46; i++) {
		distance = 0;
		for (j=0; j<64; j++) {
			distance += pow(features[j]-ave[i][j], 2);
		}
		if (distance < nearest || i==0) { nearest = distance; index = i; }
	}

	return index+1;
}
