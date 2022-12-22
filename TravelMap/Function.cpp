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

bool IsDirectConnected(ALGraph G, int i, int j)
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

bool IsReachable(ALGraph G, int i, int j)
{
    AdjSpotNodeP p;
    bool Found = false;
    G.tags[i] = VISITED;
    if (i == j) {
        return true;
    }
    for (p = G.Spots[i].FirstSpot; p && Found == false; p = p->NextSpot) {
        if (G.tags[p->SpotSymbol]==UNVISITED) {
            Found = IsReachable(G, p->SpotSymbol, j);
        }
    }
    return Found;
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
    if (IsDirectConnected(G, i, j)) {
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
    if (!IsDirectConnected(G, i, j)) {
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
    if (IsDirectConnected(G, SpotOne, SpotTwo) == false) {
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

Status ShortestRoad(ALGraph G, int Start, Dijskra*& dij)
{
    dij = (Dijskra*)calloc(G.n, sizeof(Dijskra));
    for (int i = 0; i < G.n; i++) {
        G.tags[i] = UNSELECTED;
        dij[i].info = INFINITY;
    }
    for (AdjSpotNodeP p = G.Spots[Start].FirstSpot; p; p = p->NextSpot) {
        dij[p->SpotSymbol].PreNode = Start;
        dij[p->SpotSymbol].info = p->Distence;
    }
    G.tags[Start] = SELECTED;
    dij[Start].PreNode = -1;
    dij[Start].info = 0;
    for (int i = 0; i < G.n; i++) {
        int MinDistence = INFINITY, SelectedNode = 0;
        for (int j = 0; j < G.n; j++) {
            if (G.tags[j] == UNSELECTED && dij[j].info < MinDistence) {
                SelectedNode = j;
                MinDistence = dij[j].info;
            }
        }
        G.tags[SelectedNode] = SELECTED;
        for (AdjSpotNodeP p = G.Spots[SelectedNode].FirstSpot; p; p = p->NextSpot) {
            if (G.tags[p->SpotSymbol] == UNSELECTED && dij[SelectedNode].info + p->Distence < dij[p->SpotSymbol].info) {
                dij[p->SpotSymbol].info = dij[SelectedNode].info + p->Distence;
                dij[p->SpotSymbol].PreNode = SelectedNode;
            }
        }
    }
    return OK;
}

Status MostAppealingRoad(ALGraph G, int start,int des,int *CurrentPath,int*path,int CurrentLen,int &len,int CurrentLevel,int &MaxLevel)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        printf("目的地或出发地编号超出最大编号%d或小于0,查找失败\n", G.n - 1);
        return ERROR;
    }
    G.tags[start] = VISITED;
    path[CurrentLen] = start;
    CurrentLen++;
    if (start == des && CurrentLen >= 0&&MaxLevel<CurrentLevel) {    
        MaxLevel = CurrentLevel;
        path = CurrentPath;
        len = CurrentLen;
        G.tags[start] = UNVISITED;
        return true;
    }
    for (AdjSpotNodeP p = G.Spots[start].FirstSpot; p; p = p->NextSpot) {
        if (G.tags[p->SpotSymbol] == UNVISITED) {
            MostAppealingRoad(G, p->SpotSymbol, des,CurrentPath,path,CurrentLen,len,CurrentLevel+p->Level,MaxLevel);
        }
    }
    G.tags[start] = UNVISITED;
    CurrentLen--;
    return false;
}

bool FindAllRoad(ALGraph G, int start, int des, int *path,int len, int& sum)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        printf("目的地或出发地编号超出最大编号%d或小于0,查找失败\n", G.n - 1);
        return ERROR;
    }
    G.tags[start] = VISITED;
    path[len] = start;
    len++;
    if (start == des && len >= 0) {
        printf("路径%d:", ++sum);
        for (int i = 0; i < len; i++) {
            printf("%d:%s->", G.Spots[path[i]].Symbol, G.Spots[path[i]].Name);

        }
        G.tags[start] = UNVISITED;
        printf("\n");
        return true;
    }
    for (AdjSpotNodeP p = G.Spots[start].FirstSpot; p; p = p->NextSpot) {
        if (G.tags[p->SpotSymbol] == UNVISITED) {
            FindAllRoad(G, p->SpotSymbol, des, path, len, sum);
        }
    }
    G.tags[start] = UNVISITED;
    len--;
    return false;
}

void OutputRoad(ALGraph G, Dijskra* dij, int Destinatin)
{
    if (Destinatin > G.n) {
        printf("目的地的编号超出最大编号%d,输出失败\n", G.n - 1);
        return;
    }
    if (Destinatin == -1)return;
    OutputRoad(G, dij, dij[Destinatin].PreNode);
    printf("%-2d:%-6s->", G.Spots[Destinatin].Symbol, G.Spots[Destinatin].Name);
}

long long int Fact(int i)
{
    if (i == 0||i==1) {
        return 1;
    }
    else {
        return i * Fact(i - 1);
    }
}

void Swap(int x, int y, int* num)
{
    int t = num[x];
    num[x] = num[y];
    num[y] = t;
}

Status Perm(int left, int CntOfNum, int* num, int& CntOfPerm, Permutations*& result)
{
    if (left == CntOfNum) {//当p==q说明到了最后一个元素 ，输出        
        for (int j = 0; j <= CntOfNum; j++) {
            result[CntOfPerm].path[j] = num[j];
        }
        CntOfPerm = CntOfPerm + 1;
        printf("\n");
    }
    else {     
        for (int i = left; i <= CntOfNum; i++) {//对从下标从p到q的元素进行全排列
            Swap(left, i, num);
            Perm(left + 1, CntOfNum, num, CntOfPerm, result);
            Swap(left, i, num);
        }
    }
    return OK;
}

Status SuitableRoad(ALGraph G, int start, int des)
{
    int cnt;
    printf("请输入您准备途径的景点的个数\n");
    scanf("%d", &cnt);
    int* num = (int*)calloc(cnt, sizeof(int));
    for (int i = 0; i < cnt; i++) {
        printf("第%d个数字，请输入想途径的景点的编号 ", i + 1);
        scanf("%d", &num[i]);
    }
    long long int FactResult = Fact(cnt);
    Permutations* per = (Permutations*)calloc(FactResult, sizeof(Permutations));
    if (per == NULL) {
        return OVERFLOW;
    }
    for (int i = 0; i < FactResult; i++) {
        per[i].path = (int*)calloc(cnt, sizeof(int));
        if (NULL == per[i].path) {
            return OVERFLOW;
        }
    }
    int Time = 0;//表示符合全部排列的次数
    Perm(0, cnt - 1, num, Time, per);
    Dijskra** dij = (Dijskra**)calloc(G.n,sizeof(Dijskra*));
    if (dij == NULL) {
        return OVERFLOW;
    }
    ShortestRoad(G, start, dij[start]);
    for (int i = 0; i < cnt; i++) {
        ShortestRoad(G, num[i], dij[num[i]]);
    }
    for (int i = 0; i < FactResult; i++) {
        per[i].info += dij[start][per[i].path[0]].info;
        for (int j = 0; j < cnt-1; j++) {
            per[i].info += dij[per[i].path[j]][per[i].path[j+1]].info;
        }
        per[i].info += dij[per[i].path[cnt-1]][des].info;
    }
    int MinDistence = INFINITY, number = 0;
    for (int i = 0; i < FactResult; i++) {
        if (MinDistence > per[i].info) {
            MinDistence = per[i].info;
            number = i;
        }
    }
    printf("找到符合要求的路线如下：\n");
    printf("第1段：");
    OutputRoad(G, dij[start], per[number].path[0]);
    for (int i = 0; i < cnt - 1; i++) {
        printf("\n第%d段：", 2 + i);
        OutputRoad(G, dij[per[number].path[i]], per[number].path[i + 1]);
        printf("\n");
    }
    printf("\n第%d段：", cnt + 1);
    OutputRoad(G, dij[per[number].path[cnt - 1]], des);
    printf("\n对应的权值为%d\n", per[number].info);
    return OK;
}

void Clearbuffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

