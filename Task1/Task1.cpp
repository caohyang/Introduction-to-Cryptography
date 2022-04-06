#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

const int N=1e6;
char inp[N],txt[N];
int startpos,endpos,totlen;
int cnt1,cnt2,cnt3; //number of 1/2/3 character(s)

struct word{
	char s[5];
	int cnt; //number of appearances
	double prob; //probability 
}w1[200],w2[40000],w3[N];

bool Judge(char ch){
	if (int(ch)<startpos || int(ch)>endpos) return false;
	return true;
}

int gcd(int x,int y){
	if (x<y) swap(x,y);
	int k;
	while (y!=0){
		k=x%y; x=y; y=k;
	}
	return x;
}

void init(){
	int i;
	for (i=0;i<26;i++){ //a-z
		w1[i].s[0]=char(i+97);
		w1[i].cnt=0;
	}
	for (i=27;i<52;i++){ //A-Z
		w1[i].s[0]=char(i+38);
		w1[i].cnt=0;
	}
}

int main(){
	int i,j,flag;
	
	init();
	printf("Analyze a paragraph(0) or a given passage(1)? ");
	scanf("%d",&flag);
	if (flag==0){
		printf("Input the range of characters in ASCII: ");
		scanf("%d %d",&startpos,&endpos); 
		cout << "Loading the paragraph...\n";
		freopen("Passage.txt","r",stdin);
		cnt1=52; 
    	char x; i=0;
    	while (scanf("%c",&x)!=EOF) inp[i++]=x;
    	
    	for (i=0;i<strlen(inp);i++){ //1 letter
    		if (!Judge(inp[i])) continue;
    		for (j=0;j<cnt1;j++)
    			if (inp[i]==w1[j].s[0]) {w1[j].cnt++; break;}
    		if (j==cnt1) {w1[cnt1].s[0]=inp[i]; w1[cnt1++].cnt++;}
    	}
    	for (i=0;i<strlen(inp)-1;i++){ //2 letters
    		if (!Judge(inp[i]) || !Judge(inp[i+1])) continue;
    		for (j=0;j<cnt2;j++)
    			if (inp[i]+inp[i+1]==w2[j].s[0]+w2[j].s[1]) {w2[j].cnt++; break;}
    		if (j==cnt2) {w2[cnt2].s[0]=inp[i]; w2[cnt2].s[1]=inp[i+1]; w2[cnt2++].cnt++;}
    	}
    	for (i=0;i<strlen(inp)-2;i++){ //3 letters
    		if (!Judge(inp[i]) || !Judge(inp[i+1]) || !Judge(inp[i+2])) continue;
    		for (j=0;j<cnt3;j++)
    			if (inp[i]+inp[i+1]+inp[i+2]==w3[j].s[0]+w3[j].s[2]+w3[j].s[2]) {w3[j].cnt++; break;}
    		if (j==cnt3) {w3[cnt3].s[0]=inp[i]; w3[cnt3].s[1]=inp[i+1]; w3[cnt3].s[2]=inp[i]+2; w3[cnt3++].cnt++;}
    	}
    	
    	//calculate the probability
    	for (i=0;i<cnt1;i++)
    		if (w1[i].cnt!=0) w1[i].prob=w1[i].cnt*1.0/strlen(inp);
    		else w1[i].prob=0;
    	for (i=0;i<cnt2;i++)
    		if (w2[i].cnt!=0) w2[i].prob=w2[i].cnt*1.0/(strlen(inp)-1);
    		else w2[i].prob=0;
    	for (i=0;i<cnt3;i++)
    		if (w3[i].cnt!=0) w3[i].prob=w3[i].cnt*1.0/(strlen(inp)-2);
    		else w3[i].prob=0;
    		
    	//print the results
    	printf("1-character words:\n");
    	for (i=0;i<cnt1;i++)
    		if (w1[i].cnt!=0) printf("%s %.6f\n",w1[i].s,w1[i].prob);
    	printf("2-character words:\n");
    	for (i=0;i<cnt2;i++)
    		if (w2[i].cnt!=0) printf("%s %.6f\n",w2[i].s,w2[i].prob);
    	printf("3-character words:\n");
    	for (i=0;i<cnt3;i++)
    		if (w3[i].cnt!=0) printf("%s %.6f\n",w3[i].s,w3[i].prob);
    }
    else{
    	freopen("Pride and Prejudice.txt","r",stdin);
    	char x; i=0;
    	while (scanf("%c",&x)!=EOF) inp[i++]=x;
    	totlen=0;
    	for (i=0;inp[i];i++){
    		if (inp[i]>='A' && inp[i]<='Z') inp[i]+=32;
    		if (inp[i]>='a' && inp[i]<='z'){
    			totlen+=1;
    			w1[inp[i]-'a'].cnt++;
    		}
    	}
    	for (i=0;i<26;i++){
    		w1[i].prob=w1[i].cnt*1.0/totlen;
    		printf("%s %.6f\n",w1[i].s,w1[i].prob);	
    	}
    	
    	printf("------------------------------\nAfter Encoding:\n");
    	for (i=0;i<26;i++) w1[i].prob=w1[i].cnt=0;
    	char code[20]="vigenerecode",inp0[N];
    	int l=0;
    	j=0;
    	for (i=0;inp[i];i++){
    		if (inp[i]>='a' && inp[i]<='z'){
    			int tmp=int(inp[i])+int(code[j]);
    			j=(j+1)%12; tmp=tmp%26+97;
				inp0[l++]=char(tmp); 
    			w1[inp0[l-1]-'a'].cnt++;
    		}
    	}
    	for (i=0;i<26;i++){
    		w1[i].prob=w1[i].cnt*1.0/l;
    		printf("%s %.6f\n",w1[i].s,w1[i].prob);	
    	}
    	
    	
    	//Kasiski
    	int kasis_dict[50000],s=0,codelen=-1; //space limit; k=3
    	for (j=0;j<17576;j++){
    		kasis_dict[0]=0; 
	    	for (i=0;i<l-2;i++){
	    		s=(inp0[i]-'a')*676+(inp0[i+1]-'a')*26+inp0[i+2]-'a'; //map: aaa->000
	    		if (s!=j) continue;
	    		kasis_dict[0]++;
	    		kasis_dict[kasis_dict[0]]=i;
	    	}
			if (kasis_dict[0]>=200){
				int acc=0;
		    	for (i=1;i<kasis_dict[0];i++){
		    		if ((kasis_dict[i+1]-kasis_dict[i])%12==0)
		    			acc++;
		    		if (codelen==-1) codelen=kasis_dict[i+1]-kasis_dict[i];
		    		else codelen=gcd(codelen,kasis_dict[i+1]-kasis_dict[i]);
				}
				printf("Word: %c%c%c  ",'a'+(j/676),'a'+(j%676)/26,'a'+j%26);
				printf("Number of intervals: %d  Available intervals: %d  Accuracy: %.2f\n",kasis_dict[0]-1,acc,acc*1.0/(kasis_dict[0]-1));
				//accuracy: the length of the key is divided by the interval of words 
			}
		}
		printf("Using Kasiski method to estimate the code length: %d\n",codelen);
		
	}
    return 0;
}
