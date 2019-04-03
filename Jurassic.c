#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>

/* enter any other #includes that you deem necessary below */

#define NUMROWS	    9		/* Map size */
#define NUMCOLS     9
#define NUMWALKS    1000	/* Number of random walks */

/* enter any other #defines as you deem necessary below */

void randomStep(int *x, int *y);//random walk function expect to perform 1000 steps random walk in 8 directions
int status(int i, int j, char map[NUMROWS][NUMCOLS]);//status function to 
void printResults(float results[NUMROWS][NUMCOLS], float s[NUMROWS][NUMCOLS]);
//MAIN FUNCTION-----------
int main(void) {
	/*variables to use during the program*/
	srand(time(NULL));
	int n, m = 0, a, b, e = 0;//a,b are used in nested for loops. m is for the number of walks taken
	int i, j;
	int number = 0, steps = 0, stat;
	int move;
	int END = 0;//#attempts
	float s[NUMROWS][NUMCOLS], ss[NUMROWS][NUMCOLS], ts[1000][81]; //ts=#steps for each succesfull walk - mean path
	float p[NUMROWS][NUMCOLS], mean[NUMROWS][NUMCOLS], sd[NUMROWS][NUMCOLS];
	float results[NUMROWS][NUMCOLS];
	float numstep[1000][81], temp;//numstep = #steps for each succesfull walk

	/* define the map and any other variables that you deem necessary: */

	printf("ISLAND MAP \n");
	char map[NUMROWS][NUMCOLS] = {
	{'B','W','W','B','B','W','B','W','W'},
	{'L','L','L','V','L','L','L','L','B'},
	{'W','L','L','L','L','D','L','L','B'},
	{'B','L','L','D','L','L','L','L','W'},
	{'B','L','D','L','L','L','L','L','W'},
	{'W','L','L','L','L','L','V','L','B'},
	{'W','V','L','L','L','L','L','L','W'},
	{'W','L','L','L','D','L','L','L','W'},
	{'B','B','W','B','W','B','B','W','B'}
	};
	for (i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {
			printf("%c ", map[i][j]);
			if (j == 8) {
				printf("\n");
			}
		}
	}


	/* Perform random walks and calculate results: */
	for (a = 0; a < NUMROWS; a++) {
		for (b = 0; b < NUMCOLS; b++) {
			END = 0; m = 0; steps = 0;		//set the initial values
			while (END != NUMWALKS) {		//perform until END=NUMWALKS=1000
				i = a; j = b;  				//set the position of origin
				if ((map[i][j] == 'W') || (map[i][j] == 'V') || (map[i][j] == 'D')) { break; }//if the origin position is an obtacle, break. 
				else {
					for (n = 1; n < 1000; n++) {		//perform until 1000 steps or the cicle is break
						randomStep(&i, &j); 			//call randomStep function
						stat = status(i, j, map);		//call status function and save the result in stat
						if (stat == 1) { stat = 2; break; }		//if stat=1 i.e. W,D or V then break
						if (stat == 0) { stat = 2; m = m + 1; numstep[m][number] = n; steps = steps + n; break; }
						//if stat=0 i.e. then save the number of steps taken to leave and add +1 to the number of succesfull walks
						//numstep[m][number] = n saves the number of succesfull steps for each succesfull walk
					}
				}
				END = END + 1;
			}
			s[a][b] = m;//save the total number of succesfull walks per point of origin
			ss[a][b] = steps;//save the total number of steps for succesfulls walks per point of origin
			number = number + 1;//add +1 to number (which is usefull to keep track of the cell)
		}
	}


	//---Probability-----
	for (i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {
			if (ss[i][j] == 0 || s[i][j] == 0) {}
			else {
				p[i][j] = (s[i][j] / END) * 100;// #succesfull walks/ #attemps 
			}
		}
	}

	//---Mean path steps-----
	for (i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {
			if (ss[i][j] == 0 || s[i][j] == 0) {}
			else {
				mean[i][j] = (ss[i][j] / s[i][j]);//#steps for succesfulls walks/ #succesfull walk
			}
		}
	}

	//---Standard deviation-----
	number = 0;
	for (i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {
			if (ss[i][j] == 0 || s[i][j] == 0) {}
			else {
				temp = 0;
				for (n = 1; n <= s[i][j]; n++) {
					ts[n][number] = numstep[n][number] - mean[i][j];//save in ts=#steps for each succesfull walk - mean path
					temp = ts[n][number] * ts[n][number] + temp;//sum (ts*ts)
				}
				sd[i][j] = sqrt((temp) / s[i][j]);//square root of sum (ts*ts)/#succesfull walk
			}
			number = number + 1;
		}
	}

	/* Print results: */
	printf("\n Percentage (probability) of getting off Jurassic Island \n");
	printResults(p, s);//print probability
	printf("\n Mean path length when escaping \n");
	printResults(mean, s);//print mean path
	printf("\n Standard deviation of path legth when escaping \n");
	printResults(sd, s);//print standard deviation

	return 0;
}
/*Functions*/
void randomStep(int *x, int *y) { //randomstep function, calculates the next random step
	int move;
	int i, j;
	i = *x;
	j = *y;
	move = rand() % 8;						//generate a random integer number from 0 to 7
	switch (move) {
	case 0: i = i - 1; break;				//Move North
	case 1: i = i - 1; j = j + 1; break;	//Move North East
	case 2: j = j + 1;  break;				//Move East
	case 3: i = i + 1; j = j + 1;  break;	//Move South East
	case 4: i = i + 1;  break;				//Move South
	case 5: i = i + 1; j = j - 1; break;	//Move South West
	case 6: j = j - 1; break;				//Move West
	case 7: i = i - 1; j = j - 1;  break;	//Move north West
	}
	*x = i;
	*y = j;
	return;
}


int status(int i, int j, char map[NUMROWS][NUMCOLS]) {//verify the status of the step taken
	int e = 2;
	if (( (i < 0) || (j < 0) || (i == 9) || (j == 9))) { //if the new step is out the array e=0
		e = 0;
	}
	else {
		switch (map[i][j]) { //if the new step is into W, D or V, then e=1
		case 'W': e = 1; break;
		case 'D': e = 1; break;
		case 'V': e = 1; break;
		}
	}
	//if nethier of those is the case, then e=2, 2 = continue walk 
	return e; //return the value of e
}

void printResults(float results[NUMROWS][NUMCOLS], float s[NUMROWS][NUMCOLS]) {//print results
	float zero = 0;
	int i, j;
	for (i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {
			if (s[i][j] == 0) { printf("%5.2f ", zero); } //if s (the # of results) equals 0, then print 0.00
			else {
				printf("%5.2f ", results[i][j]);//print the results up to 2 decimals
			}
			if (j == 8) { printf("\n"); }
		}
	}
	return;
}
