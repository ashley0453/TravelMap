#include "Graph.h"

int FirstAdjSpot(ALGraph G, int k, AdjSpotNodeP& p){
    if (k < 0 || k >= G.n) {
        return -1;
    }
    p = G.Spots[k].FirstSpot;
    if (p != NULL) {
        return p->SpotSymbol;
    }
    else return -1;
}

int NextAdjSpot(ALGraph G, int k, AdjSpotNodeP& p)
{
    if (k < 0 || k >= G.n) {
        return -1;
    }
    if (NULL == p) {
        return -1;
    }
    p = p->NextSpot;
    if (p != NULL) {
        return p->SpotSymbol;
    }
    else return -1;
}

void PrintSymbol(ALGraph G)
{
    puts("-------------景区标号列表-------------");
    int cnt = 3;
    for (int i = 0; i < G.n; i++) {
        printf("编号%2d:景点:%-10s；", G.Spots[i].Symbol, G.Spots[i].Name);
        if (i!=0&&i % 2 == 1) {
            printf("\n");
        }
    }
    puts("\n------------------------------------");
}

bool IsConnected(ALGraph G, int i, int j)
{
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        printf("输入数据不合法，返回主菜单\n");
        return false;
    }
    AdjSpotNodeP p = G.Spots[i].FirstSpot;
    while (p) {
        if (p->SpotSymbol == j) {
            return true;
        }
        p = p->NextSpot;
    }
    return false;
}

Status CreatUDGGraph(ALGraph& G, SpotNode* Spots, int n, ArcInfo* arce, int e)
{
    AdjSpotNodeP p, q;
    G.n = n;
    G.e = e;
    G.kind = UDN;
    G.tags = (int*)calloc(n, sizeof(int));
    G.Spots = (SpotNode*)calloc(n, sizeof(SpotNode));
    if (G.tags == NULL || G.Spots == NULL) {
        return OVERFLOW;
    }
    for (int i = 0; i < n; i++) {
        G.tags[i] = UNVISITED;
        G.Spots[i] = Spots[i];
        G.Spots[i].FirstSpot = NULL;
    }
    for (int j = 0; j < e; j++) {
        int SymbolOne = arce[j].v, SymbolTwo = arce[j].w;
        if (SymbolOne<0 || SymbolOne>n || SymbolTwo < 0 || SymbolTwo>n) {
            continue;
        }
        p = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
        if (!p) {
            return OVERFLOW;
        }
        p->SpotSymbol = SymbolTwo;
        p->NextSpot = G.Spots[SymbolOne].FirstSpot;  
        p->Distence = arce[j].info;
        p->Level = arce[j].level;
        G.Spots[SymbolOne].FirstSpot = p;
        q = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
        if (!q) {
            return OVERFLOW;
        }
        q->SpotSymbol = SymbolOne;
        q->NextSpot = G.Spots[SymbolTwo].FirstSpot;
        q->Distence = arce[j].info;
        q->Level = arce[j].level;
        G.Spots[SymbolTwo].FirstSpot = q;
        
    }
    return OK;
}

Status AddSpot(ALGraph& G)
{
    int n = G.n;
    G.Spots = (SpotNode*)realloc(G.Spots,(G.n + 1)* sizeof(SpotNode));
    G.Spots[n].Symbol = n;
    G.Spots[n].FirstSpot = NULL;
    printf("请输入您要添加的地点的名称");
    char *name = (char*)calloc(20,sizeof(char));
    gets_s(name, 20);
    G.Spots[n].Name = name;
    //strncpy(G.Spots[n].Name, name, 10);
    printf("请输入地点的介绍： ");
    char *information = (char*)calloc(100,sizeof(char));
    gets_s(information, 100);
    G.Spots[n].Introduction = information;
    G.n++;  
    return OK;
}

Status AddEdge(ALGraph& G)
{
    AdjSpotNodeP p,q;
    int i, j;
    printf("请输入想要添加的两条边的对应的编号，范围为0到%d,使用空格隔开 ", G.n - 1);
    scanf("%d %d", &i, &j);
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        printf("输入数据不合法，返回主菜单\n");
        return false;
    }
    if (IsConnected(G, i, j)) {
        printf("已经存在该条边\n");
        return ERROR;
    }
    p = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
    if (!p){
        return OVERFLOW;
    }   
    int info=-1, level=-1;
    printf("请输入一整数表示该条路的长度（单位100米）\n");
    while (1) {
        scanf("%d", &info);
        if (info <= 0) {
            printf("输入数据不合法，长度必须为正数");
        }
        else {
            break;
        }
    }
    printf("请输入一整数表示该条路风景美丽程度（最低0分满分10分）\n");
    while (1) {
        scanf("%d", &level);
        if (level < 0) {
            printf("输入数据不合法，最低0分满分10分");
        }
        else {
            break;
        }
    }
    p->SpotSymbol = j;
    p->NextSpot = G.Spots[i].FirstSpot;
    G.Spots[i].FirstSpot = p;
    q = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
    if (!q) {
        return ERROR;
    }
    q->SpotSymbol = i;
    q->NextSpot = G.Spots[j].FirstSpot;
    G.Spots[j].FirstSpot = q;
    q->Distence=p->Distence = info;
    q->Level = p->Level = level;
    return OK;
}

Status RemoveEdge(ALGraph& G)
{
    int i, j;
    printf("请输入想要添加的两条边的对应的编号，范围为0到%d,使用空格隔开 ", G.n - 1);
    scanf("%d %d", &i, &j);   
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        printf("输入数据不合法，返回主菜单\n");
        return false;
    }
    if (!IsConnected(G, i, j)) {
        printf("不存在该条边\n");
        return ERROR;
    }
    AdjSpotNodeP p = NULL, q = p;
    for (q = G.Spots[i].FirstSpot, p = q; q; p = q, p = p->NextSpot) {
        if (q->SpotSymbol == j) {
            if (q == G.Spots[i].FirstSpot) {
                G.Spots[i].FirstSpot = q->NextSpot;
            }
            else {
                p->NextSpot = q->NextSpot;
            }
            G.e--;
            free(q);
            break;
        }
    }
    for (q = G.Spots[j].FirstSpot, p = q; q; p = q, q = q->NextSpot) {
        if (q->SpotSymbol == i) {
            if (q == G.Spots[j].FirstSpot) {
                G.Spots[j].FirstSpot = q->NextSpot;
            }
            else {
                p->NextSpot = q->NextSpot;
            }
            G.e--;
            free(q);
            break;
        }
    }
    return OK;
}

Status ReviseSpot(ALGraph& G)
{
    int operate, num;
    char* New;
    New = (char*)malloc(100 * sizeof(char));
    printf("1、修改景点名称  2、修改景点简介 \n");
    scanf("%d", &operate);
    while (operate != 1 && operate != 2) {
        printf("输入操作符有误，请重新输入\n");
        scanf("%d", &operate);
    }
    printf("景点号是多少？\n");
    scanf("%d", &num);
    while (num < 0 || num >= G.n) {
        printf("输入景区编号有误，请重新输入\n");
        scanf("%d", &num);
    }
    if (1 == operate) {           //修改名称      
        printf("请输入修改后的景点名称\n");
        Clearbuffer();
        gets_s(New, 100);
        G.Spots[num].Name = New;
    }
    else {             //修改简介
        printf("请输入修改后的景点简介\n");
        Clearbuffer();
        gets_s(New, 100);
        G.Spots[num].Introduction = New;
    }
    printf("修改完毕\n");
    return OK;
}

Status ReviseEdge(ALGraph& G)
{
    int operate, NewData, SpotOne, SpotTwo;
    AdjSpotNodeP p;
    printf("1、修改道路距离  2、修改道路美丽指数 \n");
    scanf("%d", &operate);
    while (operate != 1 && operate != 2) {
        printf("输入操作符有误，请重新输入\n");
        scanf("%d", &operate);
    }
    printf("想要修改的道路连接的两个景点的编号是多少（中间使用空格隔开）？\n");
    scanf("%d %d", &SpotOne, &SpotTwo);
    while (SpotOne < 0 || SpotOne >= G.n || SpotTwo < 0||SpotTwo>=G.n) {
        printf("输入景区编号有误，请重新输入\n");
        scanf("%d %d", &SpotOne, &SpotTwo);
    }
    if (IsConnected(G, SpotOne, SpotTwo) == false) {
        printf("两个景区之间不存在直接相连的道路，返回主菜单\n");
        return ERROR;
    }
    if (1 == operate) {           //修改长度    
        printf("请输入修改后的道路的长度\n");
        scanf("%d", &NewData);
        for (p = G.Spots[SpotOne].FirstSpot; p; p = p->NextSpot) {
            if (p->SpotSymbol == SpotTwo) {
                p->Distence = NewData;
                break;
            }
        }
        for (p = G.Spots[SpotTwo].FirstSpot; p; p = p->NextSpot) {
            if (p->SpotSymbol == SpotOne) {
                p->Distence = NewData;
                break;
            }
        }
    }
    else {             //修改美丽指数
        printf("请输入修改后的道路沿路的美丽值\n");
        scanf("%d", &NewData);
        for (p = G.Spots[SpotOne].FirstSpot; p; p = p->NextSpot) {
            if (p->SpotSymbol == SpotTwo) {
                p->Level = NewData;
                break;
            }
        }
        for (p = G.Spots[SpotTwo].FirstSpot; p; p = p->NextSpot) {
            if (p->SpotSymbol == SpotOne) {
                p->Level = NewData;
                break;
            }
        }
    }
    printf("修改完毕\n");
    return OK;
}

Status SpotIntroduce(ALGraph G, int k)
{
    if (k < 0 ||k >= G.n) {
        printf("输入数据不合法，返回主菜单\n");
        return false;
    }
    printf("景点编号：%d\n",G.Spots[k].Symbol);
    printf("景点名称：%s\n", G.Spots[k].Name);
    printf("景点介绍：%s\n", G.Spots[k].Introduction);
    printf("关于本景点的介绍到此结束，感谢您的参观\n");
    return OK;
}

void Clearbuffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

