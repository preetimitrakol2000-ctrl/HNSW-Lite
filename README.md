# HNSW-Lite // Hierarchical Navigable Small World Proximity Graph

A vector database acceleration project implementing coarse-to-fine graph traversals inside custom memory layers to execute search routines under $O(\log N)$ bounds.

## Mechanics
- **Skip Graphs (C Layout):** Chains proximity nodes inside multidimensional vector topologies to eliminate full-table scanning operations.
- **Python Integration:** Directly queries physical heap elements to populate retrieval prompts for AI security evaluation pipelines.

## Launch
```bash
python hnsw_rag.py
