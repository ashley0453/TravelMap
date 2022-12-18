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

bool IsConnected(ALGraph G, int i, int j)
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
    G.Spots[n].Name = (char*)calloc(30, sizeof(char));
    G.Spots[n].Introduction = (char*)calloc(60, sizeof(char));
    printf("��������Ҫ��ӵĵص������");
    char *name = (char*)calloc(20,sizeof(char));
    gets_s(name, 20);
    //G.Spots[n].Name = name;
    strncpy(G.Spots[n].Name, name, 10);
    printf("������ص�Ľ��ܣ� ");
    char *information = (char*)calloc(50,sizeof(char));
    gets_s(information, 50);
    strncpy(G.Spots[n].Introduction, information, 20);
    G.n++;
    free(name); 
    free(information);
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
    if (IsConnected(G, i, j)) {
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
    if (!IsConnected(G, i, j)) {
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

