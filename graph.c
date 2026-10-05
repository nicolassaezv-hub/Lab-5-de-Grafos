#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "map.h"




struct Graph {

    Map* adjacencyMap; 
};


int is_equal_string(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

static char* copy_string(const char* string) {
    size_t length = strlen(string) + 1;
    char* copy = (char*)malloc(length);

    if (copy != NULL) {
        memcpy(copy, string, length);
    }

    return copy;
}



Graph* createGraph() {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->adjacencyMap = map_create(is_equal_string);
    if (g->adjacencyMap == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

void addNode(Graph* g, const char* label) {
    if (!g || !label) return;

    if (map_search(g->adjacencyMap, (void*)label) != NULL) return;

    char* labelCopy = copy_string(label);
    if (labelCopy == NULL) return;

    List* edgesList = list_create();
    if (edgesList == NULL) {
        free(labelCopy);
        return;
    }

    map_insert(g->adjacencyMap, labelCopy, edgesList);
}

void addEdge(Graph* g, const char* src, const char* dest, int weight) {
    if (!g || !src || !dest) return;

    List* edgesList = getEdges(g, src);
    if (edgesList == NULL) return;

    Edge* edge = (Edge*)malloc(sizeof(Edge));
    if (edge == NULL) return;

    edge->target = copy_string(dest);
    if (edge->target == NULL) {
        free(edge);
        return;
    }

    edge->weight = weight;
    list_pushBack(edgesList, edge);
}

List* getEdges(Graph* g, const char* label) {
    if (!g || !label) return NULL;

    MapPair* pair = map_search(g->adjacencyMap, (void*)label);
    if (pair != NULL) {
        return (List*)pair->value;
    }

    return NULL;
}

int getWeight(Graph* g, const char* label1, const char* label2) {
    if (!g || !label1 || !label2) return -1;

    List* edgesList = getEdges(g, label1);
    if (edgesList == NULL) return -1;

    Edge* edge = (Edge*)list_first(edgesList);
    while (edge != NULL) {
        if (strcmp(edge->target, label2) == 0) {
            return edge->weight;
        }

        edge = (Edge*)list_next(edgesList);
    }


    return -1; 
}


List* getAdjacentLabels(Graph* g, const char* label) {
    if (!g || !label) return NULL;

    List* edgesList = getEdges(g, label);
    if (edgesList == NULL) return NULL;

    List* labels = list_create();
    if (labels == NULL) return NULL;

    Edge* edge = (Edge*)list_first(edgesList);
    while (edge != NULL) {
        list_pushBack(labels, edge->target);
        edge = (Edge*)list_next(edgesList);
    }

    return labels; 
}

void destroyGraph(Graph* g) {
    if (!g) return;

    MapPair* pair = map_first(g->adjacencyMap);
    while (pair != NULL) {
        char* label = (char*)pair->key;
        List* edgesList = (List*)pair->value;


        Edge* e = (Edge*)list_first(edgesList);
        while (e != NULL) {
            free(e->target); 
            free(e);         
            e = (Edge*)list_next(edgesList);
        }


        list_clean(edgesList);
        free(edgesList);


        free(label);

        pair = map_next(g->adjacencyMap);
    }


    map_clean(g->adjacencyMap);
    free(g->adjacencyMap);
    free(g);
}