#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

const int N=5e5;
char str[N],inp[N],key[32];
int l,cl,S[16][16],invS[16][16],s[4][4],s0[4][4],cipher[N];
int table[256],arc_table[256],inverse_table[256],w[4][44];

struct word{
	char ch;
	int cnt; //number of appearances
	double prob; //probability 
}cha[256],char_c[256];

void init_table(){
	int i,j,k,b0[8],b[8],c[8]={1,1,0,0,0,1,1,0},val;
	table[0]=1; 
	for (i=1;i<255;i++){  
	    table[i]=(table[i-1]<<1)^table[i-1];   
	    if (table[i]&0x100) table[i]^=0x11B;
	}  
	for (i=0;i<255;++i)  
	    arc_table[table[i]]=i;   
	for (i=1;i<256;++i)
	    inverse_table[i]=table[(255-arc_table[i])%255];  

	for (i=0;i<16;i++){
		for (j=0;j<16;j++){
			if (i==0 && j==0) 
				val=0;
			else 
				val=inverse_table[i*16+j];
			
			k=0;
			while (val!=0){
				b0[k++]=val%2;
				val/=2;
			}
			while (k<8) b0[k++]=0;
			for (k=0;k<8;k++)
				b[k]=b0[k]^b0[(k+4)%8]^b0[(k+5)%8]^b0[(k+6)%8]^b0[(k+7)%8]^c[k];
			
			val=0; k=7;
			while (b[k]==0) k--;
			for (;k>=0;k--) val=val*2+b[k];
			S[i][j]=val; 
		}
	}
	for (i=0;i<16;i++)
		for (j=0;j<16;j++){
			int ff=0;
			for (int i0=0;i0<16;i0++){
				for (int j0=0;j0<16;j0++)
					if (S[i0][j0]==i*16+j){
						invS[i][j]=i0*16+j0;
						ff=1; break;
					}
				if (ff) break;
			}
		}

}

int mul(int x,int y) {
	if (x*y==0) return 0; //************VERY IMPORTANT!
	return table[(arc_table[x]+arc_table[y])%255];
}
int num(char x){
	if (x>='0' && x<='9') return x-'0';
	else return x-'a'+10;
}
char dechex(int x){
	if (x<=9) return '0'+x;
	else return 'a'+x-10;	
}

void init(){
	int i,j,RC[11]={0,1,2,4,8,16,32,64,128,27,54};
	for (i=0;i<256;i++){ 
		if (i>=97 && i<=122) cha[i].ch=char_c[i].ch=i;
		cha[i].cnt=char_c[i].cnt=0;
	}
	 //e.g. 0f1571c947d9e8590cb7add6af7f6798
	freopen("key.txt","r",stdin);
	scanf("%s",key);
	for (i=0;i<16;i++){
		if (key[2*i]>='0' && key[2*i]<='9') 
			w[i%4][i/4]+=(key[2*i]-'0')*16;
		else 
			w[i%4][i/4]+=(key[2*i]-'a'+10)*16;
		if (key[2*i+1]>='0' && key[2*i+1]<='9') 
			w[i%4][i/4]+=key[2*i+1]-'0';
		else 
			w[i%4][i/4]+=key[2*i+1]-'a'+10;
	}
	for (i=4;i<44;i++){
		if (i%4==0){
			for (j=0;j<4;j++){
				w[0][i]=w[0][i-4]^S[w[1][i-1]/16][w[1][i-1]%16]^RC[i/4];
				w[1][i]=w[1][i-4]^S[w[2][i-1]/16][w[2][i-1]%16];
				w[2][i]=w[2][i-4]^S[w[3][i-1]/16][w[3][i-1]%16];
				w[3][i]=w[3][i-4]^S[w[0][i-1]/16][w[0][i-1]%16];
			}
		}
		else{
			for (j=0;j<4;j++) w[j][i]=w[j][i-1]^w[j][i-4];
		}
	}
}

int main(){		
	int i=0,j,k; char x;
	freopen("Pride and Prejudice.txt","r",stdin);
	while (scanf("%c",&x)!=EOF) inp[i++]=x;
	init_table(); init();
	
	printf("Before Encoding:\n");
	l=0; cl=0;
	for (i=0;inp[i];i++){
		if (inp[i]>='A' && inp[i]<='Z'){
			str[l]=inp[i]+32;
			cha[str[l]].cnt++; l++;
		}
		if (inp[i]>='a' && inp[i]<='z'){
			str[l]=inp[i];
			cha[str[l]].cnt++; l++;
		}
	}

	for (i=97;i<123;i++){
		cha[i].prob=cha[i].cnt*1.0/l;
		printf("%c %.6f\n",cha[i].ch,cha[i].prob);	
	}
	
	//encryption
	printf("------------------------------\nAfter Encoding:\n");
	for (int loc=0;loc<l;){
		for (j=0;j<4;j++){
			for (k=0;k<4;k++){
				if (loc>=l) s[k][j]=0;
				else 
					s[k][j]=str[loc++];
			}
		}

		for (int round=1;round<=10;round++){
			if (round==10){
				for (j=0;j<4;j++){
					s0[0][j]=S[s[0][j]/16][s[0][j]%16]^w[0][j+round*4];
					s0[1][j]=S[s[1][(j+1)%4]/16][s[1][(j+1)%4]%16]^w[1][j+round*4];
					s0[2][j]=S[s[2][(j+2)%4]/16][s[2][(j+2)%4]%16]^w[2][j+round*4];
					s0[3][j]=S[s[3][(j+3)%4]/16][s[3][(j+3)%4]%16]^w[3][j+round*4];
				}
			}
			else{
				if (round==1){
					for (i=0;i<4;i++)
						for (j=0;j<4;j++)
							s[j][i]^=w[j][i];
				}
				for (j=0;j<4;j++){
					s0[0][j]=mul(2,S[s[0][j]/16][s[0][j]%16]) ^ mul(3,S[s[1][(j+1)%4]/16][s[1][(j+1)%4]%16]) \
						   ^mul(1,S[s[2][(j+2)%4]/16][s[2][(j+2)%4]%16]) ^ mul(1,S[s[3][(j+3)%4]/16][s[3][(j+3)%4]%16]) \
						   ^w[0][j+round*4];
					s0[1][j]=mul(1,S[s[0][j]/16][s[0][j]%16]) ^ mul(2,S[s[1][(j+1)%4]/16][s[1][(j+1)%4]%16]) \
						   ^mul(3,S[s[2][(j+2)%4]/16][s[2][(j+2)%4]%16]) ^ mul(1,S[s[3][(j+3)%4]/16][s[3][(j+3)%4]%16]) \
						   ^w[1][j+round*4];
					s0[2][j]=mul(1,S[s[0][j]/16][s[0][j]%16]) ^ mul(1,S[s[1][(j+1)%4]/16][s[1][(j+1)%4]%16]) \
						   ^mul(2,S[s[2][(j+2)%4]/16][s[2][(j+2)%4]%16]) ^ mul(3,S[s[3][(j+3)%4]/16][s[3][(j+3)%4]%16]) \
						   ^w[2][j+round*4];
					s0[3][j]=mul(3,S[s[0][j]/16][s[0][j]%16]) ^ mul(1,S[s[1][(j+1)%4]/16][s[1][(j+1)%4]%16]) \
						   ^mul(1,S[s[2][(j+2)%4]/16][s[2][(j+2)%4]%16]) ^ mul(2,S[s[3][(j+3)%4]/16][s[3][(j+3)%4]%16]) \
						   ^w[3][j+round*4];	
				
				}
			}	
			for (i=0;i<4;i++)
				for (j=0;j<4;j++)
					s[i][j]=s0[i][j];

		}
		for (i=0;i<4;i++){
			for (j=0;j<4;j++){
				cipher[cl++]=s[j][i];
				char_c[s[j][i]].cnt++;
			}
		}
	}

	for (i=0;i<256;i++){
		char_c[i].prob=char_c[i].cnt*1.0/cl;
		if (char_c[i].cnt>0) printf("Code: %d  Count: %d  Probability: %.6f\n",i,char_c[i].cnt,char_c[i].prob);
	}
	printf("The standard average probability: %.6f\n",1.0/256); 
    return 0;
}
