import ctypes
import os
import sys

class HnswBridge:
    def __init__(self):
        if not os.path.exists("./libhnsw.so") and not os.path.exists("./libhnsw.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libhnsw.dll hnsw_lite.c")
                lib_path = "./libhnsw.dll"
            else:
                os.system("gcc -shared -fPIC -o libhnsw.so hnsw_lite.c")
                lib_path = "./libhnsw.so"
        else:
            lib_path = "./libhnsw.dll" if sys.platform.startswith("win") else "./libhnsw.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_hnsw.restype = ctypes.c_void_p
        self.lib.insert_graph_node.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.POINTER(ctypes.c_float)]
        self.lib.search_layer.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int]
        self.lib.search_layer.restype = ctypes.c_int
        self.lib.free_hnsw.argtypes = [ctypes.c_void_p]
        
        self.graph_ptr = self.lib.init_hnsw()

    def add_vector(self, doc_id: int, vector: list):
        c_array = (ctypes.c_float * len(vector))(*vector)
        self.lib.insert_graph_node(self.graph_ptr, doc_id, c_array)

    def search_proximity(self, query_vector: list, targeting_layer: int) -> int:
        c_array = (ctypes.c_float * len(query_vector))(*query_vector)
        return self.lib.search_layer(self.graph_ptr, c_array, targeting_layer)

    def __del__(self):
        if hasattr(self, 'lib') and self.graph_ptr:
            self.lib.free_hnsw(self.graph_ptr)
