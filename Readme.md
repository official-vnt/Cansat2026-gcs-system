### Project Overview
This repository contains the Ground Control System (GCS) software for the VNT CanSat 2026 mission. The GCS is a desktop Qt application that:
Ingests raw binary telemetry frames arriving over a virtual COM/USB serial port from a LoRa receiver module.
Validates each frame using CRC-16 (CCITT) integrity checks.
Renders live charts for altitude, temperature, pressure, and 3-axis accelerometer data.
Logs every validated packet to a timestamped `.csv` file for post-flight analysis.
Hardware chain: `STM32 + BMP280/IMU` → `LoRa TX` RF `LoRa RX` → `USB-UART bridge` → this software.


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
### Prerequisites & Toolchain
**1. C++ Compiler**
Windows	MinGW-w64 (GCC 13+)	Bundled with Qt installer
Linux	GCC 12+ or Clang 15+	`sudo apt install build-essential`
macOS	Clang (Xcode 14+)	`xcode-select --install`
C++17 standard is required (`CMAKE_CXX_STANDARD 17` is already set in `CMakeLists.txt`).

**2. Qt 6 Framework**
Download from https://www.qt.io/download-open-source.
Required Qt modules (select during installation):
`Qt6::Core`
`Qt6::Gui`
`Qt6::Widgets`
`Qt6::SerialPort`
`Qt6::Charts`
Minimum version: Qt 6.2 LTS. The build folder in this repo was generated with Qt 6.11.1 MinGW 64-bit.

**3. CMake**
Version 3.16 or newer is required.
```bash
# Linux
sudo apt install cmake

# macOS
brew install cmake

# Windows — download installer from https://cmake.org/download/
```
---
## Quick Start — Clone & Build
```bash
**1. Clone the repository**
git clone https://github.com/official-vnt/Cansat2026-gcs-system.git
cd Cansat2026-gcs-system

**2. Configure with CMake (replace path to your Qt install)**
cmake -B build \
      -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x.x/gcc_64" \
      -DCMAKE_BUILD_TYPE=Debug

**Windows example (MinGW):**
# cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.1/mingw_64" -G "MinGW Makefiles"

**3. Build**
cmake --build build --parallel

**4. Run**
./build/app.gcs.system          # Linux/macOS
build\app.gcs.system.exe        # Windows
```
> **Qt Creator shortcut:** Open `CMakeLists.txt` directly in Qt Creator → select your kit → click the green Run button. The IDE handles CMake configuration automatically.**

---
## Step 3 — Launch the GCS dashboard
Start the built executable and, in the port selector, choose `/dev/pts/4` (or `COM6` on Windows) at 115200 baud.
The dashboard will immediately start rendering live charts fed by the simulated data.
---
1. Transmitter board. Telemetry frames will begin appearing within 1–2 seconds.
2. Logged CSV files are written to a `logs/` folder in the working directory, named by session timestamp: `session_20260530Running With Hardware
3. Connect the LoRa RX module via USB-UART bridge to your workstation.
4. Confirm the COM port appears in your OS (e.g., `/dev/ttyUSB0` on Linux, `COM3` on Windows).
5. Launch `app.gcs.system` and select the correct port at 115200 baud (or match your firmware's baud rate).
6. Power on the STM32_143022.csv`.
---

## 📅 Phased Execution Workflow

This project is executed in **5 distinct phases**. 

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

| Offset (Byte) | Field Name     | Data Type  | Size (Bytes) | Description                    |
| ------------- | -------------- | ---------- | ------------ | ------------------------------ |
| `0x00`        | `START_FRAME`  | `uint16_t` | 2            | Sync Marker: Always `0xAA55`   |
| `0x02`        | `TEAM_ID`      | `char[8]`  | 8            | Team Identifier (ASCII String) |
| `0x0A`        | `TIME`         | `int32_t`  | 4            | Mission elapsed time (seconds) |
| `0x0E`        | `PACKET_COUNT` | `uint16_t` | 2            | Incrementing packet counter    |
| `0x10`        | `ALTITUDE`     | `float`    | 4            | Altitude (m)                   |
| `0x14`        | `PRESSURE`     | `float`    | 4            | Pressure (Pa)                  |
| `0x18`        | `TEMPERATURE`  | `float`    | 4            | Temperature (°C)               |
| `0x1C`        | `VOLTAGE`      | `float`    | 4            | Battery Voltage (V)            |
| `0x20`        | `LATITUDE`     | `double`   | 8            | GPS Latitude                   |
| `0x28`        | `LONGITUDE`    | `double`   | 8            | GPS Longitude                  |
| `0x30`        | `GPS_ALTITUDE` | `float`    | 4            | GPS Altitude (m)               |
| `0x34`        | `SATELLITES`   | `int32_t`  | 4            | Number of GPS Satellites       |
| `0x38`        | `ACCEL_X`      | `float`    | 4            | Acceleration X (g)             |
| `0x3C`        | `ACCEL_Y`      | `float`    | 4            | Acceleration Y (g)             |
| `0x40`        | `ACCEL_Z`      | `float`    | 4            | Acceleration Z (g)             |
| `0x44`        | `GYRO_X`       | `float`    | 4            | Gyroscope X (°/s)              |
| `0x48`        | `GYRO_Y`       | `float`    | 4            | Gyroscope Y (°/s)              |
| `0x4C`        | `GYRO_Z`       | `float`    | 4            | Gyroscope Z (°/s)              |
| `0x50`        | `STATE`        | `char[16]` | 16           | Current Mission State          |
| `0x60`        | `CRC_CHECKSUM` | `uint16_t` | 2            | CRC-16 (CCITT)                 |

**Total Packet Payload Weight:** 
| Component             | Size         |
| --------------------- | ------------ |
| Payload               | 96 Bytes     |
| CRC                   | 2 Bytes      |
| **Total Packet Size** | **98 Bytes** |

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

## Troubleshooting
**CMake cannot find Qt6:**
Set `CMAKE_PREFIX_PATH` to your Qt installation directory, e.g. `-DCMAKE_PREFIX_PATH="C:/Qt/6.11.1/mingw_64"`.
**Serial port permission denied (Linux):**
Add your user to the `dialout` group: `sudo usermod -aG dialout $USER` and log out/in.
**All packets fail CRC:**
Confirm the CRC polynomial and initial value in `DataParser` exactly match the firmware. Also check endianness — the STM32 is little-endian and so is x86/x64, but verify `#pragma pack(push, 1)` is active on both sides.
**Charts freeze or lag:**
Ensure chart updates are driven by the `QTimer` at 30 Hz rather than directly from the `packetReady` signal. High packet rates will saturate the GUI thread if connected directly.
**Simulator not sending bytes:**
Check that you are writing to the correct end of the virtual COM pair (the write end, not the read end). The GCS dashboard must be connected to the other end.
---

