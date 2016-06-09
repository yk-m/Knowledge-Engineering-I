void extract(char p[64][64])
{
	int i, j;
	int feature[16][4];

	for(i=0; i<16; i++) {
		for (j=0; j<4; j++) {
			feature[i][j] = 0;
		}
		for (y=i/4*16; y<i%4*16+16; y++) {
			for (x=i%4*16; x<i%4*16+16; x++) {
				if (p[y][x] == 1) {
					feature[i][0] = p[y][x-1] & p[y][x+1];
					feature[i][1] = p[y-1][x] & p[y+1][x];
					feature[i][2] = p[y-1][x-1] & p[y+1][x+1];
					feature[i][3] = p[y+1][x-1] & p[y-1][x+1];
				}
			}
		}
	}
}