#pragma once
#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>

typedef int Status;
#define UNVISITED 0
#define VISITED 1
#define INFINITY INT_MAX
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW 0
#define UNSELECTED 0
#define SELECTED 1


typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef struct SpotNode{
    int Symbol;
    char *Name;
    char *Introduction;
    int x, y;//��¼λ����Ϣ�������жϷ���
    struct AdjSpotNode* FirstSpot;
}SpotNode;
typedef struct AdjSpotNode {
    int SpotSymbol;
    struct AdjSpotNode* NextSpot;
    int Distence;//����
    int Level;//��;�羰�ȼ�
} AdjSpotNode, * AdjSpotNodeP;
typedef struct ALGraph {
    SpotNode* Spots;
    int n, e;
    GraphKind kind;
    int* tags;
} ALGraph;

typedef struct ArcInfo {
    int v, w;   //�ߵĶ˵�
    int info;   //�Դ�Ȩͼ��ΪȨֵ���˴�Ϊ����
    int level;
}ArcInfo;   //�洢����Ϣ

int FirstAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);
int NextAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);

bool IsConnected(ALGraph G, int i, int j);
Status CreatUDGGraph(ALGraph& G, SpotNode* Spots, int n, ArcInfo* arce, int e);
Status AddSpot(ALGraph& G);
Status AddEdge(ALGraph& G);
Status RemoveEdge(ALGraph& G);
Status ReviseSpot(ALGraph& G);
Status ReviseEdge(ALGraph& G);
Status SpotIntroduce(ALGraph G, int k);


#endif