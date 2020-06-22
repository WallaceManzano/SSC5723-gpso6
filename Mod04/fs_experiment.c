#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define PATH_NTFS "/media/wallacemanzano/NTFS"
#define PATH_FAT  "/media/wallacemanzano/FAT"
#define PATH_EXT4 "/media/wallacemanzano/EXT"

#define REP 10



int size[7] = {5, 10, 100, 1*1024, 10*1024, 100*1024, 500*1024};

float **experiment(char *path);
float *createFiles(char *path);
float *deleteFiles(char *path);
float *readfiles(char *path); 

void printResults(float ***results, int n);
void printMean(float **results[REP][3], int n);
void freeResults(float ***results);

int main() {
	float **results[REP][3];
	for(int i = 0; i < REP; i++){
		printf("---- Execution %d ---- \n", i + 1);
		results[i][0] = experiment(PATH_NTFS);
		results[i][1] = experiment(PATH_FAT);
		results[i][2] = experiment(PATH_EXT4);
	}

	for(int i = 0; i < REP; i++){
		printResults(results[i], i);
	}

	printMean(results, REP);

	for(int i = 0; i < REP; i++){
		freeResults(results[i]);
	}

	return 0;
}


void printResults(float ***results, int n) {
	printf("\n\n\n");
	printf("----- Exec %d -----\n", n + 1);
	for(int i = 0; i < 3; i++) {
		printf("%s\n", (i == 0) ? "NTFS" : ((i == 1) ? "FAT" : "EXT4"));
		printf("\n==========================\n");
		for(int j = 0; j < 3; j++) {

			printf("%s\n", (j == 0) ? "create" : ((j == 1) ? "read" : "delete"));
			for(int k = 0; k < 7; k++) {
				printf("%fsec ", results[i][j][k]);
			}
			printf("\n");
			// free(results[i][j]);
		}
		printf("==========================\n");
		// free(results[i]);
	}
}

void printMean(float **results[REP][3], int n) {
	printf("\n\n\n");
	float mean[3][3][7];
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {

			for(int k = 0; k < 7; k++) {
				mean[i][j][k] = 0;
				for(int v = 0; v < n; v++) {
					mean[i][j][k] += results[v][i][j][k];
				}
				mean[i][j][k] /= n;
			}
		}
	}


	printf("\n\n\n");
	printf("----- Mean-----\n");
	for(int i = 0; i < 3; i++) {
		printf("%s\n", (i == 0) ? "NTFS" : ((i == 1) ? "FAT" : "EXT4"));
		printf("\n==========================\n");
		for(int j = 0; j < 3; j++) {

			printf("%s\n", (j == 0) ? "create" : ((j == 1) ? "read" : "delete"));
			for(int k = 0; k < 7; k++) {
				printf("%fsec ", mean[i][j][k]);
			}
			printf("\n");
			// free(results[i][j]);
		}
		printf("==========================\n");
		// free(results[i]);
	}
}


void freeResults(float ***results) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			free(results[i][j]);
		}
		free(results[i]);
	}
}



float **experiment(char *path) {
	float **results = (float**) malloc(3 * sizeof(float *));
	results[0] = createFiles(path);
	results[1] = readfiles(path);
	results[2] = deleteFiles(path);
	return results;
}

float *createFiles(char *path) {
	FILE *pFile;
	float *seconds = (float*) malloc(7 * sizeof(float));
	struct timeval stop, start;
	char *string;

	char file_name[256];
	for(int i = 0; i < 7; i++) {
		sprintf(file_name,"%s/file%d%s.txt",path, (size[i] < 1024) ? size[i] : size[i]/1024, (size[i] < 1024) ? "Kb" : "Mb");


		gettimeofday(&start, NULL);

		pFile = fopen(file_name, "w");
		
        // fseek(pFile, size[i]*1024, SEEK_SET);
        // fputc('\0', pFile);

		ftruncate(fileno(pFile), size[i]*1024);

		fclose(pFile);
		printf("File created: %s\n", file_name);
		gettimeofday(&stop, NULL);

		seconds[i] = ((float) ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec))/1000000.0;
	}
	return seconds;
}


float *readfiles(char *path) {
	FILE *pFile;
	char file_name[256], buffer[256];
	float *seconds = (float*) malloc(7 * sizeof(float));
	struct timeval stop, start;

	for(int i = 0; i < 7; i++) {
		sprintf(file_name,"%s/file%d%s.txt",path, (size[i] < 1024) ? size[i] : size[i]/1024, (size[i] < 1024) ? "Kb" : "Mb");

		
		gettimeofday(&start, NULL);
		pFile = fopen(file_name, "r" );
		
        while( fgets (buffer, 254, pFile)!=NULL );
        fclose(pFile);
		printf("File readed: %s\n", file_name);
		gettimeofday(&stop, NULL);

		seconds[i] = ((float) ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec))/1000000.0;

   }

	return seconds;
}

float *deleteFiles(char *path) {
	FILE *pFile;
	char file_name[256];
	float *seconds = (float*) malloc(7 * sizeof(float));
	struct timeval stop, start;

	for(int i = 0; i < 7; i++) {
		sprintf(file_name,"%s/file%d%s.txt",path, (size[i] < 1024) ? size[i] : size[i]/1024, (size[i] < 1024) ? "Kb" : "Mb");

		gettimeofday(&start, NULL);

		remove(file_name);
		printf("File deleted: %s\n", file_name);
		gettimeofday(&stop, NULL);

		seconds[i] = ((float) ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec))/1000000.0;
	}

	return seconds;
}