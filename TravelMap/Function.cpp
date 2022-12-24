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
        MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ�������,�������˵�", "��ʾ", MB_OK | MB_ICONERROR);
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
    InputBox(name, 20, "��������Ҫ��ӵĵص������");
    G.Spots[n].Name = name;
    char *information = (char*)calloc(100,sizeof(char));
    if (information == NULL) {
        return OVERFLOW;
    }
    InputBox(information, 20, "��������Ҫ��ӵĵص�Ľ���");
    G.Spots[n].Introduction = information;
    G.n++;  
    return OK;
}

Status AddEdge(ALGraph& G)
{
    AdjSpotNodeP p,q;
    int i, j;
    char buf[1000] = "\0";
    sprintf(buf,"��������Ҫ��ӵ������ߵĶ�Ӧ�ı�ţ���ΧΪ0��%d", G.n - 1);
    MessageBox(GetHWnd(), buf, "��ʾ", MB_OK);
    i = MyInpuSymbolNum("�����������");
    j = MyInpuSymbolNum("�������յ���");
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ�������,�������˵�", "��ʾ", MB_OK | MB_ICONERROR);
        return false;
    }
    if (IsDirectConnected(G, i, j)) {
        MessageBox(GetHWnd(), "�������Ѿ����ڣ����ʧ��", "��ʾ", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    p = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
    if (!p){
        return OVERFLOW;
    }   
    int info=-1, level=-1;
    
    while (1) {
        info = MyInpuSymbolNum("������һ��������ʾ����·�̶ȣ���λ100��");
        if (info <= 0) {
            MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ������֣�������������", "��ʾ", MB_OK | MB_ICONERROR);
        }
        else {
            break;
        }
    }
    
    while (1) {
        level = MyInpuSymbolNum("������һ������ʾ����·�羰�����̶ȣ����0������10��)");
        if (level < 0) {
            MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ�������,���ַ�Χ0-10", "��ʾ", MB_OK | MB_ICONERROR);
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
    sprintf(buf, "��������Ҫɾ���������ߵĶ�Ӧ�ı�ţ���ΧΪ0��%d,ʹ�ÿո���� ", G.n - 1);
    MessageBox(GetHWnd(), buf, "��ʾ", MB_OK);
    i = MyInpuSymbolNum("�������·�����");
    j = MyInpuSymbolNum("�������·�յ���");
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ�������,�������˵�", "��ʾ", MB_OK | MB_ICONERROR);
        return false;
    }
    if (!IsDirectConnected(G, i, j)) {
        MessageBox(GetHWnd(), "�����߲����ڣ�", "��ʾ", MB_OK | MB_ICONERROR);
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
    operate = MyInpuSymbolNum("�������Ӧ�Ĳ�����\n1���޸ľ�������  2���޸ľ����� \n");
    while (operate != 1 && operate != 2) {
        MessageBox(GetHWnd(), "��������������������", "��ʾ", MB_OK | MB_ICONERROR);
        operate = MyInpuSymbolNum("��������ȷ�Ķ�Ӧ�Ĳ�����");
    }
    num = MyInpuSymbolNum("��������Ҫ�޸ĵľ����ı��");
    while (num < 0 || num >= G.n) {
        MessageBox(GetHWnd(), "������Ų��Ϸ�������������", "��ʾ", MB_OK | MB_ICONERROR);
        num = MyInpuSymbolNum("��������Ҫ�޸ĵľ����ı��");
    }
    if (1 == operate) {           //�޸�����      
        Clearbuffer();
        InputBox(New, 100, "�������޸ĺ�ľ�������");
        G.Spots[num].Name = New;
    }
    else {             //�޸ļ��
        Clearbuffer();
        InputBox(New, 100, "�������޸ĺ�ľ�����");
        G.Spots[num].Introduction = New;
    }
    MessageBox(GetHWnd(), "�޸������", "��ʾ", MB_OK);
    return OK;
}

Status ReviseEdge(ALGraph& G)
{
    int operate, NewData, SpotOne, SpotTwo;
    AdjSpotNodeP p;
    operate = MyInpuSymbolNum("�������Ӧ�Ĳ�����\n1Ϊ�޸ĵ�·����  2Ϊ�޸ĵ�·����ָ��");
    while (operate != 1 && operate != 2) {
        MessageBox(GetHWnd(), "��������������������", "��ʾ", MB_OK | MB_ICONERROR);
        operate = MyInpuSymbolNum("�������Ӧ�Ĳ�����\n1Ϊ�޸ĵ�·����  2Ϊ�޸ĵ�·����ָ��");
    }
    char buf[1000] = "\0";
    sprintf(buf, "��������Ҫ�޸ĵ�·��������Ķ�Ӧ�ı�ţ���ΧΪ0��%d", G.n - 1);
    MessageBox(GetHWnd(), buf, "��ʾ", MB_OK);
    SpotOne = MyInpuSymbolNum("�������·���");
    SpotTwo = MyInpuSymbolNum("�������·���");
    while (SpotOne < 0 || SpotOne >= G.n || SpotTwo < 0||SpotTwo>=G.n) {
        MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ�������", "��ʾ", MB_OK | MB_ICONERROR);
        SpotOne = MyInpuSymbolNum("�����������");
        SpotTwo = MyInpuSymbolNum("�������·���");
    }
    if (IsDirectConnected(G, SpotOne, SpotTwo) == false) {
        MessageBox(GetHWnd(), "ͼ�в����ڸ���·���޷��޸ģ�", "��ʾ", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    if (1 == operate) {           //�޸ĳ���    
        NewData = MyInpuSymbolNum("�������޸ĺ�ĵ�·�ĳ���");
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
    else {             //�޸�����ָ��
        while (true) {
            NewData = MyInpuSymbolNum("������һ������ʾ�޸ĺ�ĵ�·�������̶ȣ����0���10��");
            if (NewData >= 0 && NewData <= 10) {
                break;
            }
            else {
                MessageBox(GetHWnd(), "���ֵ���Ϸ�������������", "����", MB_OK | MB_ICONERROR);
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
    MessageBox(GetHWnd(), "�޸�����ɣ��������˵�", "��ʾ", MB_OK);
    return OK;
}

Status SpotIntroduce(ALGraph G)
{
    int k = MyInpuSymbolNum("��������Ҫ�鿴�ľ�����");
    if (k < 0 ||k >= G.n) {
        MessageBox(GetHWnd(), "������Ĳ���һ���Ϸ�������,�������˵�", "��ʾ", MB_OK | MB_ICONERROR);
        return false;
    }
    char* Information = (char*)calloc(1000, sizeof(char));
    if (Information == NULL) {
        return OVERFLOW;
    }
    sprintf(Information, "�����ţ�%d\n�������ƣ�%s\n������ܣ�%s", G.Spots[k].Symbol, G.Spots[k].Name, G.Spots[k].Introduction);
    MessageBox(GetHWnd(), Information, "��������", MB_OK);
    MessageBox(GetHWnd(), "���ν��ܵ���Ϊֹ����л�ι�", "��ʾ", MB_OK);
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
    int start = MyInpuSymbolNum("��������㾰�����");
    int des = MyInpuSymbolNum("�������յ㾰�����");
    MostAppealingRoad(G, start, des, Currentpath, path, 0, len, 0, MaxLevel);

}

Status MostAppealingRoad(ALGraph G, int start,int des,int *CurrentPath,int*path,int CurrentLen,int &len,int CurrentLevel,int &MaxLevel)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        MessageBox(GetHWnd(), "������ĳ����ػ�Ŀ�ĵر�����󣬲���ʧ��", "��ʾ", MB_OK | MB_ICONERROR);
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
    sprintf(Road,"����·��ָ��Ϊ%d:\n", MaxLevel);
    for (int i = 0; i < length; i++) {
        sprintf(Temp,"%d:%s->", G.Spots[path[i]].Symbol, G.Spots[path[i]].Name);
        strcat(Road, Temp);
    }
    MessageBox(GetHWnd(), Road, "���ҽ��", MB_OK);
    free(Road);
    free(Temp);
}

bool FindAllRoad(ALGraph G, int start, int des, int *path,int len, int& sum)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        MessageBox(GetHWnd(), "������ĳ����ػ�Ŀ�ĵر�����󣬲���ʧ��", "��ʾ", MB_OK | MB_ICONERROR);
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
        sprintf(Road,"·��%d:", ++sum);
        for (int i = 0; i < len; i++) {
            sprintf(Temp,"%d:%s->", G.Spots[path[i]].Symbol, G.Spots[path[i]].Name);
            strcat(Road, Temp);
        }
        MessageBox(GetHWnd(), Road, "���ҽ��", MB_OK);
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
        MessageBox(GetHWnd(), "�������Ŀ�ĵر�����󣬲���ʧ��", "��ʾ", MB_OK | MB_ICONERROR);
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
    if (left == CntOfNum) {//�������Ԫ����������һ���غϣ�˵���������һ��Ԫ�� �����        
        for (int j = 0; j <= CntOfNum; j++) {
            result[CntOfPerm].path[j] = num[j];
        }
        CntOfPerm = CntOfPerm + 1;
    }
    else {     
        for (int i = left; i <= CntOfNum; i++) {//�Դ��±��left�����ұߵ�Ԫ�ؽ���ȫ����
            Swap(left, i, num);
            Perm(left + 1, CntOfNum, num, CntOfPerm, result);
            Swap(left, i, num);
        }
    }
    return OK;
}

Status SuitableRoad(ALGraph G, int start, int des)
{
    int cnt = MyInpuSymbolNum("��������׼��;���ľ���ĸ���");
    if (cnt <= 0) {
        MessageBox(GetHWnd(), "����;��һ����", "��ʾ", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    if (cnt > G.n-2) {
        MessageBox(GetHWnd(), ";���ĵ�ĸ������ھ�������", "��ʾ", MB_OK | MB_ICONERROR);
        return ERROR;
    }
    int* num = (int*)calloc(cnt, sizeof(int));
    for (int i = 0; i < cnt; i++) {
        num[i] = MyInpuSymbolNum("�������·���");
    }
    bool Reach = IsReach(G, start, des);
    for (int i = 0; i < cnt; i++) {
        if (IsReach(G, start, num[i]) == false||Reach == false) {
            MessageBox(GetHWnd(), "��������֮�䲻��ͨ������ʧ��", "��ʾ", MB_OK | MB_ICONERROR);
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
    int Time = 0;//��ʾ����ȫ�����еĴ���
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
    sprintf(Road,"�ҵ�����Ҫ���·�����£�\n��1�Σ�");
    OutputRoad(G, dij[start], per[number].path[0], Road);  
    for (int i = 0; i < cnt - 1; i++) {
        sprintf(Temp,"\n��%d�Σ�", 2 + i);
        strcat(Road, Temp);
        OutputRoad(G, dij[per[number].path[i]], per[number].path[i + 1], Road);
    }
    sprintf(Temp,"\n��%d�Σ�", cnt + 1);
    strcat(Road, Temp);
    OutputRoad(G, dij[per[number].path[cnt - 1]], des, Road);
    sprintf(Temp,"\n��Ӧ��ȨֵΪ%d\n", per[number].info);
    strcat(Road, Temp);
    MessageBox(GetHWnd(), Road, "���ҽ��", MB_OK);
    free(Temp);
    free(Road);
    return OK;
}

void Clearbuffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

