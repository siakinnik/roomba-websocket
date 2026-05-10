# Roomba WebSocket Control

![License](https://img.shields.io/github/license/siakinnik/roomba-websocket)
![Platform](https://img.shields.io/badge/platform-ESP8266-blue)

Arduino firmware for ESP8266 (NodeMCU v3) to control an iRobot Roomba (tested on e5) via WebSockets. This project uses a hybrid Serial configuration to ensure stable communication and telemetry.

## Key Features

*   **WebSocket Interface:** Low-latency binary protocol for real-time steering.
*   **Hybrid Serial Architecture:**
    *   **Hardware TX:** Used for sending commands to Roomba with perfect timing.
    *   **Software RX:** Used for receiving sensor data on a separate pin.
*   **mDNS Integration:** Access your robot at `http://roomba.local`.
*   **Auto-Wake:** Includes BRC pin logic to wake the Roomba from sleep mode.

## Required Arduino Libraries

Install these via the Arduino Library Manager:
*   `WebSockets` by Markus Sattler
*   `ESP8266WiFi` (Built-in)
*   `SoftwareSerial` (Built-in for ESP8266)
*   `ESP8266mDNS` (Built-in)

## Hardware Wiring (NodeMCU v3)

| ESP8266 Pin | Roomba ROI Pin | Function |
| :--- | :--- | :--- |
| **TX (GPIO1)** | **RXD** | Sending commands (Hardware) |
| **D7 (GPIO13)** | **TXD** | Receiving data (Software RX) |
| **D5 (GPIO14)** | **BRC** | Wake up / Baud rate change |
| **GND** | **GND** | Common Ground |

*Note: A logic level shifter (5V to 3.3V) is recommended between Roomba's TX and ESP8266's RX.*

## How It Works

1.  **Initial Connection:** The ESP8266 connects to your Wi-Fi and starts a WebSocket server on port `81`.
2.  **Wake up (Command 170):** If the server receives a binary `170` (0xAA), it triggers the BRC pin (LOW for 500ms) to wake the Roomba and sends the start command `128`.
3.  **Command Forwarding:** Any other binary payload is passed directly to the Roomba via Hardware Serial.
4.  **Telemetry:** Data coming from the Roomba is captured via `SoftwareSerial` on pin `D7` and broadcasted to all connected WebSocket clients.

## Installation

1. Clone the repo:
    
   ```bash
   git clone https://github.com/siakinnik/roomba-websocket.git
   ```

2. Open the `.ino` file in Arduino IDE
3. Replace `ssid` and `password` with your Wi-Fi credentials
4. Select `NodeMCU 1.0`(ESP-12E Module) and upload

## License

This project is licensed under the MIT License - see the LICENSE file for details
