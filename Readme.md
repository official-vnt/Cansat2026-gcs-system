### Project Architecture
app.gcs.system/
├── .gitignore
├── CMakeLists.txt                 # Root CMake configuration
├── README.md                      # Project documentation (generated earlier)
│
├── cmake/                         # Optional: Custom CMake modules / finder scripts
│
├── src/                           # Main source directory
│   ├── main.cpp                   # Application entry point
│   │
│   ├── common/                    # Shared definitions between modules
│   │   └── TelemetryDef.h         # Byte-aligned structural data contract
│   │
│   ├── core/                      # Backend, threading, and data processing
│   │   ├── SerialWorker.h         # Async QSerialPort receiver
│   │   ├── SerialWorker.cpp
│   │   ├── DataParser.h           # Packet validation & CRC calculation
│   │   └── DataParser.cpp
│   │
│   ├── gui/                       # Frontend UI rendering & layout classes
│   │   ├── MainWindow.h           # Main dashboard control logic
│   │   ├── MainWindow.cpp
│   │   ├── MainWindow.ui          # Qt Designer UI layout file
│   │   ├── DashboardChart.h       # Custom real-time chart wrapper widget
│   │   └── DashboardChart.cpp
│   │
│   └── storage/                   # Data logging & file handling
│       ├── Logger.h               # CSV/Binary continuous stream writer
│       └── Logger.cpp
│
├── third_party/                   # External header-only libraries or dependencies
│   └── qcustomplot/               # High-performance plotting widget directory
│       ├── qcustomplot.h
│       └── qcustomplot.cpp
│
└── tools/                         # Testing utilities and simulators
    └── simulator.py               # Python mock byte generator

1. **Hardware Node (STM32 + LoRa TX):** Collects physical sensor readings, builds structured byte frames, appends validation bits, and transmits them via RF.
2. **Ground Station Node (LoRa RX + USB Bridge):** Receives RF packets and pushes raw serial binary data through a virtual COM port over USB.
3. **Dashboard Software (Qt C++ App):** * **Ingestion Worker (Non-Blocking):** Continuously captures raw bytes via `QSerialPort` into a ring buffer inside a dedicated `QThread`.
   * **Parser / Validator:** Identifies frame headers, verifies data integrity using a Cyclic Redundancy Check (CRC), reconstructs variables, and routes them.
   * **UI Thread:** Render widgets, gauges, state matrices, and dynamic charts without frame drops.

---

## 📅 Phased Execution Workflow

This project is executed in **5 distinct phases** over a 4-week timeline. Do not move to the next phase until the verification checklist for the current phase passes completely.

### Phase 1: Data Contract & Infrastructure Setup
* **Objective:** Establish the baseline code architecture, communication boundaries, and packet specifications.
* **Tasks:**
  * Define the exact bit-level byte alignment structure (`#pragma pack(1)`) shared between the STM32 firmware and C++ dashboard.
  * Initialize the git workspace containing two primary sub-directories: `/firmware` and `/dashboard`.
  * Set up the base Qt project using CMake, linking required modules (`Core`, `Widgets`, `SerialPort`, `Charts`).

### Phase 2: Backend Enginer & Simulation Engine
* **Objective:** Code the processing logic and decouple from physical hardware by building a virtual data provider.
* **Tasks:**
  * Develop the non-blocking asynchronous `SerialReceiver` worker sub-classing `QObject` and managing a internal packet buffer.
  * Implement standard CRC-16 or CRC-32 algorithmic checks to drop partial/corrupt frames.
  * Construct a Python or C++ Mock Data Generator script capable of outputting structured byte streams to a virtual COM pair (`socat` or `com0com`).

### Phase 3: Telemetry Core UI & Graphics Binding
* **Objective:** Design a responsive graphical system to process and display high-speed telemetry vectors cleanly.
* **Tasks:**
  * Architect a main visual dashboard layout with a unified darker interface theme optimized for outdoor/high-ambient environments.
  * Integrate `QCustomPlot` or `QtCharts` widgets, writing a localized update loop restricted to 30Hz to prevent CPU rendering saturation.
  * Interconnect backend thread signals to frontend chart update slots using Qt's thread-safe connection mechanism (`Qt::QueuedConnection`).

### Phase 4: Data Persistence & Session Log Engine
* **Objective:** Build a fail-safe streaming file writer to log flight or operation runs safely onto local non-volatile storage.
* **Tasks:**
  * Implement an active file writer utility tracking current operating system timestamps.
  * Save files dynamically to `.csv` format inside a `/logs` root directory, flushing data lines continuously to prevent memory caching loss on unexpected terminal crashes.

### Phase 5: Hardware-in-the-Loop (HIL) Integration & Stress Validation
* **Objective:** Bind software against actual LoRa transceiver configurations and perform stability stress profiles.
* **Tasks:**
  * Wire the physical STM32 development board directly to the workstation over USB for wired loopback serial checks.
  * Deploy the LoRa wireless transmitter and receiver, testing signal fade margins, Packet Drop Rates (PDR), RSSI readouts, and parsing response profiles.
  * Run continuous 4-hour performance profiles to discover and eliminate heap leaks or GUI thread deadlocks.

---

## 📄 Telemetry Data Frame Contract

To ensure interoperability, the STM32 (Transmitter) and the Qt Dashboard (Receiver) must strictly adhere to the exact binary structure mapped below.

| Offset (Byte) | Field Name | Data Type | Size (Bytes) | Description / Range |
| :--- | :--- | :--- | :--- | :--- |
| `0x00` | `START_FRAME` | `uint16_t` | 2 | Sync Marker: Always `0xAA55` |
| `0x02` | `PACKET_ID` | `uint32_t` | 4 | Monotonically incrementing packet counter |
| `0x06` | `TIMESTAMP` | `uint32_t` | 4 | System uptime in milliseconds since boot |
| `0x0A` | `TEMPERATURE` | `float` | 4 | Sensor internal temperature in °C |
| `0x0E` | `PRESSURE` | `float` | 4 | Atmospheric pressure in Pascals (Pa) |
| `0x12` | `ALTITUDE` | `float` | 4 | Calculated altitude relative to ground level (meters) |
| `0x16` | `ACCEL_X` | `float` | 4 | Accelerometer X-axis force in $g$ |
| `0x1A` | `ACCEL_Y` | `float` | 4 | Accelerometer Y-axis force in $g$ |
| `0x1E` | `ACCEL_Z` | `float` | 4 | Accelerometer Z-axis force in $g$ |
| `0x22` | `RSSI` | `int16_t` | 2 | Received Signal Strength Indicator (dBm) |
| `0x24` | `SNR` | `int16_t` | 2 | Signal-to-Noise Ratio (dB) |
| `0x26` | `STATUS_BYTE` | `uint8_t` | 1 | System error masking bitmap flags |
| `0x27` | `CRC_CHECKSUM` | `uint16_t` | 2 | CRC-16 (CCITT) checksum value calculated over bytes `0x00` to `0x26` |

**Total Packet Payload Weight:** 41 Bytes

---

## 🛠️ Step-by-Step Implementation Guide

### Step 1: Establish Your Development Environment
Ensure you have the required toolchains installed and configured on your host workstation.

* **C++ Compiler:** GCC (Linux), MinGW (Windows), or Clang (macOS) supporting the **C++17** standard or newer.
* **Qt Framework:** Qt 6.x (or 5.15 LTS) Community or Commercial Edition.
* **Build System:** CMake (v3.16+).
* **Virtual Ports (For Testing):**
  * *Linux/macOS:* Install `socat` via package managers (`sudo apt install socat`).
  * *Windows:* Install [com0com](https://sourceforge.net/projects/com0com/) or an equivalent null-modem emulator.

### Step 2: C++ Telemetry Frame Implementation
Declare the matching telemetry block layout explicitly inside a shared configuration header (`TelemetryDef.h`) to guarantee byte alignment across architectures:

```cpp
#pragma once
#include <cstdint>

// Force compilers to prevent byte padding optimized for host processors
#pragma pack(push, 1)
struct TelemetryPacket {
    uint16_t start_frame;   // 0xAA55
    uint32_t packet_id;
    uint32_t timestamp;
    float    temperature;
    float    pressure;
    float    altitude;
    float    accel_x;
    float    accel_y;
    float    accel_z;
    int16_t  rssi;
    int16_t  snr;
    uint8_t  status_byte;
    uint16_t crc;
};
#pragma pack(pop)

Step 3: Implement Multi-Threaded Serial Ingestion Worker
Write a non-blocking consumer model using native Qt threading design to intercept serial data streams safely without degrading frontend layout frames.

Step 4: Spawning Worker Threads within Main Window Core
Instantiate the thread pipeline safely when initialization flags run:

Step 5: Constructing The Testing Mock Simulator
Create a Python validation script (simulator.py) to feed dummy telemetry bytes over physical testing targets for standalone debugging sessions:

