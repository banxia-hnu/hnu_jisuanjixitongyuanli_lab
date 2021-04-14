/*文件名:     csim_part2.c        */
/*班级：      计科1601       */
/*作者：      吾尔肯·祖哈吉       */
/*学号：      201608010131       */
/*内容：      partA 2       */
#include "cachelab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

int miss,hit,evictions;
char input[50];
long change2number(char *ss){
	int len=strlen(ss);
	long ret=0;
	for(int i=0;i<len;i++){
		ret=ret*10+(ss[i]-'0');
	}
	return ret;
}

int hexnumber(char c){
	if(c>='0'&&c<='9') return c-'0';
	if(c>='a'&&c<='f') return c-'a'+10;
	if(c>='A'&&c<='F') return c-'A'+10;
	return 0;
}

void op_load(int index,int ttab,
			int S,int E, int *vaild,
			long *tab,int *recenttime,int count){

	for(int i=index*E;i<(index+1)*E;i++){
		if(vaild[i]&&tab[i]==ttab){
			printf("%s hit\n",input);
			hit++;
			recenttime[i]=count;
			return;
		}
	}
	for(int i=index*E;i<(index+1)*E;i++){
		if(!vaild[i]){
			vaild[i]=1;
			tab[i]=ttab;
			printf("%s miss\n",input);
			miss++;
			recenttime[i]=count;
			return;
		}
	}
	int flag=0;
	int maxn=1e9;
	for(int i=index*E;i<(index+1)*E;i++){
		if(recenttime[i]<maxn){
			maxn=recenttime[i];
			flag=i;
		}
	}
	printf("%s miss eviction\n",input);
	miss++;evictions++;
	tab[flag]=ttab;
	recenttime[flag]=count;
}


int main(int argc, char* argv[]){
	int s,S,E,b;
	FILE *fp;
	for(int i=1;i<argc;){
		//printf("%s",argv[i]);
		if(argv[i][0]=='-'){
			if(argv[i][1] == 's'){
				i++;
				s = change2number(argv[i]);
				S = (1<<s);
				i++;
			}
			else if(argv[i][1] == 'E'){
				i++;
				E = change2number(argv[i]);
				i++;
			}
			else if(argv[i][1] == 'b'){
				i++;
				b = change2number(argv[i]);
				i++;
			}
			else if(argv[i][1] == 't'){
				i++;
				fp = fopen(argv[i],"r");
				i++;
			}
		}
	}
	miss=0,hit=0,evictions=0;
	//printf("%d %d %d\n",s,E,b);
	int *vaild = (int *)malloc(sizeof(int)*S*E);
	long *tab = (long *)malloc(sizeof(long)*S*E);
	int *recenttime = (int *)malloc(sizeof(int)*S*E);
	for(int i=0;i<S*E;i++){
		vaild[i] = 0;
		tab[i] = 0;
		recenttime[i] = 0;
	}
	int count=0;
	while(fgets(input, 100,fp)){
		//printf("%s",input);
		count++;
		int len=strlen(input)-1;
		input[len]=0;
		int op;
		int dou=0;
		int size=0;
		long address = 0;
		for(int i=0;i<len;i++){
			if(input[i]=='I')  op=0;
			else if(input[i]=='L') op=1;
			else if(input[i] == 'M') op=2;
			else if(input[i] == 'S') op=3;
			else if(input[i]==' ') continue;
			else if(input[i] == ',') dou=1;
			else{
				if(!dou){
					address = address*16+hexnumber(input[i]);
				}
				else size = size*10+(input[i]-'0');
			}
		}
		int offset = 0;
		int indexgroup = 0;
		for(int i=0;i<b;i++){
			offset=offset*2+(address&1);
			address>>=1;
		}
		for(int i=0;i<s;i++){
			indexgroup = indexgroup*2 + (address&1);
			address>>=1;
		}
		if(op==1||op==3) op_load(indexgroup, address, S, E, vaild, tab, recenttime,count);
		if(op==2){
			op_load(indexgroup, address, S, E, vaild, tab, recenttime,count);
			printf("hit\n");
			hit++;
		}
	}
    printSummary(hit, miss, evictions);
    return 0;
}
