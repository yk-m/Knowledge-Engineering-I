#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILEPATH "/Users/yuka/SampleData/dataS/%02d.ftr"

void importData(double ave[], FILE *fp);
void calcAve   (double ave[], int **data, int num);
void output    (double ave[46][64]);

int main(int argc, char *argv[]) {
	FILE *fp;
	int i;
	char fn[64];
	double ave[46][64];

	for (i=0; i<46; i++) {
		sprintf(fn, FILEPATH, i+1);
		fp = fopen(fn, "r");
		if (fp==NULL) {
			printf("can't open a file\n");
			exit(1);
		}

		importData(ave[i], fp);
		fclose(fp);
	}
	output(ave);
}

void importData(double ave[], FILE *fp)
{
	char readline[1024];
	int **data, num, col, pos, buf
	  , row = 0
	  , length = 100
	;

	data = (int **)malloc(sizeof(int*)*length);
	data[0] = (int *)malloc(sizeof(int)*64);
	
	while (fgets(readline, sizeof(readline), fp) != NULL) {
		col=0;
		pos=0;
		buf=0;

		while (sscanf(readline+pos, "%d%n", &data[row][col], &buf) != EOF) { col++; pos += buf; }
		row++;

		if (row == length) {
			length += 100;
			data = (int **)realloc(data, sizeof(int*)*length);
		}

		data[row] = (int *)malloc(sizeof(int)*64);
	}
	calcAve (ave, data, row);
}

void calcAve (double ave[], int **data, int num)
{
	int i, j;

	for (i=0; i<64; i++) {
		ave[i] = 0;
		for (j=0; j<num; j++) {
			ave[i] += data[j][i];
		}
		ave[i] /= num;
	}
}

void output (double ave[46][64])
{
	int i, j;
	FILE *fp;

	fp = fopen("ave.dic", "w");
	if (fp==NULL) {
		printf("can't open a file\n");
		exit(1);
	}

	for (i=0; i<46; i++) {
		for (j=0; j<64; j++) {
			fprintf(fp, "%lf ", ave[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}
