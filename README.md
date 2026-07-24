# ZGuard

### Zero Trust Industrial Guardian for Intelligent Machine Safety

An intelligent Industrial IoT platform that combines real-time machine health monitoring, Zero Trust cybersecurity, and autonomous decision-making through the Guardian Decision Engine.

---

<p align="center">
  <img src="https://img.shields.io/badge/Industrial%20IoT-007ACC?style=for-the-badge" alt="Industrial IoT" />
  <img src="https://img.shields.io/badge/Zero%20Trust-red?style=for-the-badge" alt="Zero Trust" />
  <img src="https://img.shields.io/badge/Guardian%20Decision%20Engine-blueviolet?style=for-the-badge" alt="Guardian Decision Engine" />
  <img src="https://img.shields.io/badge/Industrial%20Safety%20Index-success?style=for-the-badge" alt="Industrial Safety Index" />
  <br/>
  <img src="https://img.shields.io/badge/Digital%20Twin-orange?style=for-the-badge" alt="Digital Twin" />
  <img src="https://img.shields.io/badge/ESP32-222222?style=for-the-badge" alt="ESP32" />
  <img src="https://img.shields.io/badge/Real--Time%20Monitoring-00c853?style=for-the-badge" alt="Real-Time Monitoring" />
  <img src="https://img.shields.io/badge/Hackathon%20MVP-gold?style=for-the-badge" alt="Hackathon MVP" />
</p>

---

## Team

- **Team Name:** FIVEFOLD
- **Members:** K G V Kiruthika sri, M V Vijay aditiya, Bhavadharani R, Thanishkar, Padmavathi

### Roles:
- **System Orchestrator:** K G V Kiruthika sri
- **Trust Engine Architect:** M V Vijay aditiya
- **Cyber Defense Architect:** Bhavadharani R
- **Guardian Intelligence Architect:** Thanishkar
- **Digital Twin & Visual Storyteller (Dashboard and UI Engineer):** Padmavathi

- **Hackathon:** 
- **Duration:** 24 Hours
- **Domain:** Industrial IoT • Cybersecurity • Embedded Systems • Smart Manufacturing

---

## System Architecture

```mermaid
flowchart LR

subgraph Hardware["IoT Hardware Layer"]
A[Power Source]
B[Voltage Sensor]
C[Current Sensor]
D[ESP32]
E[RFID Reader]
F[Authorized RFID Tag]
end

subgraph Edge["Edge Processing"]
G[Sensor Data Acquisition]
H[RFID Authentication]
end

subgraph Backend["Z Guard Backend"]
I[Data Processing API]
J[Fault Detection Engine]
K[Zero Trust Authentication]
L[Decision Engine]
M[(Database)]
end

subgraph Response["Response Layer"]
N[Dashboard]
O[Alert Notification]
P[Device Isolation]
end

A --> B
A --> C

B --> D
C --> D

F --> E
E --> H

D --> G

G --> I
H --> K

I --> J
J --> L
K --> L

L --> N
L --> O
L --> P

I --> M
J --> M
K --> M
```

## Detailed Workflow

This is the exact decision sequence the ESP32 runs on every loop cycle — from raw sensor read to physical action.

```mermaid
flowchart TD
    Start(["Power on / Boot"]) --> Init["Initialize sensors, RFID,<br/>LCD, relay, Wi-Fi"]
    Init --> Loop["Main loop start"]

    Loop --> ReadSensors["Read potentiometer, potentiometer,<br>LDRvalues"]
    ReadSensors --> Tamper{"Tamper switch<br/>triggered?"}

    Tamper -- "Yes" --> ForceStop["Trust = 0<br/>Force STOP OPERATION"]
    ForceStop --> Actuate

    Tamper -- "No" --> RFIDCheck{"Valid RFID<br/>scanned?"}
    RFIDCheck -- "No" --> Locked["Decision = LOCKED<br/>Motor won't start"]
    Locked --> Actuate

    RFIDCheck -- "Yes" --> Normalize["Normalize sensor values<br/>to 0–100 scale"]
    Normalize --> Health["Compute Health Score<br/>(temp + current + vibration)"]
    Health --> Trust["Compute Trust Score<br/>(RFID + tamper status)"]
    Trust --> Risk["Compute Predictive Risk Score<br/>(rule-based fault prediction)"]
    Risk --> ISI["Compute ISI<br/>= w1·Health + w2·Trust + w3·(100−Risk)"]

    ISI --> Decide{"ISI value?"}
    Decide -- "≥ 75" --> Safe["SAFE TO OPERATE"]
    Decide -- "50–74" --> Warn["WARNING"]
    Decide -- "25–49" --> Maint["MAINTENANCE REQUIRED"]
    Decide -- "< 25" --> Stop["STOP OPERATION"]

    Safe --> StateCheck
    Warn --> StateCheck
    Maint --> StateCheck
    Stop --> StateCheck["FSM check:<br/>was previous state STOP?"]
    StateCheck -- "Yes, needs manual reset" --> ForceStop
    StateCheck -- "No" --> Actuate["Drive relay,<br/>update OLED, buzzer, LEDs"]

    Actuate --> Log["Log event to timeline"]
    Log --> Loop
```

### Step-by-step

1. **Boot** — ESP32 initializes all sensors, the RFID reader, OLED, and relay to a known safe state.
2. **Sensor read** — current, temperature, and vibration are sampled every cycle.
3. **Tamper check** — this overrides everything else; a triggered enclosure switch forces STOP regardless of sensor health.
4. **RFID check** — an invalid or missing card locks the motor out before any score is even computed.
5. **Scoring** — Health, Trust, and Risk scores are calculated independently, then fused into the ISI.
6. **Decision mapping** — the ISI is mapped to one of four states.
7. **State memory (FSM)** — the system won't silently leave STOP just because the ISI recovers; it requires an explicit reset.
8. **Action** — the relay, LCD, buzzer, and LEDs are updated to reflect the final decision.
9. **Logging** — every decision is appended to the event timeline for the dashboard.
10. Loop repeats continuously.

---

# 🛠 Tech Stack

## Hardware
- ESP32 Dev Module
- RFID-RC522 Reader
- Voltage Sensor
- Current Sensor
- Buzzer
- Relay Module
- LED
- Breadboard & Jumper Wires

## Software
- Arduino IDE
- Python (FastAPI)
- HTML, CSS, JavaScript
- Git & GitHub

## Database
- SQLite / PostgreSQL 

## Communication
- Wi-Fi
- HTTP REST API
- JSON

---

## ESP32 Firmware

1. Open the `firmware` folder in Arduino IDE.
2. Select **ESP32 Dev Module**.
3. Update Wi-Fi credentials.
4. Upload the firmware to ESP32.

---

# ▶️ Usage

1. Power the ESP32.
2. Scan an authorized RFID card.
3. ESP32 starts collecting voltage and current data.
4. Sensor data is transmitted to the backend.
5. Fault detection analyzes electrical parameters.
6. Dashboard displays device status.
7. If a fault is detected, an alert is generated and the device can be isolated.

---

# 🗄 Database

## Tables

### Devices
- Device ID
- RFID UID
- Status

### Sensor Data
- Timestamp
- Voltage
- Current
- Device ID

### Alerts
- Alert ID
- Fault Type
- Severity
- Timestamp

---

# 🤖 Fault Detection Workflow

```text
RFID Authentication
        │
        ▼
ESP32 Activated
        │
        ▼
Read Voltage & Current
        │
        ▼
Transmit Sensor Data
        │
        ▼
Backend Processing
        │
        ▼
Fault Detection
        │
        ▼
Normal? ─────► Dashboard Updated
        │
       No
        ▼
Alert Generated
        │
        ▼
Device Isolation (if required)
```

---

# ⚠️ Challenges Faced

- Integrating hardware components (ESP32, RFID, voltage and current sensors).
- Ensuring reliable real-time sensor data transmission.
- Reducing false fault detections caused by sensor noise.
- Synchronizing RFID authentication with live device monitoring.
- Maintaining low latency between hardware and backend.
- Managing secure communication between IoT devices and the server.

---

# 🚀 Future Scope

- Integrate AI/ML models for predictive fault detection and anomaly analysis.
- Support multiple IoT devices with centralized monitoring.
- Implement MQTT for scalable real-time communication.
- Add cloud deployment for remote monitoring and management.
- Introduce role-based access control and advanced Zero Trust policies.
- Send instant alerts via SMS, Email, or mobile notifications.
- Develop a dedicated Android/iOS application.
- Generate automated maintenance reports and analytics dashboards.

---

# 🔒 Security Measures

- RFID-based device authentication to allow only authorized access.
- Secure communication between ESP32 and backend using REST APIs.
- Input validation and sanitization for all incoming sensor data.
- Fault detection using real-time voltage and current monitoring.
- Zero Trust approach where every device request is verified before processing.
- API keys and sensitive credentials are stored using environment variables (`.env`) and are never hardcoded.
- Access logs are maintained for monitoring authentication and system events.

---

# 🤖 AI Integration

This project uses the **Google Gemini API** for AI-powered analysis and assistance.

## AI Features
- Fault analysis and interpretation.
- Intelligent insights from sensor data.
- Assistance in identifying abnormal device behavior.
- Context-aware recommendations based on detected faults.
