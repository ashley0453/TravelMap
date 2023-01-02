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
    int v, w;   
    int info;   
    int level;
}ArcInfo;   //�洢����Ϣ

typedef struct Dijskra {
    int PreNode;
    int info;
}Dijskra;//�Ͻ�˹�����㷨�д���ǰ��ڵ���Ȩֵ

typedef struct Permutations {
    int* path;
    int info;
}Permutations;//����;��·����ȫ�����Լ���Ӧ��Ȩֵ

int FirstAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);
int NextAdjSpot(ALGraph G, int k, AdjSpotNodeP& p);

bool IsDirectConnected(ALGraph G, int i, int j);//�ж����������Ƿ����ֱ��������

bool IsReach(ALGraph G, int i, int j);//��Ϊ�ж��Ƿ�ɵ�������
bool IsReachable(ALGraph G, int i, int j);
void SetTags(ALGraph& G);//��ͼ�е�Tag��������


Status CreatUDGGraph(ALGraph& G, SpotNode* Spots, int n, ArcInfo* arce, int e);//��ʼ��ͼ
Status AddSpot(ALGraph& G);//���������
Status AddEdge(ALGraph& G);//������·
Status RemoveEdge(ALGraph& G);//ɾ�����еı�
Status ReviseSpot(ALGraph& G);//�޸����о������Ϣ
Status ReviseEdge(ALGraph& G);//�޸����бߵ���Ϣ
Status SpotIntroduce(ALGraph G);//��������

Status FindShortRoad(ALGraph G);
Status ShortestRoad(ALGraph G, int Start, Dijskra*& dij);//Ѱ������֮�����·��
void OutputRoad(ALGraph G, Dijskra* dij, int Destinatin, char* Road);

Status FindAppealingRoad(ALGraph G);//���ݵ�·��·��������ȥѡ��������·���ú���Ϊ���
Status MostAppealingRoad(ALGraph G, int start, int des, int* CurrentPath, int* path, int CurrentLen, int& len, int CurrentLevel, int& MaxLevel);
void PrintAppealingRoad(ALGraph G, int* path, int length, int MaxLevel);

bool FindAllRoad(ALGraph G, int start, int destination, int* path, int len, int& sum);//�������е�·


long long int Fact(int i);
void Swap(int x, int y, int* num);
Status Perm(int left, int right, int* num, int& cnt, Permutations*& result);
Status SuitableRoad(ALGraph G, int start, int des);//���ڲ��ҵ�����ָ����λ;��ʱ�����·��

void Clearbuffer();//���������
void DrawUI(ALGraph G);
void CreatVexAndArc(SpotNode* spots, ArcInfo* arcs);
#endif