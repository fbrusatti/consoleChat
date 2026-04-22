#!/usr/bin/env python3
"""
Simple integration tests for the chat server.
"""

import socket
import subprocess
import time
import sys
import os
import signal

SERVER_PATH = "./server"
HOST = "127.0.0.1"
PORT = 4849

def start_server():
    proc = subprocess.Popen(
        [SERVER_PATH, str(PORT)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    time.sleep(1)
    return proc

def connect_client(name):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(2)
    sock.connect((HOST, PORT))
    sock.recv(1024)
    sock.sendall(f"\\NICKNAME {name}\n".encode())
    sock.recv(1024)
    return sock

def get_list(sock):
    sock.sendall(b"\\LIST\n")
    time.sleep(0.2)
    data = b""
    sock.setblocking(False)
    try:
        while True:
            chunk = sock.recv(1024)
            if chunk:
                data += chunk
            else:
                break
    except BlockingIOError:
        pass
    return data.decode()

def cleanup(proc):
    proc.terminate()
    try:
        proc.wait(timeout=2)
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait()

def test_server_starts():
    proc = start_server()
    time.sleep(0.5)
    result = proc.poll()
    cleanup(proc)
    print("PASS" if result is None else f"FAIL: {result}")
    return result is None

def test_client_connection():
    proc = start_server()
    try:
        sock = connect_client("Alice")
        sock.close()
        print("PASS")
        return True
    except Exception as e:
        print(f"FAIL: {e}")
        return False
    finally:
        cleanup(proc)

def test_disconnect_removes():
    proc = start_server()
    try:
        sock1 = connect_client("Alice")
        sock2 = connect_client("Bob")
        
        time.sleep(0.2)
        sock2.close()
        time.sleep(0.5)
        
        response = get_list(sock1)
        sock1.close()
        
        has_alice = "- Alice" in response
        has_bob = "- Bob" in response
        
        print("PASS" if has_alice and not has_bob else f"FAIL: alice={has_alice} bob={has_bob}")
        return has_alice and not has_bob
    except Exception as e:
        print(f"FAIL: {e}")
        return False
    finally:
        cleanup(proc)

def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)) + "/..")
    print(f"Testing server at {SERVER_PATH}")
    print(f"PORT: {PORT}")
    
    results = []
    results.append(("Server starts", test_server_starts()))
    results.append(("Client connection", test_client_connection()))
    results.append(("Disconnect removes from list", test_disconnect_removes()))
    
    print("\n=== RESULTS ===")
    all_pass = True
    for name, passed in results:
        print(f"{'PASS' if passed else 'FAIL'}: {name}")
        all_pass = all_pass and passed
    
    return 0 if all_pass else 1

if __name__ == "__main__":
    sys.exit(main())