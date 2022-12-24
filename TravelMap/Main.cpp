#include"Graph.h"
#include"User.h"
void myprint(ALGraph G) {
	for (int i = 0; i < G.n; i++) {
		printf("%2d.%-6s:%-10s->", G.Spots[i].Symbol, G.Spots[i].Name, G.Spots[i].Introduction);
		for (AdjSpotNodeP p = G.Spots[i].FirstSpot; p; p = p->NextSpot) {
			printf("sym%-2d:dis=%-3d->", p->SpotSymbol, p->Distence);
		}
		printf("\n");
	}
}
void CreatVexAndArc(SpotNode* spots, ArcInfo* arcs) {
	spots[0].Name = "crrrw"; spots[0].Introduction = "ssw";
	spots[1].Name = "a"; spots[1].Introduction = "s";
	spots[2].Name = "bcd"; spots[2].Introduction = "ss";
	spots[3].Name = "cw"; spots[3].Introduction = "sw";
	arcs[0].w = 0; arcs[0].v = 1; arcs[0].info = 18; arcs[0].level = 20;
	arcs[1].w = 0; arcs[1].v = 2; arcs[1].info = 10; arcs[1].level = 80;
	arcs[2].w = 0; arcs[2].v = 3; arcs[2].info = 6; arcs[2].level = 120;
	arcs[3].w = 1; arcs[3].v = 2; arcs[3].info = 100; arcs[2].level = 120;
	arcs[4].w = 1; arcs[4].v = 3; arcs[4].info = 9; arcs[2].level = 120;
	arcs[5].w = 2; arcs[5].v = 3; arcs[5].info = 50; arcs[2].level = 120;


}
int main() {
	initgraph(1200, 800);
	puts("Success");
	ALGraph G;
	SpotNode spots[4];
	for (int i = 0; i < 4; i++) {
		spots[i].Symbol = i;
	}
	ArcInfo arcs[10];
	CreatVexAndArc(spots, arcs);
	CreatUDGGraph(G, spots, 4, arcs, 6);
	myprint(G);
	int cnt = 4;
	Dijskra* dij;
	AddSpot(G);
	myprint(G);
	while (cnt--) {
		//SpotIntroduce(G);
		puts("--");
		int start,des;
		start = MyInpuSymbolNum("�������·�����");
		des = MyInpuSymbolNum("�������·�յ���");
		if (des >= G.n || start >= G.n || start < 0 || des < 0) {
			printf("Ŀ�ĵػ�����ر�ų��������%d��С��0,����ʧ��\n", G.n - 1);
		}
		else {
			for (int i = 0; i < G.n; i++)G.tags[i] = 0;
			//SuitableRoad(G, start, des);
			/*for (int i = 0; i < G.n; i++) {
				G.tags[i] = 0;
			}*/
			int* path = (int*)calloc(G.n, sizeof(int));
			int len = 0, sum = 0;
			
			//FindAllRoad(G, start, des, path, len, sum);
			ShortestRoad(G, start, dij);
			//FindAppealingRoad(G);
			char road[500] = "\0";
			char t[50] = "\0";
			OutputRoad(G, dij, des, road);
			sprintf(t,"\nLevel=%d\n", dij[des].info);
			strcat(road, t);
			MessageBox(GetHWnd(), road, "���ҽ��", MB_OK);

		}
	}
	
	return 0;
}