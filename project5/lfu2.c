#include <stdio.h>

int main()
{
    int n;
    printf("Enter String length :");
    scanf("%d",&n);
    int arr[n];
    printf("Enter the string\n");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    int frames;
    int stringCount[10]={0};
    printf("Enter frames :");
    scanf("%d",&frames);
    int arrframes[frames];
    for(int i=0;i<frames;i++){
        arrframes[i]=0;
    }
    int count=0;
    int pagefault=0;
    while(count < n){
        int min=100;
        int pos;
        int flag=0;
        int d=arr[count];
        for(int i=0;i<frames;i++){
            if( d == arrframes[i]){
                stringCount[arr[i]]++;
                flag=1;
                break;
            }
        }
            if(count < frames && flag==0){
             arrframes[count]=d;
             stringCount[arr[count]]++;
             pagefault++;
            }
            else if(count >= frames && flag==0){
                printf("Replacement takes place ");
                for(int k=0;k<frames;k++){
                    if(stringCount[arrframes[k]] < min){
                        min=stringCount[arrframes[k]];
                    }
                }

				//3번 반복
                for(int l=0;l<frames;l++){
                    if(stringCount[arrframes[l]] == min){
                        arrframes[l]=arr[count];
                        stringCount[arr[count]]++;
                        pagefault++;
                        break;
                    }
                }
            }
            for(int j=0;j<frames;j++){
                 printf("%d\t",arrframes[j]);
                }
        printf("\n");
        count++;
        }
       printf("\nPage fault is : %d",pagefault);
       
        return 0;
    }
