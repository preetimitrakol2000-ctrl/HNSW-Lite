#ifndef HNSW_LITE_H
#define HNSW_LITE_H

typedef struct GraphNode GraphNode;
typedef struct HNSWGraph HNSWGraph;
HNSWGraph* init_hnsw();
void insert_graph_node(HNSWGraph* g, int doc_id, float* vector);
int search_layer(HNSWGraph* g, float* query, int layer);
void free_hnsw(HNSWGraph* g);

#endif
