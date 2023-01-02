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
    int v, w;   
    int info;   
    int level;
}ArcInfo;   //存储边信息

typedef struct Dijskra {
    int PreNode;
    int info;
}Dijskra;//迪杰斯特拉算法中储存前序节点与权值

typedef struct Permutations {
    int* path;
    int info;
}Permutations;//储存途径路径的全排列以及对应的权值

int FirstAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);
int NextAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);

bool IsDirectConnected(ALGraph G, int i, int j);//判断两个景点是否可以直接相连接

bool IsReach(ALGraph G, int i, int j);//作为判断是否可到达的入口
bool IsReachable(ALGraph G, int i, int j);
void SetTags(ALGraph& G);//将图中的Tag给予重置


Status CreatUDGGraph(ALGraph& G, SpotNode* Spots, int n, ArcInfo* arce, int e);//初始化图
Status AddSpot(ALGraph& G);//加入新起点
Status AddEdge(ALGraph& G);//加入新路
Status RemoveEdge(ALGraph& G);//删除已有的边
Status ReviseSpot(ALGraph& G);//修改现有景点的信息
Status ReviseEdge(ALGraph& G);//修改现有边的信息
Status SpotIntroduce(ALGraph G);//景区介绍

Status FindShortRoad(ALGraph G);
Status ShortestRoad(ALGraph G, int Start, Dijskra*& dij);//寻找两点之间最短路径
void OutputRoad(ALGraph G, Dijskra* dij, int Destinatin, char* Road);

Status FindAppealingRoad(ALGraph G);//根据道路道路美丽评分去选择最美道路，该函数为入口
Status MostAppealingRoad(ALGraph G, int start, int des, int* CurrentPath, int* path, int CurrentLen, int& len, int CurrentLevel, int& MaxLevel);
void PrintAppealingRoad(ALGraph G, int* path, int length, int MaxLevel);

bool FindAllRoad(ALGraph G, int start, int destination, int* path, int len, int& sum);//查找所有的路


long long int Fact(int i);
void Swap(int x, int y, int* num);
Status Perm(int left, int right, int* num, int& cnt, Permutations*& result);
Status SuitableRoad(ALGraph G, int start, int des);//用于查找当含有指定点位途径时的最短路径

void Clearbuffer();//清除缓冲区
void DrawUI(ALGraph G);
void CreatVexAndArc(SpotNode* spots, ArcInfo* arcs);
#endif