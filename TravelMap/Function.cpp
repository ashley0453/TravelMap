#include "Graph.h"
#include"User.h"
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


bool IsDirectConnected(ALGraph G, int i, int j)
{
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        MessageBox(GetHWnd(), "您输入的不是一个合法的数字,返回主菜单", "提示", MB_OK | MB_ICONERROR);
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

bool IsReach(ALGraph G, int i, int j)
{
    bool found = IsReachable(G, i, j);
    SetTags(G);
    return found;
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

void SetTags(ALGraph& G)
{
    for (int i = 0; i < G.n; i++) {
        G.tags[i] = 0;
    }
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
    char *name = (char*)calloc(20,sizeof(char));
    if (name == NULL) {
        return OVERFLOW;
    }
    InputBox(name, 20, "请输入您要添加的地点的名称");
    G.Spots[n].Name = name;
    char *information = (char*)calloc(100,sizeof(char));
    if (information == NULL) {
        return OVERFLOW;
    }
    InputBox(information, 20, "请输入您要添加的地点的介绍");
    G.Spots[n].Introduction = information;
    G.n++;  
    return OK;
}

Status AddEdge(ALGraph& G)
{
    AdjSpotNodeP p,q;
    int i, j;
    char buf[1000] = "\0";
    sprintf(buf,"请输入想要添加的两条边的对应的编号，范围为0到%d", G.n - 1);
    MessageBox(GetHWnd(), buf, "提示", MB_OK);
    i = MyInpuSymbolNum("请输入起点编号");
    j = MyInpuSymbolNum("请输入终点编号");
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        MessageBox(GetHWnd(), "您输入的不是一个合法的数字,返回主菜单", "提示", MB_OK | MB_ICONERROR);
        return false;
    }
    if (IsDirectConnected(G, i, j)) {
        MessageBox(GetHWnd(), "该条边已经存在，添加失败", "提示", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    p = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
    if (!p){
        return OVERFLOW;
    }   
    int info=-1, level=-1;
    
    while (1) {
        info = MyInpuSymbolNum("请输入一正整数表示该条路程度，单位100米");
        if (info <= 0) {
            MessageBox(GetHWnd(), "您输入的不是一个合法的数字，请输入正整数", "提示", MB_OK | MB_ICONERROR);
        }
        else {
            break;
        }
    }
    
    while (1) {
        level = MyInpuSymbolNum("请输入一整数表示该条路风景美丽程度（最低0分满分10分)");
        if (level < 0) {
            MessageBox(GetHWnd(), "您输入的不是一个合法的数字,数字范围0-10", "提示", MB_OK | MB_ICONERROR);
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
    char buf[1000] = "\0";
    sprintf(buf, "请输入想要删除的两条边的对应的编号，范围为0到%d,使用空格隔开 ", G.n - 1);
    MessageBox(GetHWnd(), buf, "提示", MB_OK);
    i = MyInpuSymbolNum("请输入道路起点编号");
    j = MyInpuSymbolNum("请输入道路终点编号");
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        MessageBox(GetHWnd(), "您输入的不是一个合法的数字,返回主菜单", "提示", MB_OK | MB_ICONERROR);
        return false;
    }
    if (!IsDirectConnected(G, i, j)) {
        MessageBox(GetHWnd(), "该条边不存在！", "提示", MB_OK | MB_ICONERROR);
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
    operate = MyInpuSymbolNum("请输入对应的操作码\n1、修改景点名称  2、修改景点简介 \n");
    while (operate != 1 && operate != 2) {
        MessageBox(GetHWnd(), "操作符有误，请重新输入", "提示", MB_OK | MB_ICONERROR);
        operate = MyInpuSymbolNum("请输入正确的对应的操作码");
    }
    num = MyInpuSymbolNum("请输入想要修改的景区的编号");
    while (num < 0 || num >= G.n) {
        MessageBox(GetHWnd(), "景点符号不合法，请重新输入", "提示", MB_OK | MB_ICONERROR);
        num = MyInpuSymbolNum("请输入想要修改的景区的编号");
    }
    if (1 == operate) {           //修改名称      
        Clearbuffer();
        InputBox(New, 100, "请输入修改后的景点名称");
        G.Spots[num].Name = New;
    }
    else {             //修改简介
        Clearbuffer();
        InputBox(New, 100, "请输入修改后的景点简介");
        G.Spots[num].Introduction = New;
    }
    MessageBox(GetHWnd(), "修改已完成", "提示", MB_OK);
    return OK;
}

Status ReviseEdge(ALGraph& G)
{
    int operate, NewData, SpotOne, SpotTwo;
    AdjSpotNodeP p;
    operate = MyInpuSymbolNum("请输入对应的操作码\n1为修改道路距离  2为修改道路美丽指数");
    while (operate != 1 && operate != 2) {
        MessageBox(GetHWnd(), "操作符有误，请重新输入", "提示", MB_OK | MB_ICONERROR);
        operate = MyInpuSymbolNum("请输入对应的操作码\n1为修改道路距离  2为修改道路美丽指数");
    }
    char buf[1000] = "\0";
    sprintf(buf, "请输入想要修改的路的两个点的对应的编号，范围为0到%d", G.n - 1);
    MessageBox(GetHWnd(), buf, "提示", MB_OK);
    SpotOne = MyInpuSymbolNum("请输入道路编号");
    SpotTwo = MyInpuSymbolNum("请输入道路编号");
    while (SpotOne < 0 || SpotOne >= G.n || SpotTwo < 0||SpotTwo>=G.n) {
        MessageBox(GetHWnd(), "您输入的不是一个合法的数字", "提示", MB_OK | MB_ICONERROR);
        SpotOne = MyInpuSymbolNum("请输入起点编号");
        SpotTwo = MyInpuSymbolNum("请输入道路编号");
    }
    if (IsDirectConnected(G, SpotOne, SpotTwo) == false) {
        MessageBox(GetHWnd(), "图中不存在该条路，无法修改！", "提示", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    if (1 == operate) {           //修改长度    
        NewData = MyInpuSymbolNum("请输入修改后的道路的长度");
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
        while (true) {
            NewData = MyInpuSymbolNum("请输入一整数表示修改后的道路的美丽程度（最低0最大10）");
            if (NewData >= 0 && NewData <= 10) {
                break;
            }
            else {
                MessageBox(GetHWnd(), "打分值不合法，请重新输入", "警告", MB_OK | MB_ICONERROR);
            }
        }      
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
    MessageBox(GetHWnd(), "修改已完成，返回主菜单", "提示", MB_OK);
    return OK;
}

Status SpotIntroduce(ALGraph G)
{
    int k = MyInpuSymbolNum("请输入想要查看的景点编号");
    if (k < 0 ||k >= G.n) {
        MessageBox(GetHWnd(), "您输入的不是一个合法的数字,返回主菜单", "提示", MB_OK | MB_ICONERROR);
        return false;
    }
    char* Information = (char*)calloc(1000, sizeof(char));
    if (Information == NULL) {
        return OVERFLOW;
    }
    sprintf(Information, "景点编号：%d\n景点名称：%s\n景点介绍：%s", G.Spots[k].Symbol, G.Spots[k].Name, G.Spots[k].Introduction);
    MessageBox(GetHWnd(), Information, "景区介绍", MB_OK);
    MessageBox(GetHWnd(), "本次介绍到此为止，感谢参观", "提示", MB_OK);
    free(Information);
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

void FindAppealingRoad(ALGraph G)
{
    int* path = (int*)calloc(G.e, sizeof(int));
    int* Currentpath = (int*)calloc(G.e, sizeof(int));
    int len = 0, MaxLevel = 0;
    int start = MyInpuSymbolNum("请输入起点景区编号");
    int des = MyInpuSymbolNum("请输入终点景区编号");
    MostAppealingRoad(G, start, des, Currentpath, path, 0, len, 0, MaxLevel);

}

Status MostAppealingRoad(ALGraph G, int start,int des,int *CurrentPath,int*path,int CurrentLen,int &len,int CurrentLevel,int &MaxLevel)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        MessageBox(GetHWnd(), "您输入的出发地或目的地编号有误，查找失败", "提示", MB_OK | MB_ICONERROR);
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

void PrintAppealingRoad(ALGraph G, int* path, int length, int MaxLevel)
{
    char* Road = (char*)calloc(2000, sizeof(char));
    char* Temp = (char*)calloc(50, sizeof(char));
    if (Road == NULL || Temp == NULL) {
        return;
    }
    sprintf(Road,"最美路线指数为%d:\n", MaxLevel);
    for (int i = 0; i < length; i++) {
        sprintf(Temp,"%d:%s->", G.Spots[path[i]].Symbol, G.Spots[path[i]].Name);
        strcat(Road, Temp);
    }
    MessageBox(GetHWnd(), Road, "查找结果", MB_OK);
    free(Road);
    free(Temp);
}

bool FindAllRoad(ALGraph G, int start, int des, int *path,int len, int& sum)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        MessageBox(GetHWnd(), "您输入的出发地或目的地编号有误，查找失败", "提示", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    G.tags[start] = VISITED;
    path[len] = start;
    len++;
    if (start == des && len >= 0) {
        char* Road = (char*)calloc(2000, sizeof(char));
        char* Temp = (char*)calloc(50, sizeof(char));
        if (Road == NULL || Temp == NULL) {
            return OVERFLOW;
        }
        sprintf(Road,"路径%d:", ++sum);
        for (int i = 0; i < len; i++) {
            sprintf(Temp,"%d:%s->", G.Spots[path[i]].Symbol, G.Spots[path[i]].Name);
            strcat(Road, Temp);
        }
        MessageBox(GetHWnd(), Road, "查找结果", MB_OK);
        G.tags[start] = UNVISITED;
        free(Temp);
        free(Road);
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

void OutputRoad(ALGraph G, Dijskra* dij, int Destinatin,char* Road)
{
    if (Destinatin > G.n) {
        MessageBox(GetHWnd(), "您输入的目的地编号有误，查找失败", "提示", MB_OK | MB_ICONERROR);
        return;
    }
    if (Destinatin == -1)return;
    OutputRoad(G, dij, dij[Destinatin].PreNode,Road);
    char* CurrentSpot = (char*)calloc(40, sizeof(char));
    if (CurrentSpot == NULL) {
        return;
    }
    sprintf(CurrentSpot,"%-2d:%-6s->", G.Spots[Destinatin].Symbol, G.Spots[Destinatin].Name);
    strcat(Road, CurrentSpot);
    free(CurrentSpot);
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
    if (left == CntOfNum) {//当最左边元素序号与最后一个重合，说明到了最后一个元素 ，输出        
        for (int j = 0; j <= CntOfNum; j++) {
            result[CntOfPerm].path[j] = num[j];
        }
        CntOfPerm = CntOfPerm + 1;
    }
    else {     
        for (int i = left; i <= CntOfNum; i++) {//对从下标从left到最右边的元素进行全排列
            Swap(left, i, num);
            Perm(left + 1, CntOfNum, num, CntOfPerm, result);
            Swap(left, i, num);
        }
    }
    return OK;
}

Status SuitableRoad(ALGraph G, int start, int des)
{
    int cnt = MyInpuSymbolNum("请输入您准备途径的景点的个数");
    if (cnt <= 0) {
        MessageBox(GetHWnd(), "至少途径一个点", "提示", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    if (cnt > G.n-2) {
        MessageBox(GetHWnd(), "途径的点的个数大于景点总数", "提示", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    int* num = (int*)calloc(cnt, sizeof(int));
    for (int i = 0; i < cnt; i++) {
        num[i] = MyInpuSymbolNum("请输入道路编号");
    }
    bool Reach = IsReach(G, start, des);
    for (int i = 0; i < cnt; i++) {
        if (IsReach(G, start, num[i]) == false||Reach == false) {
            MessageBox(GetHWnd(), "存在两点之间不联通，查找失败", "提示", MB_OK | MB_ICONERROR);
            return ERROR;
        }
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
    char* Road = (char*)calloc(2000, sizeof(char));
    char* Temp = (char*)calloc(50, sizeof(char));
    if (Road == NULL || Temp == NULL) {
        return OVERFLOW;
    }
    sprintf(Road,"找到符合要求的路线如下：\n第1段：");
    OutputRoad(G, dij[start], per[number].path[0], Road);  
    for (int i = 0; i < cnt - 1; i++) {
        sprintf(Temp,"\n第%d段：", 2 + i);
        strcat(Road, Temp);
        OutputRoad(G, dij[per[number].path[i]], per[number].path[i + 1], Road);
    }
    sprintf(Temp,"\n第%d段：", cnt + 1);
    strcat(Road, Temp);
    OutputRoad(G, dij[per[number].path[cnt - 1]], des, Road);
    sprintf(Temp,"\n对应的权值为%d\n", per[number].info);
    strcat(Road, Temp);
    MessageBox(GetHWnd(), Road, "查找结果", MB_OK);
    free(Temp);
    free(Road);
    return OK;
}

void Clearbuffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

