#include <esp_now.h>
#include <WiFi.h>

const int laser = 4;
// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x78, 0xE3, 0x6D, 0x18, 0x1C, 0x88};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int id; // must be unique for each sender board
  int x;
  int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  pinMode(laser, INPUT);
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  int tap = digitalRead(laser);
  // Set values to send

  if (tap == HIGH) {
    myData.id = 9;
    myData.x = 1;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    //delay(250);

    if (result == ESP_OK) {
      Serial.println("Sent with success");
//      myData.id = 1;
//      myData.x = 0;
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
    else {
      Serial.println("Error sending the data");
    }
  }
  //  else {
  //    myData.id = 1;
  //    myData.x = 0;
  //    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  //    delay(500);
  //    if (result == ESP_OK) {
  //      Serial.println("Sent with success");
  //    }
  //    else {
  //      Serial.println("Error sending the data");
  //    }
  //  }

  //  myData.id = 1;
  //  myData.x = random(0, 50);
  //  myData.y = random(0, 50);

  // Send message via ESP-NOW
  //  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  //
  //  if (result == ESP_OK) {
  //    Serial.println("Sent with success");
  //  }
  //  else {
  //    Serial.println("Error sending the data");
  //  }
  //delay(10000);
}
