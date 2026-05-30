import time
import argparse
import serial
import math

def generate_packet(packet_id, timestamp):
    team_id = "TEAM_CANSAT"
    time_val = timestamp
    packet_count = packet_id % 65536
    altitude = 10.0 + (packet_id % 500)
    pressure = 101325.0 - (packet_id % 100) * 10
    temperature = 25.0 + (packet_id % 10) * 0.5
    voltage = 9.0 - (packet_id * 0.001)
    if voltage < 6.0: voltage = 9.0
    
    latitude = 34.0 + (packet_id * 0.0001)
    longitude = -118.0 + (packet_id * 0.0001)
    gps_altitude = altitude + 2.0
    satellites = 8
    
    ax = math.sin(packet_id * 0.1)
    ay = math.cos(packet_id * 0.1)
    az = 9.81
    
    gx = 0.01
    gy = -0.02
    gz = 0.00
    
    state = "FLIGHT" if altitude > 50 else "IDLE"

    # CSV Format: teamID,time,packetCount,altitude,pressure,temperature,voltage,lat,lon,gpsAlt,sats,ax,ay,az,gx,gy,gz,state
    csv_string = f"{team_id},{time_val},{packet_count},{altitude:.2f},{pressure:.2f},{temperature:.2f},{voltage:.2f},{latitude:.6f},{longitude:.6f},{gps_altitude:.2f},{satellites},{ax:.2f},{ay:.2f},{az:.2f},{gx:.2f},{gy:.2f},{gz:.2f},{state}\n"
    
    return csv_string.encode('latin1')

def main():
    parser = argparse.ArgumentParser(description="GCS Telemetry CSV Simulator")
    parser.add_argument("--port", type=str, required=True, help="Serial port to send data (e.g. COM1)")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    parser.add_argument("--rate", type=float, default=10.0, help="Packets per second (Hz)")
    args = parser.parse_args()

    try:
        ser = serial.Serial(args.port, args.baud)
    except serial.SerialException as e:
        print(f"Error opening port {args.port}: {e}")
        return

    print(f"Streaming telemetry to {args.port} at {args.baud} baud ({args.rate} Hz)...")
    
    packet_id = 0
    start_time = time.time()
    sleep_time = 1.0 / args.rate

    try:
        while True:
            timestamp_ms = int((time.time() - start_time) * 1000)
            packet = generate_packet(packet_id, timestamp_ms)
            ser.write(packet)
            
            if packet_id % int(args.rate) == 0:
                print(f"Sent Packet ID {packet_id}")

            packet_id += 1
            time.sleep(sleep_time)
    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
