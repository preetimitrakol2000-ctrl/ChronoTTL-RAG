from chrono_bridge import ChronoBridge

if __name__ == "__main__":
    stream_engine = ChronoBridge()

    # Base telemetry insertion paths
    print("=== CHRONOTTL-RAG ACTIVE EXPIRATION PIPELINE ===")
    print("[*] Streaming temporal intelligence packets into low-level ring stores...")
    
    # Simulating epochs: node 1 expires at t=100, node 2 expires at t=200
    stream_engine.register_chunk(201, "Telemetry Alert Alpha: Endpoint 10.0.0.4 dropped standard handshake packets.", 100)
    stream_engine.register_chunk(202, "Telemetry Alert Beta: Memory manipulation traces tracked in runtime spaces.", 200)

    # Time moves to t=150. Purge expired chunks
    print("[*] Synchronizing timeline parameters forward to t=150...")
    stream_engine.clear_stale_records(evaluation_timestamp=150)

    active_knowledge = stream_engine.extract_active_context()
    print(f" -> Active, Non-Expired Knowledge Base Context Remaining:\n {active_knowledge}")
