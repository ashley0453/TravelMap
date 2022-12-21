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
    int x, y;//记录位置信息，用于判断方向
    struct AdjSpotNode* FirstSpot;
}SpotNode;
typedef struct AdjSpotNode {
    int SpotSymbol;
    struct AdjSpotNode* NextSpot;
    int Distence;//距离
    int Level;//沿途风景等级
} AdjSpotNode, * AdjSpotNodeP;
typedef struct ALGraph {
    SpotNode* Spots;
    int n, e;
    GraphKind kind;
    int* tags;
} ALGraph;

typedef struct ArcInfo {
    int v, w;   //边的端点
    int info;   //对带权图，为权值，此处为距离
    int level;
}ArcInfo;   //存储边信息

typedef struct Dijskra {
    int PreNode;
    int info;
}Dijskra;

int FirstAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);
int NextAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);

void PrintSymbol(ALGraph G);

bool IsDirectConnected(ALGraph G, int i, int j);
bool IsReachable(ALGraph G, int i, int j);
Status CreatUDGGraph(ALGraph& G, SpotNode* Spots, int n, ArcInfo* arce, int e);
Status AddSpot(ALGraph& G);
Status AddEdge(ALGraph& G);
Status RemoveEdge(ALGraph& G);
Status ReviseSpot(ALGraph& G);
Status ReviseEdge(ALGraph& G);
Status SpotIntroduce(ALGraph G, int k);

Status ShortestRoad(ALGraph G, int Start, Dijskra*& dij);
Status MostAppealingRoad(ALGraph G, int start, int des, int* CurrentPath, int* path, int CurrentLen, int& len, int CurrentLevel, int& MaxLevel);
bool FindAllRoad(ALGraph G, int start, int destination, int* path, int len, int& sum);
void OutputRoad(ALGraph G, Dijskra* dist, int Destinatin);

void Clearbuffer();//清除缓冲区


#endif