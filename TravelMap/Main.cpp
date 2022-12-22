#include"Graph.h"
void myprint(ALGraph G) {
	for (int i = 0; i < G.n; i++) {
		printf("%2d.%-6s:%-10s->", G.Spots[i].Symbol, G.Spots[i].Name, G.Spots[i].Introduction);
		for (AdjSpotNodeP p = G.Spots[i].FirstSpot; p; p = p->NextSpot) {
			printf("sym%-2d:dis=%-3d,level=%-3d->", p->SpotSymbol, p->Distence, p->Level);
		}
		printf("\n");
	}
}
int main() {
	puts("Success");
	ALGraph G;
	SpotNode spots[4];
	for (int i = 0; i < 3; i++) {
		spots[i].Symbol = i;
	}
	ArcInfo arcs[10];
	spots[1].Name = "a"; spots[1].Introduction = "s"; 
	spots[2].Name = "bcd"; spots[2].Introduction = "ss";
	spots[0].Name = "crrrw"; spots[0].Introduction = "sssw";
	arcs[0].w = 1; arcs[0].v = 0; arcs[0].info = 55; arcs[0].level = 20;
	arcs[1].w = 1; arcs[1].v = 2; arcs[1].info = 15; arcs[1].level = 80;
	arcs[2].w = 0; arcs[2].v = 2; arcs[2].info = 5; arcs[2].level = 120;
	CreatUDGGraph(G, spots, 3, arcs, 2+1);
	myprint(G);
	//AddSpot(G);
	puts("\nadd");
	myprint(G);
	PrintSymbol(G);
	int cnt = 10;
	Dijskra* dij;
	//AddSpot(G);
	while (cnt--) {

		puts("--");
		int start,des;
		scanf("%d %d", &start,&des);
		if (des >= G.n || start >= G.n || start < 0 || des < 0) {
			printf("目的地或出发地编号超出最大编号%d或小于0,查找失败\n", G.n - 1);
		}
		else {
			SuitableRoad(G, start, des);
			/*for (int i = 0; i < G.n; i++) {
				G.tags[i] = 0;
			}*/
			//int* path = (int*)calloc(G.e , sizeof(int));
			//int* Currentpath = (int*)calloc(G.e, sizeof(int));
			//int len = 0, MaxLevel = 0;
			//MostAppealingRoad(G,start,des,Currentpath,path,0,len,0,MaxLevel);
			//printf("最美路线指数为%d:\n", MaxLevel);
			//for (int i = 0; i < len; i++) {
				//printf("%d:%s->", G.Spots[path[i]].Symbol, G.Spots[path[i]].Name);
			//}
			//FindAllRoad(G, start, des, path, len, sum);
			/*ShortestRoad(G, start, dij);
			MostAppealingRoad(G, start, dij);
			OutputRoad(G, dij, des);
			printf("\nLevel=%d\n", dij[des].info);*/

		}
	}
	
	return 0;
}