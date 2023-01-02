#include"Graph.h"
#include"User.h"
char* Title = "欢迎使用校园导览";
char* Menu[] = { 
	"查景区信息","添加新景点","修建新道路","删除旧道路","改景点信息","改道路信息","两点寻找最短路","两点寻找最美路","两点查找所有路","多点寻找最短路","退出" 
};

int main() {
	initgraph(1200, 600);
	int width = textwidth(Title);
	int high = textheight(Title);
	settextstyle(high, 0, _T("宋体"));
	ALGraph G;
	SpotNode spots[12];
	ArcInfo arcs[14];
	//Dijskra* dij;
	for (int i = 0; i < 12; i++) {
		spots[i].Symbol = i;
	}
	CreatVexAndArc(spots, arcs);
	CreatUDGGraph(G, spots, 12, arcs, 13);
	DrawUI(G);
	MOUSEMSG msg;
	while (!((GetAsyncKeyState(VK_ESCAPE) & 0x8000))) {
		if (MouseHit) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN || msg.uMsg == WM_LBUTTONDBLCLK) {
				if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 1 + high * 1 - 3 && msg.y <= 20 + 25 * 1 + high * 2 + 3) {
					SpotIntroduce(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 2 + high * 2 - 3 && msg.y <= 20 + 25 * 2 + high * 3 + 3) {
					AddSpot(G);
					cleardevice();
					DrawUI(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 3 + high * 3 - 3 && msg.y <= 20 + 25 * 3 + high * 4 + 3) {
					AddEdge(G);
					cleardevice();
					DrawUI(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 4 + high * 4 - 3 && msg.y <= 20 + 25 * 4 + high * 5 + 3) {
					RemoveEdge(G);
					cleardevice();
					DrawUI(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 5 + high * 5 - 3 && msg.y <= 20 + 25 * 5 + high * 6 + 3) {
					ReviseSpot(G);
					cleardevice();
					DrawUI(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 6 + high * 6 - 3 && msg.y <= 20 + 25 * 6 + high * 7 + 3) {
					ReviseEdge(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 7 + high * 7 - 3 && msg.y <= 20 + 25 * 7 + high * 8 + 3) {
					FindShortRoad(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 8 + high * 8 - 3 && msg.y <= 20 + 25 * 8 + high * 9 + 3) {
					FindAppealingRoad(G);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 9 + high * 9 - 3 && msg.y <= 20 + 25 * 9 + high * 10 + 3) {
					int start = MyInpuSymbolNum("请输入起点编号");
					int des = MyInpuSymbolNum("请输入终点编号");
					int* path = (int*)calloc(G.n, sizeof(int));
					int sum = 0;
					if (des >= G.n || start >= G.n || start < 0 || des < 0 || des == start) {
						MessageBox(GetHWnd(), "您输入的出发地或目的地编号有误，查找失败", "提示", MB_OK | MB_ICONERROR);
						continue;
					}
					SetTags(G);
					FindAllRoad(G, start, des, path, 0, sum);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 10 + high * 10 - 3 && msg.y <= 20 + 25 * 10 + high * 11 + 3) {
					int start = MyInpuSymbolNum("请输入起点编号");
					int des = MyInpuSymbolNum("请输入终点编号");					
					if (des >= G.n || start >= G.n || start < 0 || des < 0 || des == start) {
						MessageBox(GetHWnd(), "您输入的出发地或目的地编号有误，查找失败", "提示", MB_OK | MB_ICONERROR);
						continue;
					}
					SuitableRoad(G, start, des);
				}
				else if (msg.x >= 750 && msg.x <= 850 + width && msg.y >= 20 + 25 * 11 + high * 11 - 3 && msg.y <= 20 + 25 * 11 + high * 12 + 3) {
					MessageBox(GetHWnd(), "感谢您的使用，欢迎下次参观", "再见", MB_OK);
					break;
				}				
			}
		}
	}
	return 0;
}

void DrawUI(ALGraph G) {
	int width = textwidth(Title);
	int high = textheight(Title);
	outtextxy(500, 50, "-------->东");
	char ch[100] = "\0";
	for (int i = 0; i < G.n; i++) {
		sprintf(ch, "%d:%s", G.Spots[i].Symbol, G.Spots[i].Name);
		outtextxy(G.Spots[i].x * 60, G.Spots[i].y * 60, ch);
	}
	for (int i = 0; i < G.n; i++) {
		for (AdjSpotNodeP p = G.Spots[i].FirstSpot; p; p = p->NextSpot) {
			line(G.Spots[i].x * 60 + 30, G.Spots[i].y * 60 - 10, G.Spots[p->SpotSymbol].x * 60 + 30, G.Spots[p->SpotSymbol].y * 60 - 10);
		}
	}
	setcolor(GREEN);
	outtextxy(800, 15, Title);
	setcolor(WHITE);
	line(700, 20 + high, 900 + width, 20 + high);
	for (int i = 1; i <= 11; i++) {
		rectangle(750, 20 + 25 * i + high * i - 3, 850 + width, 20 + 25 * i + high * (i + 1) + 3);
		outtextxy(750 + (100 + width - textwidth(Menu[i - 1])) / 2, 20 + 25 * i + high * i, Menu[i - 1]);
	}
	settextstyle(high*1.2, 0, _T("楷体"));
	outtextxy(900, 580, "计科伏羲-3121005309-孙天一");
	settextstyle(high, 0, _T("宋体"));
}

void CreatVexAndArc(SpotNode* spots, ArcInfo* arcs) {
	spots[0].Name = "生活西区"; spots[0].Introduction = "同学们饮食休息的地方"; spots[0].x = 0; spots[0].y = 3;
	spots[1].Name = "体育馆"; spots[1].Introduction = "强身健体的地方"; spots[1].x = 2; spots[1].y = 3;
	spots[2].Name = "校医院"; spots[2].Introduction = "治病的地方"; spots[2].x = 4; spots[2].y = 0;
	spots[3].Name = "一食堂"; spots[3].Introduction = "菜式多样，品种齐全"; spots[3].x = 4; spots[3].y = 1;
	spots[4].Name = "广工通道"; spots[4].Introduction = "校园中最拥堵的路口"; spots[4].x = 4; spots[4].y = 3;
	spots[5].Name = "教学楼"; spots[5].Introduction = "6栋教学楼，学生进行理论学习的地方"; spots[5].x = 4; spots[5].y = 4;
	spots[6].Name = "图书馆"; spots[6].Introduction = "形状为魔方形，这里是知识的海洋"; spots[6].x = 5; spots[6].y = 4;
	spots[7].Name = "人工湖"; spots[7].Introduction = "休憩，观景的好地方"; spots[7].x = 5; spots[7].y = 6;
	spots[8].Name = "行政楼"; spots[8].Introduction = "各行政部门的办公地点"; spots[8].x = 5; spots[8].y = 8;
	spots[9].Name = "正大门"; spots[9].Introduction = "大柱子竖立在此处彰显了学校特色"; spots[9].x = 5; spots[9].y = 9;
	spots[10].Name = "工学楼"; spots[10].Introduction = "学生进行工科实验学习的地方"; spots[10].x = 6; spots[10].y = 8;
	spots[11].Name = "实验楼"; spots[11].Introduction = "学生进行综合实践的地方"; spots[11].x = 9; spots[11].y = 4;

	arcs[0].w = 0; arcs[0].v = 1; arcs[0].info = 2; arcs[0].level = 5;
	arcs[1].w = 0; arcs[1].v = 8; arcs[1].info = 11; arcs[1].level = 9;
	arcs[2].w = 1; arcs[2].v = 4; arcs[2].info = 2; arcs[2].level = 4;
	arcs[3].w = 2; arcs[3].v = 3; arcs[3].info = 1; arcs[3].level = 3;
	arcs[4].w = 3; arcs[4].v = 4; arcs[4].info = 2; arcs[4].level = 4;
	arcs[5].w = 4; arcs[5].v = 5; arcs[5].info = 1; arcs[5].level = 5;
	arcs[6].w = 5; arcs[6].v = 6; arcs[6].info = 1; arcs[6].level = 6;
	arcs[7].w = 6; arcs[7].v = 7; arcs[7].info = 2; arcs[7].level = 8;
	arcs[8].w = 6; arcs[8].v = 11; arcs[8].info = 4; arcs[8].level = 6;
	arcs[9].w = 7; arcs[9].v = 8; arcs[9].info = 2; arcs[9].level = 7;
	arcs[10].w = 8; arcs[10].v = 10; arcs[10].info = 1; arcs[10].level = 3;
	arcs[11].w = 8; arcs[11].v = 9; arcs[11].info = 1; arcs[11].level = 7;
	arcs[12].w = 3; arcs[12].v = 11; arcs[11].info = 12; arcs[11].level = 7;
}