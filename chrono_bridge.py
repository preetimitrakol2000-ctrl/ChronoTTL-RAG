import ctypes
import os
import sys

class ChronoBridge:
    def __init__(self):
        if not os.path.exists("./libchrono.so") and not os.path.exists("./libchrono.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libchrono.dll ttl_ring.c")
                lib_path = "./libchrono.dll"
            else:
                os.system("gcc -shared -fPIC -o libchrono.so ttl_ring.c")
                lib_path = "./libchrono.so"
        else:
            lib_path = "./libchrono.dll" if sys.platform.startswith("win") else "./libchrono.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_ring.restype = ctypes.c_void_p
        self.lib.push_log_chunk.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_ulonglong]
        self.lib.purge_expired_chunks.argtypes = [ctypes.c_void_p, ctypes.c_ulonglong]
        self.lib.compile_valid_text.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.compile_valid_text.restype = ctypes.c_int
        
        self.ring_ptr = self.lib.init_ring()

    def register_chunk(self, doc_id: int, message: str, expiry_timestamp: int):
        self.lib.push_log_chunk(self.ring_ptr, doc_id, message.encode('utf-8'), expiry_timestamp)

    def clear_stale_records(self, evaluation_timestamp: int):
        self.lib.purge_expired_chunks(self.ring_ptr, evaluation_timestamp)

    def extract_active_context(self) -> str:
        buf = ctypes.create_string_buffer(2048)
        self.lib.compile_valid_text(self.ring_ptr, buf)
        return buf.value.decode('utf-8')
