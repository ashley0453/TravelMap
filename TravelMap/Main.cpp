#include"Graph.h"
void myprint(ALGraph G) {
	for (int i = 0; i < G.n; i++) {
		printf("%d.%s:%s->", G.Spots[i].Symbol, G.Spots[i].Name, G.Spots[i].Introduction);
		for (AdjSpotNodeP p = G.Spots[i].FirstSpot; p; p = p->NextSpot) {
			printf("sym%d:dis=%d,level=%d->", p->SpotSymbol, p->Distence, p->Level);
		}
		printf("\n");
	}
}
int main() {
	puts("Success");
	ALGraph G;
	SpotNode spots[3];
	for (int i = 0; i < 3; i++) {
		spots[i].Symbol = i;
	}
	ArcInfo arcs[2];
	spots[1].Name = "a"; spots[1].Introduction = "s"; 
	spots[2].Name = "b"; spots[2].Introduction = "ss";
	spots[0].Name = "c"; spots[0].Introduction = "sssw";
	arcs[0].w = 1; arcs[0].v = 0; arcs[0].info = 5; arcs[0].level = 100;
	arcs[1].w = 1; arcs[1].v = 2; arcs[1].info = 15; arcs[1].level = 1200;
	CreatUDGGraph(G, spots, 3, arcs, 2);
	myprint(G);
	//AddSpot(G);
	puts("\nadd");
	myprint(G);
	
	int cnt = 3;
	for (int i = -1; i < 5; i++) {
		SpotIntroduce(G, i);
	}

	return 0;
}