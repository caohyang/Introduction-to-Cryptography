#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

int main(){	
	int v[20000],len[4]={1e3,5e3,1e4,2e4};  char x;
	int C[10005],B[10005],T[10005],i,j,L,m,d,N=0;
	
	freopen("test.txt","r",stdin);
	while (scanf("%c",&x)!=EOF) v[N++]=x-'0';
	if (N>1e6) {printf("The string is too long!\n"); return 0;}
	printf("The string is: ");
	for (i=0;i<N;i++) printf("%c",'0'+v[i]);
	printf("\n");
	
	//BM Algorithm 
	for (i=0;i<10005;i++) B[i]=C[i]=T[i]=0;
	C[0]=B[0]=1; L=m=0; j=0;
	while (L<=1e4 && j<N){
		d=v[j];
		for (i=j-1;i>=j-L;i--) d^=C[j-i]*v[i];
		m++;
		if (d!=0){
			for (i=0;i<=L;i++) T[i]=C[i]; 
			for (i=0;i<=L;i++) C[i+m]^=B[i];
			if (L<=j/2){
				L=j+1-L; m=0;
				for (i=0;i<=L;i++) B[i]=T[i]; 
			}
		}
		j++;
	} 
	if (L>1e4) printf("The linear complexity is greater than 10000.\n");
	else{
		printf("The linear complexity is %d.\n",L);
		printf("The connecting polynomial is: 1");
		for (i=1;i<=L;i++)
			if (C[i]){
				if (i==1) printf("+D");
				else printf("+D^%d",i);
			}
		printf(".\n"); 
	} 
    return 0;
}
