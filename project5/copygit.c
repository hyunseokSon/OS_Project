#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int in(int size, int * array, int value){
	int i;
	for(i=0;i<size;i++){
		if(array[i] == value)
			return 1;
	}
	return 0;
}

int closestIndex(int size, int currentLocation, int * array,  int value, int direction){
	int i=currentLocation;
	if(direction==-1){
		printf("closetIndex의 I : %d\n", i);
		for(i--;i>=0;i--){
			if(array[i] == value)
				return i;
		}
		return i;
	}
	else{
		for(i++;i<size;i++){
    		if(array[i]==value){
                return i;
            }
		}
		return i;
	}
}

int extreme(int size, int * array, int type){
	int extreme = 0;
	if(type==1){//max
		int i=0;
		int max=array[0];
		for(i++;i<size;i++){
			if(array[i] > max){
				max = array[i];
				extreme = i;
			}
		}
		return extreme;
	}
	else{
		int i=0;
		int min=array[0];
		for(i++;i<size;i++){
			if(array[i] < min){
				min = array[i];
				extreme = i;
			}
		}
		return extreme;
	}
}
void printArray(int size, int * array){
    int i=0;
    for(;i<size;i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}
int main(){
	int n=20;
	int m=3;
	printf("Enter number of page requests: ");	// 500
	printf("Enter number of pages: ");			// 3
	int currPages[3]={0};
	int pageRequests[20]={7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};

	printf("Enter the page requests separated by space: \n"); // page_string;
	int i;

	int faults=0;
	int index=-1;
    for(i=0;i<3;i++) currPages[i] = -1;
	for(i=0;i<20;i++){
		if(in(m, currPages, pageRequests[i])) continue;
		else{
			currPages[(++index)%m] = pageRequests[i];
			faults++;
		}
	}
	printf("FIFO:\n");
    printf("\tPage faults : %d\n", faults);
    printf("\t       Hits : %d\n", n - faults);
    printf("\t  Hit Ratio : %.2f%%\n", (n-faults)*100.0/n);

	for(i=0;i<m;i++) currPages[i] = -1;
    int nextIndex[m];
	int filledBlocks = 0;
	faults = 0;
    index = -1;
    for(i=0;i<n;i++){
		if(in(filledBlocks, currPages, pageRequests[i])) continue;
		else{
			if(filledBlocks == m){
				int j=0;
				for(;j<m;j++){
					nextIndex[j] = 	closestIndex(n, i, pageRequests , currPages[j], 1);
    			}
				int farest = extreme(m, nextIndex, 1);
				currPages[farest] = pageRequests[i];
			}
			else{
				currPages[(++index)%m] = pageRequests[i];
				filledBlocks++;
			}
			faults++;
 		}
	}
	printf("\nOptimal: \n");
    printf("\tPage faults : %d\n", faults);
    printf("\t       Hits : %d\n", n - faults);
    printf("\t  Hit Ratio : %.2f%%\n", (n-faults)*100.0/n);

	//////////////////////////////////
	// m = 3, n = 500
	// currPages = pageArray, pageRequests = page_string
	// closestIndex = nearIndex
	// extreme = checkMax

    faults = 0;
    int prevIndex[m];
    filledBlocks = 0;
    index = -1;
    for(i=0;i<m;i++) currPages[i] = -1;
    for(i=0;i<n;i++){
        if(in(m, currPages, pageRequests[i])) continue;
        else{
            if(filledBlocks == m){
                int j=0;
					printf("i : %d", i);
                for(;j<m;j++){
                    prevIndex[j] = closestIndex(n, i, pageRequests, currPages[j], -1);
					printf("  prevIndex[%d] : %d  ", j, prevIndex[j]);
                }
				printf("\n");
                int farest = extreme(m, prevIndex, -1);
                currPages[farest] = pageRequests[i];
            }
            else{
                currPages[(++index)%m] = pageRequests[i];
                filledBlocks++;
            }
            faults++;
           }
    }
    printf("\nLRU: \n");
    printf("\tPage faults : %d\n", faults);
    printf("\t       Hits : %d\n", n - faults);
    printf("\t  Hit Ratio : %.2f%%\n", (n-faults)*100.0/n);
    printf("\n");
}
