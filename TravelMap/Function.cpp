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
    puts("-------------��������б�-------------");
    int cnt = 3;
    for (int i = 0; i < G.n; i++) {
        printf("���%2d:����:%-10s��", G.Spots[i].Symbol, G.Spots[i].Name);
        if (i!=0&&i % 2 == 1) {
            printf("\n");
        }
    }
    puts("\n------------------------------------");
}

bool IsDirectConnected(ALGraph G, int i, int j)
{
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        printf("�������ݲ��Ϸ����������˵�\n");
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
    printf("��������Ҫ��ӵĵص������");
    char *name = (char*)calloc(20,sizeof(char));
    gets_s(name, 20);
    G.Spots[n].Name = name;
    //strncpy(G.Spots[n].Name, name, 10);
    printf("������ص�Ľ��ܣ� ");
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
    printf("��������Ҫ��ӵ������ߵĶ�Ӧ�ı�ţ���ΧΪ0��%d,ʹ�ÿո���� ", G.n - 1);
    scanf("%d %d", &i, &j);
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        printf("�������ݲ��Ϸ����������˵�\n");
        return false;
    }
    if (IsDirectConnected(G, i, j)) {
        printf("�Ѿ����ڸ�����\n");
        return ERROR;
    }
    p = (AdjSpotNodeP)calloc(1, sizeof(AdjSpotNode));
    if (!p){
        return OVERFLOW;
    }   
    int info=-1, level=-1;
    printf("������һ������ʾ����·�ĳ��ȣ���λ100�ף�\n");
    while (1) {
        scanf("%d", &info);
        if (info <= 0) {
            printf("�������ݲ��Ϸ������ȱ���Ϊ����");
        }
        else {
            break;
        }
    }
    printf("������һ������ʾ����·�羰�����̶ȣ����0������10�֣�\n");
    while (1) {
        scanf("%d", &level);
        if (level < 0) {
            printf("�������ݲ��Ϸ������0������10��");
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
    printf("��������Ҫ��ӵ������ߵĶ�Ӧ�ı�ţ���ΧΪ0��%d,ʹ�ÿո���� ", G.n - 1);
    scanf("%d %d", &i, &j);   
    if (i < 0 || j < 0 || i >= G.n || j >= G.n) {
        printf("�������ݲ��Ϸ����������˵�\n");
        return false;
    }
    if (!IsDirectConnected(G, i, j)) {
        printf("�����ڸ�����\n");
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
    printf("1���޸ľ�������  2���޸ľ����� \n");
    scanf("%d", &operate);
    while (operate != 1 && operate != 2) {
        printf("�����������������������\n");
        scanf("%d", &operate);
    }
    printf("������Ƕ��٣�\n");
    scanf("%d", &num);
    while (num < 0 || num >= G.n) {
        printf("���뾰�������������������\n");
        scanf("%d", &num);
    }
    if (1 == operate) {           //�޸�����      
        printf("�������޸ĺ�ľ�������\n");
        Clearbuffer();
        gets_s(New, 100);
        G.Spots[num].Name = New;
    }
    else {             //�޸ļ��
        printf("�������޸ĺ�ľ�����\n");
        Clearbuffer();
        gets_s(New, 100);
        G.Spots[num].Introduction = New;
    }
    printf("�޸����\n");
    return OK;
}

Status ReviseEdge(ALGraph& G)
{
    int operate, NewData, SpotOne, SpotTwo;
    AdjSpotNodeP p;
    printf("1���޸ĵ�·����  2���޸ĵ�·����ָ�� \n");
    scanf("%d", &operate);
    while (operate != 1 && operate != 2) {
        printf("�����������������������\n");
        scanf("%d", &operate);
    }
    printf("��Ҫ�޸ĵĵ�·���ӵ���������ı���Ƕ��٣��м�ʹ�ÿո��������\n");
    scanf("%d %d", &SpotOne, &SpotTwo);
    while (SpotOne < 0 || SpotOne >= G.n || SpotTwo < 0||SpotTwo>=G.n) {
        printf("���뾰�������������������\n");
        scanf("%d %d", &SpotOne, &SpotTwo);
    }
    if (IsDirectConnected(G, SpotOne, SpotTwo) == false) {
        printf("��������֮�䲻����ֱ�������ĵ�·���������˵�\n");
        return ERROR;
    }
    if (1 == operate) {           //�޸ĳ���    
        printf("�������޸ĺ�ĵ�·�ĳ���\n");
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
    else {             //�޸�����ָ��
        printf("�������޸ĺ�ĵ�·��·������ֵ\n");
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
    printf("�޸����\n");
    return OK;
}

Status SpotIntroduce(ALGraph G, int k)
{
    if (k < 0 ||k >= G.n) {
        printf("�������ݲ��Ϸ����������˵�\n");
        return false;
    }
    printf("�����ţ�%d\n",G.Spots[k].Symbol);
    printf("�������ƣ�%s\n", G.Spots[k].Name);
    printf("������ܣ�%s\n", G.Spots[k].Introduction);
    printf("���ڱ�����Ľ��ܵ��˽�������л���Ĳι�\n");
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

Status MostAppealingRoad(ALGraph G, int Start, Dijskra*& dij)
{
    return OK;
}

bool FindAllRoad(ALGraph G, int start, int des, int *path,int len, int& sum)
{
    if (des >= G.n || start >= G.n || start < 0 || des < 0) {
        printf("Ŀ�ĵػ�����ر�ų��������%d��С��0,����ʧ��\n", G.n - 1);
        return ERROR;
    }
    G.tags[start] = VISITED;
    path[len] = start;
    len++;
    if (start == des && len >= 0) {
        printf("·��%d:", ++sum);
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
        printf("Ŀ�ĵصı�ų��������%d,���ʧ��\n", G.n - 1);
        return;
    }
    if (Destinatin == -1)return;
    OutputRoad(G, dij, dij[Destinatin].PreNode);
    printf("%-2d:%-6s->", G.Spots[Destinatin].Symbol, G.Spots[Destinatin].Name);
}

void Clearbuffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

