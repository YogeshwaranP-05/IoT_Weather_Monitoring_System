/*--------------------------------------------------------------
---------------------------Header Files-------------------------
----------------------------------------------------------------*/
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "Arduino_LED_Matrix.h"
#include "WiFiS3.h"


/*-----------------------------------------------------------------------
-------------------------Sensor pin definition Macros-------------------
-------------------------------------------------------------------------*/
#define Rain_SensorPin 3
#define Air_SensorPin A3
#define Temp_Hum_SensorPin 2


/*-------------------------------------------------------------------------
--------------------------Object instantiation-----------------------------
--------------------------------------------------------------------------*/
DHT_Unified dht(Temp_Hum_SensorPin, DHT11);
Adafruit_BMP085 bmp;
WiFiServer server(80);
ArduinoLEDMatrix matrix;  


/*--------------------------------------------------------------------
-------------------------Global variables----------------------------
----------------------------------------------------------------------*/

//Bring WiFi Connected Symbol on LED Matrix
const uint32_t wifi_connected[] = {
    0x3f840,
		0x49f22084,
		0xe4110040
};


//Bring WiFi Disconnrcted Symbol on LED Matrix
const uint32_t no_wifi[] = {
    0x403f844,
		0x49f22484,
		0xe4110040
};


//SSID & PASSWORD of LAN Router
char ssid[] = "Semicon Media";
char pass[] = "XXXXXXXXXXXXX";


//Variables to hold weather data from sensors
float temperature = 0.0, humidity = 0.0, pressure = 0.0;
int AQI = 0, rainfall = 0;

//Flag variable for storing last sensor data update time and last wifi connectivity checking time
unsigned long lastSensorUpdate = 0;
unsigned long lastWiFiCheck = 0;


/*---------------------------------------------------------------------
-----------------User Defined Functions--------------------------------
---------------------------------------------------------------------------*/


//Function for making Arduino to connect to the WiFi Network
void wifi_connect(){


  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    matrix.loadFrame(no_wifi);
    while (true);
  }

  Serial.print("Connecting to WiFi...");
  matrix.loadSequence(LEDMATRIX_ANIMATION_WIFI_SEARCH);
  matrix.play(true);
  delay(6000);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  matrix.loadFrame(wifi_connected);
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}


//Function for making Arduino to reconnect to the WiFi Network, after getting disconnected
void wifi_reconnect(){
    Serial.println("Wifi Reconnecting........");
    matrix.loadFrame(no_wifi);
    delay(6000);
    wifi_connect();
}



//Functiom to read the weather data from the collection of sensors
void read_sensor_data(){
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    temperature = event.temperature;

    dht.humidity().getEvent(&event);
    humidity = event.relative_humidity;

    pressure = bmp.readPressure() / 100.0; // Convert Pa to mbar

    int mq135Raw = analogRead(Air_SensorPin);
    float mq135PPM = mq135Raw * (5.0 / 1023.0) * 20.0; // Convert raw value to PPM
    AQI = map(mq135PPM, 0, 500, 0, 300); // Convert PPM to AQI (adjust as needed)

    rainfall = digitalRead(Rain_SensorPin) == HIGH ? 0 : 1;

}


//Function to send JSON data to the requested Web client Device(Browser)
void send_json_data(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  
  String json = "{\"temperature\":" + String(temperature) +
                ",\"humidity\":" + String(humidity) +
                ",\"pressure\":" + String(pressure) +
                ",\"aqi\":" + String(AQI) +
                ",\"rainfall\":" + String(rainfall) + "}";
  
  client.println(json);
}



//Function to send Dynamic weather dashboard webpage to the requested web client(Browser)
void send_web_page(WiFiClient &client) {
  // Send HTTP headers
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  // Use raw string literal for HTML content
  const char* html = R"rawliteral(
<!DOCTYPE html>
<html lang='en'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Weather Dashboard</title>
<style>
  body { font-family: Arial, sans-serif; background: #f4f4f4; color: #333; text-align: center; padding: 20px; }
  h1 { font-size: 2rem; color: #0077cc; }
  .container { max-width: 900px; margin: auto; }
  .data-container { display: flex; flex-direction: column; gap: 10px; }
  .data-row { display: flex; justify-content: space-between; align-items: center; }
  .data-card { background: #fff; padding: 15px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); flex: 1; margin: 5px; text-align: center; }
  .graph { background: #fff; padding: 15px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); margin-top: 15px; }
  canvas { width: 100%; height: 400px; }
</style>
</head>
<body>
<h1>Weather Dashboard</h1>
<div class='container'>
  <div id='weather' class='data-container'></div>
  <div class='graph'><canvas id='combinedGraph'></canvas></div>
</div>

<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>
<script>
  const ctxCombined = document.getElementById('combinedGraph').getContext('2d');

  // Create a single graph with both temperature and humidity
  const combinedChart = new Chart(ctxCombined, {
    type: 'line',
    data: {
      labels: [],
      datasets: [
        {
          label: 'Temperature (°C)',
          data: [],
          borderColor: '#ff5733',
          backgroundColor: 'rgba(255, 87, 51, 0.2)',
          fill: true,
          tension: 0.4,
          pointRadius: 3
        },
        {
          label: 'Humidity (%)',
          data: [],
          borderColor: '#2196f3',
          backgroundColor: 'rgba(33, 150, 243, 0.2)',
          fill: true,
          tension: 0.4,
          pointRadius: 3
        }
      ]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: false,
      scales: {
        x: { title: { display: true, text: 'Time' } },
        y: { 
          beginAtZero: true,
          min: 0,
          max: 100,
          ticks: { stepSize: 10 }
        }
      }
    }
  });

  // Fetch and update data
  function fetchWeatherData() {
    fetch('/data')
      .then(response => response.json())
      .then(data => {
        document.getElementById('weather').innerHTML = `
          <div class='data-row'>
            <div class='data-card'>🌡️ Temp: ${data.temperature}°C &nbsp;&nbsp; 💧 Humidity: ${data.humidity}%</div>
          </div>
          <div class='data-row'>
            <div class='data-card'>🌬️ Pressure: ${data.pressure} mbar</div>
          </div>
          <div class='data-row'>
            <div class='data-card'>🌫️ AQI: ${data.aqi} &nbsp;&nbsp; ☔ Rainfall: ${data.rainfall ? 'Yes' : 'No'}</div>
          </div>`;

        let time = new Date().toLocaleTimeString();
        
        // Add new data points
        combinedChart.data.labels.push(time);
        combinedChart.data.datasets[0].data.push(data.temperature);  // Temperature
        combinedChart.data.datasets[1].data.push(data.humidity);    // Humidity

        // Keep only the last 10 points
        if (combinedChart.data.labels.length > 10) {
          combinedChart.data.labels.shift();
          combinedChart.data.datasets[0].data.shift();
          combinedChart.data.datasets[1].data.shift();
        }

        combinedChart.update();
      });
  }

  setInterval(fetchWeatherData, 1000);
</script>
</body>
</html>
)rawliteral";

  // Send the entire HTML content in one go
  client.print(html);
}


//Function to run a local webserver on Arduino for handling client request
void run_local_webserver(){
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("GET / ") != -1) {
      send_web_page(client);
    } else if (request.indexOf("GET /data") != -1) {
      send_json_data(client);
    }

    client.stop();
  }
}


/*-----------------------------------------------------------------
-----------------------Setup Function------------------------------
------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  matrix.begin();
  
  wifi_connect();

  server.begin();

  pinMode(Rain_SensorPin, INPUT);
  pinMode(Air_SensorPin, INPUT);
  dht.begin();

  while (!bmp.begin()) {
    Serial.println("BMP085 sensor not found, check wiring!");
  }
}



/*-----------------------------------------------------------
-----------------Loop function-------------------------------
-------------------------------------------------------------*/
void loop() {

 if (millis() - lastSensorUpdate >= 1000) {
    lastSensorUpdate = millis();
    read_sensor_data();
 }
 
 if(millis() - lastWiFiCheck >= 5000){
     lastWiFiCheck = millis();
     
     if(WiFi.status() != WL_CONNECTED){
        wifi_reconnect();
     }
 } 

 run_local_webserver();  
  
}





