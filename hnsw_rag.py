from hnsw_bridge import HnswBridge

if __name__ == "__main__":
    kb_data = {
        5001: "Cluster Alpha: Access control telemetry logs confirm anomaly patterns on domain nodes.",
        5002: "Cluster Beta: Firewall filtering tables updated to drop anomalous external handshake requests."
    }

    hnsw = HnswBridge()
    # Populate embedding spaces mapped to index nodes
    hnsw.add_vector(5001, [0.15, 0.88, 0.34, 0.55])
    hnsw.add_vector(5002, [0.75, 0.22, 0.91, 0.12])

    # Search the top layer for coarse, rapid navigation
    query = [0.12, 0.82, 0.31, 0.59]
    matched_id = hnsw.search_proximity(query, targeting_layer=0)

    print("=== HNSW-LITE HIERARCHICAL NAVIGABLE GRAPH PROXIMITY ENGINE ===")
    print(f"[*] Traversed Multi-Layer Topology Graph. Closest Proximity Entity Matches: {matched_id}")
    print(f"[*] Retrieved Segment:\n -> {kb_data.get(matched_id)}")
