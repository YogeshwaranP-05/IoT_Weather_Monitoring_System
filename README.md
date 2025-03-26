### **IoT Weather Monitoring System using Arduino**

---

### **🌤️ Project Overview**

This project demonstrates how to build an **IoT Weather Monitoring System** using an **Arduino** and a collection of sensors. The system measures **Air Quality Index (AQI)**, **temperature**, **humidity**, **atmospheric pressure**, and detects **rain**. The data is displayed on a **local webpage** served by the Arduino, allowing users to monitor real-time weather conditions from any device connected to the same network.

✅ **Key Features:**

* Real-time monitoring of environmental data

* Web interface accessible over a local network

* Continuous data refresh for accurate updates

* Easy-to-build DIY project with basic components

---

### **🛠️ Components Required**

* **Arduino Uno R4 WiFi**  → For processing sensor data and WiFi connectivity

* **MQ135** → Air Quality sensor to measure AQI

* **DHT11** → Temperature and humidity sensor

* **BMP180** → Atmospheric pressure sensor

* **Rain Sensor** → Detects rain presence

* **Breadboard** → For prototyping

* **Resistors & Jumper Wires** → For connections

* **Power Supply (5V)** → To power the system

---

### **🗂️ Project Structure**

`/IoT-Weather-Monitoring-System`    
`├── /Code                  # Arduino sketch (.ino)`    
`├── /Circuit Diagram       # Circuit diagram and wiring illustrations`    
`├── /Block Diagram         # Block diagram of IoT Weather Station`  
`├── README.md              # Project documentation`  

---

### **⚙️ Installation and Setup**

#### **1\. Clone the Repository**

`git clone`   
`https://github.com/YogeshwaranP-05/IoT_Weather_Monitoring_System.git`   
`cd IoT_Weather_Monitoring_System`  

#### **2\. Connect the Components**

* **MQ135 (AQI Sensor)**:

  * VCC → 5V

  * GND → GND

  * AOUT → Analog Pin (A3)

* **DHT11 (Temp & Humidity)**:

  * VCC → 5V

  * GND → GND

  * Data → Digital Pin (D2)

* **BMP180 (Pressure Sensor)**:

  * VCC → 5V

  * GND → GND

  * SDA → SDA (A4)

  * SCL → SCL (A5)

* **Rain Sensor:**

  * VCC → 5V

  * GND → GND

  * Digital Output → Digital Pin (D3)

#### **3\. Upload the Code**

* Open the **IoT\_Weather\_Monitoring\_Arduino`.ino`** file in the **Arduino IDE**.

* Select the appropriate board (Arduino Uno R4 WiFi Board) and COM port.

* Upload the code to the Arduino.

#### **4\. Run the System**

* Power the system with a 5V supply.

* On your locally connected device, open a web browser and enter Arduino's IP address.

* View the real-time weather data on the webpage.

---

### **📝 Code Explanation**

✅ **Sensor Data Collection:**

* The **MQ135** sensor measures **Air Quality Index (AQI)**.

* The **DHT11** sensor provides **temperature** and **humidity** readings.

* The **BMP180** measures **atmospheric pressure** using I2C communication.

* The **Rain Sensor** detects rain presence and sends a digital signal.

✅ **Web Server Implementation:**

* The **Arduino** creates a simple local web server.

* Environmental data is displayed on the webpage with real-time updates.

✅ **Webpage Display:**

* **Temperature & Humidity: `27.5°C` and `65%`**

* **Atmospheric Pressure: `1013 hPa`**

* **Air Quality: `Good (AQI: 45)`**

* **Rain Status: `No Rain` or `Raining`**

---

### **🌐 Local Webpage Preview**

`-------------------------------------------`  
`|   IoT Weather Monitoring System         |`  
`-------------------------------------------`  
`| Temperature: 27.5°C                     |`  
`| Humidity: 65%                           |`  
`| Pressure: 1013 hPa                      |`  
`| Air Quality: Good (AQI: 45)             |`  
`| Rain Status: No Rain                    |`  
`-------------------------------------------`

---

### **🤝 Contributing**

We welcome contributions\! 🎯  
 If you have ideas for improvement or want to add new features:

1. Fork the repository.

2. Make your changes.

3. Submit a pull request.

---

### **🛡️ License**

This project is licensed under the **GPL-3.0 License**.  
 Refer to the **`LICENSE`** file for details.

---

### **📧 Contact**

For any queries or suggestions, reach out via:

 🌐 **Official Website:** [https://circuitdigest.com/microcontroller-projects/](https://github.com/YogeshwaranP-05)

🚀 **Happy coding and weather monitoring\!** 🌦️

