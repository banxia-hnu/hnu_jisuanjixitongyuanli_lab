#include "cachelab.h" 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#define MAX_NUM 0x3f3f3f3f
int misses, hits, evictions;
typedef struct	//<一个数据块>	B=2^b个数据
{
	int valid;		//有效位    	
	int tag;		//标志位		
	int LruNumber;	//LRU算法需要位	
}Line;			//cache line 行

typedef struct	//<一行>	E个数据块	E*B个数据
{
	Line *lines;	//一组中的所有行
}Set;			//cache set 组

typedef struct	//<一整块缓存区>	S=2^s列	S*E*B个数据
{
	int set_num;	//组数			(S=2^s)
	int line_num;	//每行的组数	(E)
	Set *sets;		//整个高速缓存块cache，多组
}Sim_Cache;		//cache

//帮助信息打印函数
void printHelpMenu()
{
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>");
 	printf("\n\nOptions:\n");
 	printf("    -h         Print this help message.\n");
 	printf("    -v         Optional verbose flag.\n");
 	printf("    -s <num>   Number of set index bits.\n");
 	printf("    -E <num>   Number of lines per set.\n");
	printf("    -b <num>   Number of block offset bits.\n");
	printf("    -t <file>  Trace file.\n\n");
	printf("Examples:\n");
	printf("linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
	printf("linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
//检查命令行输入是否合法函数
void checkOptarg(char *curOptarg)
{
	if(curOptarg[0]=='-')
	{
		printf("./csim	: Missing required command line argument\n");
		printHelpMenu();
		exit(0);
	}
}
//解析输入函数
int get_Opt(int argc, char **argv, int *s, int *E, int* b,
			char *tracefileName, int *isVerbose)
{
	int opt;
	for(;((opt=getopt(argc,argv,"hvs:E:b:t:"))!=-1);)
	{
    	switch(opt)
		{
			case 'h':
				printHelpMenu();
				exit(0);
			case 'v':
				*isVerbose=1;break;
			case 's':
				checkOptarg(optarg);
				*s=atol(optarg);break;
			case 'E':
				checkOptarg(optarg);
				*E=atol(optarg);break;
			case 'b':
				checkOptarg(optarg);
				*b=atol(optarg);break;
			case 't':
				checkOptarg(optarg);
				strcpy(tracefileName, optarg);break;
			default:
				printf("invalid option -- '%c'\n", opt);
				printHelpMenu();
				exit(0);
         }
    }
	return 1;
}
//获取组数行数，分配组内存，行内存，清零有效位和Lru计数值
void init_SimCache(int s, int E, int b, Sim_Cache *cache)
{
	cache -> set_num = 2<<s;
	cache -> line_num = E;
	cache -> sets = (Set *)malloc(cache -> set_num * sizeof(Set));
	for(int i = 0; i < cache -> set_num; i++)
	{
		cache -> sets[i].lines = (Line *)malloc(E * sizeof(Line));
		for(int j = 0; j < cache -> line_num; j++)
		{
			cache -> sets[i].lines[j].valid = 0;
			cache -> sets[i].lines[j].tag = 0;
			cache -> sets[i].lines[j].LruNumber = 0;
		}
	}
}
//更新LruNumber, hit的话位最大打MAX_NUM,其他行的Lru均减1
void updataLruNumber(Sim_Cache *sim_cache, int setBits, int hitIndex)
{
	sim_cache ->sets[setBits].lines[hitIndex].LruNumber = MAX_NUM;
	for(int j = 0; j < sim_cache -> line_num; j++)
		for(j = 0; j < sim_cache -> line_num; j++)
			if(j != hitIndex)
				sim_cache -> sets[setBits].lines[j].LruNumber--;

}
//查找某组中当前最小的LruNumber行，作为牺牲行
int findMinLruNumber(Sim_Cache *sim_cache, int setBits)
{
	int minIndex, minLru = MAX_NUM;
	for(int i = 0; i < sim_cache -> line_num; i++)
	{
		if(sim_cache -> sets[setBits].lines[i].LruNumber <= minLru)
		{
			minIndex = i;
			minLru = sim_cache -> sets[setBits].lines[i].LruNumber;
		}
	}
	return minIndex;
}
//判断是否命中
int isMiss(Sim_Cache *sim_cache, int setBits, int tagBits)
{
	int Miss = 1;
	for(int i = 0; i < sim_cache -> line_num; i++)
	{
		if((sim_cache -> sets[setBits].lines[i].valid == 1)&&
		   (sim_cache -> sets[setBits].lines[i].tag == tagBits))
		{
			Miss = 0;
			updataLruNumber(sim_cache, setBits, i);
		}
	}
	return Miss;
}
//更新高速缓存数据
int updateCache(Sim_Cache *sim_cache, int setBits, int tagBits)
{
	int i, full = 1;
	for(i = 0; i < sim_cache -> line_num; i++)
	{
		if(sim_cache -> sets[setBits].lines[i].valid == 0)
		{
			full = 0;	break;
		}
	}
	if(full == 0)	//第i个有空位
	{
		sim_cache -> sets[setBits].lines[i].valid = 1;
		sim_cache -> sets[setBits].lines[i].tag = tagBits;
		updataLruNumber(sim_cache, setBits, i);
	}
	else			//满了，驱逐
	{
		int evictionIndex = findMinLruNumber(sim_cache, setBits);
		sim_cache -> sets[setBits].lines[evictionIndex].valid = 1;
		sim_cache -> sets[setBits].lines[evictionIndex].tag = tagBits;
		updataLruNumber(sim_cache, setBits, evictionIndex);
	}
	return full;
}
//trace文件 L操作
void loadData(Sim_Cache *sim_cache, int setBits,int tagBits,int isVerbose)
{
	if(isMiss(sim_cache, setBits, tagBits) == 1)			//未命中
	{
		misses++;
		if(isVerbose == 1)
			printf("miss ");
		if(updateCache(sim_cache, setBits, tagBits) == 1)	//驱逐
		{
			evictions++;
			if(isVerbose == 1)
				printf("eviction ");
		}
	}
	else													//命中
	{
		hits++;
		if(isVerbose == 1)
			printf("hit ");
	}

}
//trace文件 S操作
void storeData(Sim_Cache *sim_cache, int setBits,int tagBits,int isVerbose)
{
	loadData(sim_cache, setBits, tagBits, isVerbose);
}
//trace文件 M操作
void modifyData(Sim_Cache *sim_cache,int setBits,int tagBits,int isVerbose)
{
	loadData(sim_cache, setBits, tagBits, isVerbose);
	storeData(sim_cache, setBits, tagBits, isVerbose);
}
//获取地址中的组索引
int getSet(int addr, int s, int b)
{
	return (addr>>b)&((1<<s) - 1);
}
//获取地址中的标记位
int getTag(int addr, int s, int b)
{
	return addr>>(b+s);
}
int main(int argc,char **argv)
{
	misses = 0; hits = 0; evictions = 0;
	int s = 0, E = 0, b = 0, isVerbose = 0;
	char *tracefileName = (char *)malloc(100);
	Sim_Cache cache;
	get_Opt(argc, argv, &s, &E, &b,tracefileName, &isVerbose);
	init_SimCache(s,E,b,&cache);
	FILE *Path = fopen(tracefileName,"r");
	if(s<=0||E<=0||b<=0)
	{
		printf("Please check the range about 's' 'E' or 'b'\n");
		printf("%d %d %d\n",s,E,b);
		exit(0);
	}
	if(Path==NULL)
	{
		printf("%s : No such file or directory\n",tracefileName);
		exit(0);
	}
	int addr, number;
	char c;
	char opt[3];
	for(;(fscanf(Path,"%s %x%c%d",opt,&addr,&c,&number)!=EOF);)
	{
		if(opt[0]=='I')
			continue;
		int setBits = getSet(addr, s, b);
		int tagBits = getTag(addr, s, b);
		//printf("setBits is %x, tagBits is %x\n", setBits, tagBits);
		
		if(isVerbose == 1)
			printf("%s %x,%d ", opt, addr, number);
		if(opt[0] == 'L')
			loadData(&cache, setBits, tagBits, isVerbose);
		else if(opt[0] == 'S')
			storeData(&cache, setBits, tagBits, isVerbose);
		else if(opt[0] == 'M')
			modifyData(&cache,setBits, tagBits, isVerbose);
		else
			printf("Error!!!\n");

		if(isVerbose == 1)
			printf("\n");
	}
	printSummary(hits,misses,evictions);
	
	//free_SimCache(&cache);
    return 0;
}
