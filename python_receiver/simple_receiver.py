import socket
import struct
import time

# UDP Server erstellen
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('0.0.0.0', 8888))
print("UDP Server listening on port 8888...")
print("Waiting for data from Nucleo board (192.168.1.38)...")

try:
    while True:
        data, addr = sock.recvfrom(1024)
        
        if len(data) == 12:  # SensorData: float+float+uint32 = 12 bytes
            temp, hum, timestamp = struct.unpack('ffI', data)
            current_time = time.strftime("%H:%M:%S")
            print(f"[{current_time}] From {addr[0]}: Temp={temp:.2f}°C, Hum={hum:.2f}%, Time={timestamp}ms")
        else:
            print(f"From {addr}: Unexpected data length {len(data)} bytes")
            
except KeyboardInterrupt:
    print("\nServer stopped")
finally:
    sock.close()
    