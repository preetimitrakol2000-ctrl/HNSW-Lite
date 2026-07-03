#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LAYERS 3
#define MAX_EDGES 4
#define DIM 4

typedef struct GraphNode {
    int doc_id;
    float coord[DIM];
    int neighbors[MAX_LAYERS][MAX_EDGES];
    int neighbor_count[MAX_LAYERS];
} GraphNode;

typedef struct {
    GraphNode nodes[32];
    int node_count;
    int entry_point_id;
} HNSWGraph;

#ifdef _WIN32
    __declspec(dllexport) HNSWGraph* init_hnsw();
    __declspec(dllexport) void insert_graph_node(HNSWGraph* g, int doc_id, float* vector);
    __declspec(dllexport) int search_layer(HNSWGraph* g, float* query, int layer);
    __declspec(dllexport) void free_hnsw(HNSWGraph* g);
#endif

HNSWGraph* init_hnsw() {
    HNSWGraph* g = (HNSWGraph*)malloc(sizeof(HNSWGraph));
    g->node_count = 0;
    g->entry_point_id = 0;
    return g;
}

float calculate_euclidean(float* v1, float* v2) {
    float sum = 0.0f;
    for (int i = 0; i < DIM; i++) sum += powf(v1[i] - v2[i], 2.0f);
    return sqrtf(sum);
}

void insert_graph_node(HNSWGraph* g, int doc_id, float* vector) {
    int idx = g->node_count;
    g->nodes[idx].doc_id = doc_id;
    memcpy(g->nodes[idx].coord, vector, sizeof(float) * DIM);
    
    for (int l = 0; l < MAX_LAYERS; l++) {
        g->nodes[idx].neighbor_count[l] = 0;
    }
    
    if (idx > 0) {
        // Bi-directional linking with the previous node across infrastructure layers
        int prev = idx - 1;
        for (int l = 0; l < MAX_LAYERS; l++) {
            if (g->nodes[prev].neighbor_count[l] < MAX_EDGES) {
                g->nodes[prev].neighbors[l][g->nodes[prev].neighbor_count[l]++] = idx;
                g->nodes[idx].neighbors[l][g->nodes[idx].neighbor_count[l]++] = prev;
            }
        }
    }
    g->node_count++;
}

int search_layer(HNSWGraph* g, float* query, int layer) {
    if (g->node_count == 0) return -1;
    
    int current_id = g->entry_point_id;
    float current_dist = calculate_euclidean(g->nodes[current_id].coord, query);
    bool changed = true;
    
    while (changed) {
        changed = false;
        GraphNode* curr_node = &g->nodes[current_id];
        for (int i = 0; i < curr_node->neighbor_count[layer]; i++) {
            int neighbor_id = curr_node->neighbors[layer][i];
            float d = calculate_euclidean(g->nodes[neighbor_id].coord, query);
            if (d < current_dist) {
                current_dist = d;
                current_id = neighbor_id;
                changed = true;
            }
        }
    }
    return g->nodes[current_id].doc_id;
}

void free_hnsw(HNSWGraph* g) { free(g); }
