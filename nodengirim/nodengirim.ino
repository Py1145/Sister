//************************************************************
// this is a simple example that uses the painlessMesh library
// 
// This example shows how to build a mesh with named nodes
//
//************************************************************
#include "namedMesh.h"
#include "DHT.h"

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define   DHTTYPE DHT11
#define   DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

Scheduler  userScheduler; // to control your personal task
namedMesh  mesh;

String nodeName = "Node 1"; // Name needs to be unique

float suhu = dht.readTemperature();

Task taskSendMessage( TASK_SECOND*30, TASK_FOREVER, []() {
    String msg = String("Pesan dari: ") + nodeName + String("Nilai Suhu:") + suhu + String(" °C");
    String to = "Node Lapor";
    mesh.sendSingle(to, msg); 
}); // start with a one second interval

void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);  // set before init() so that you can see startup messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);

  mesh.setName(nodeName); // This needs to be an unique name! 

  mesh.onChangedConnections([]() {
    Serial.printf("Changed connection\n");
  });

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
  dht.begin();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
