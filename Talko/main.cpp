//
//  main.cpp
//  Talko
//
//  Created by Jian Lou on 5/10/15.
//  Copyright (c) 2015 JL. All rights reserved.
//

#include <iostream>

#include <iostream>
#define DICT_SIZE    200
#define OUTPUT_DEBUG 0
#define CHAR_NUM     3

template <class T>
int32_t alloc_mem_1d(T** p, uint32_t d)
{
    *p = new T[d];
    if (*p == NULL)
    {
        return -1;
    }
    return 0;
}

template <class T>
int32_t free_mem_1d(T* p)
{
    if (p)
    {
        delete [] p;
        return 0;
    }
    else
    {
        return -1;
    }
}

template <class T>
int32_t alloc_mem_2d(T*** p, uint32_t d1, uint32_t d2)
{
    *p = new T*[d1];
    if (*p == NULL)
    {
        return -1;
    }
    if (alloc_mem_1d(*p, d1*d2) == -1)
    {
        return -1;
    }
    for (uint32_t i=1; i<d1; i++)
    {
        (*p)[i] = (*p)[i-1] + d2;
    }
    return 0;
}

template <class T>
int32_t free_mem_2d(T** p)
{
    if (p)
    {
        if (free_mem_1d(*p) == -1)
        {
            return -1;
        }
        delete p;
        return 0;
    }
    else
    {
        return 0;
    }
}

uint32_t checkConnected(char A[5], char B[5])
{
    uint32_t identicalCharNum = 0;
    
    for (uint32_t i=0; i<5; i++)
    {
        if (A[i] == B[i])
            identicalCharNum++;
    }
    
    return (identicalCharNum==4);
}

int32_t isNotInQueue(int32_t* Queue, int32_t r, int32_t i)
{
    for (uint32_t j=0; j<=r; j++)
    {
        if (i == Queue[j])
        {
            return 0;
        }
    }
    
    return 1;
}

static int32_t f=0, r=-1;
int32_t findABPath(uint32_t AIdx, uint32_t BIdx, char** Map, char* isVisited, int32_t* Queue, int32_t* Dist)
{
#if OUTPUT_DEBUG
    printf("AIdx %d\n", AIdx);
#endif
    int32_t step = -1;
    for (uint32_t i=0; i<DICT_SIZE; i++)
    {
        if (Map[AIdx][i])
        {
            if (i==BIdx)
            {
                printf("Shortest transformation with step %d\n", Dist[AIdx]+1);
                return Dist[AIdx]+1;
            }
            if(!isVisited[i])
            {
                if (isNotInQueue(Queue, r+1, i))
                    Queue[++r] = i;
                if (Dist[i]==-1)
                {
                    Dist[i] = Dist[AIdx]+1;
                }
#if OUTPUT_DEBUG
                printf("(A=%d, i=%d s=%d)\n", AIdx, i, Dist[i]);
#endif
            }
        }
    }
#if OUTPUT_DEBUG
    printf("\n");
#endif
    if(f<=r)
    {
        isVisited[Queue[f]] = 1;
        step = findABPath(Queue[f++], BIdx, Map, isVisited, Queue, Dist);
    }
    
    return step;
}

int main(int argc, const char * argv[])
{
    char**   Dictionary;
    char**   Map;
    char*    isVisited;
    int32_t* Queue;
    int32_t* Dist;
    char     A[5];
    char     B[5];
    uint32_t AIdx, BIdx;
    
    alloc_mem_2d(&Dictionary, DICT_SIZE, 5        );
    alloc_mem_2d(&Map,        DICT_SIZE, DICT_SIZE);
    alloc_mem_1d(&isVisited,  DICT_SIZE);
    alloc_mem_1d(&Queue,      DICT_SIZE);
    alloc_mem_1d(&Dist,       DICT_SIZE);
    
    memset(isVisited,    0, DICT_SIZE);
    memset(Queue,     0xff, DICT_SIZE);
    memset(Dist,      0xff, DICT_SIZE*sizeof(int32_t));
    
    // use rand() to generate a dictionary
    for (uint32_t i=0; i<DICT_SIZE; i++)
    {
        for (uint32_t j=0; j<5; j++)
        {
            Dictionary[i][j] = rand()%CHAR_NUM;
        }
    }
    
    // generate connected map
    for (uint32_t i=0; i<DICT_SIZE; i++)
    {
        for (uint32_t j=i; j<DICT_SIZE; j++)
        {
            Map[i][j] = Map[j][i] = checkConnected(Dictionary[i], Dictionary[j]);
        }
    }
    
    // random pick A and B
    AIdx = rand()%DICT_SIZE;
    memcpy(A, Dictionary[AIdx], 5);
    printf("A %d, [%2d %2d %2d %2d %2d]\n", AIdx, A[0], A[1], A[2], A[3], A[4]);
    BIdx = rand()%DICT_SIZE;
    memcpy(B, Dictionary[BIdx], 5);
    printf("B %d, [%2d %2d %2d %2d %2d]\n", BIdx, B[0], B[1], B[2], B[3], B[4]);
    
#if OUTPUT_DEBUG
    for (uint32_t i=0; i<DICT_SIZE; i++)
    {
        printf("%d, ", i);
        for (uint32_t j=0; j<5; j++)
        {
            printf("%2d ", Dictionary[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (uint32_t i=0; i<DICT_SIZE; i++)
    {
        for (uint32_t j=0; j<DICT_SIZE; j++)
        {
            printf("%d,", Map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
#endif
    
    // find the shortest path between A and B
    Dist[AIdx]      = 0;
    isVisited[AIdx] = 1;
    uint32_t tmp = findABPath(AIdx, BIdx, Map, isVisited, Queue, Dist);
    if (tmp == -1)
    {
        printf("Could not find the shortest path between A and B\n");
    }
    else
    {
        printf("Shortest transformation is %d\n", tmp);
    }
    
#if OUTPUT_DEBUG
    for (uint32_t i=0; i<DICT_SIZE; i++)
    {
        printf("%d, ", i);
        for (uint32_t j=0; j<5; j++)
        {
            printf("%2d ", Dictionary[i][j]);
        }
        printf("\n");
    }
    printf("\n");
#endif
    
    free_mem_2d(Dictionary);
    free_mem_2d(Map);
    free_mem_1d(isVisited);
    free_mem_1d(Queue);
    free_mem_1d(Dist);
    
    return 0;
}

