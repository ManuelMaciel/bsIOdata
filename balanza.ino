/*librerias nextion, link https://github.com/bborncr/nextion */
#include <SoftwareSerial.h>
#include <Nextion.h>
//pines de conexion arduino - nextion(tx, rx);
//SoftwareSerial nextion(2, 3);
float peso;
void btnNextp13PopCallback(void *ptr);
void n0_taraPopCallback(void *ptr);
void n0_pesoActualPopCallback(void *ptr);
void n1_taraPopCallback(void *ptr);

NexPicture btnNextP13 = NexPicture(13, 25, "p1");
NexNumber n0_tara = NexNumber(13, 21, "n0_Tara");
NexNumber n0_pesoActual = NexNumber(14, 3, "n0_pesoActual");
NexNumber n1_tara = NexNumber(14, 5, "n1_Tara");

NexTouch *nex_listen_list[] = 
{
    &btnNextP13,
    &n0_tara,
    &n0_pesoActual,
    &n1_tara,
    NULL
};
char buffer[100] = {0};

void n0_taraPopCallback(void *ptr)
{
    dbSerialPrintln("evento tara inicializado");
    n0_tara.setValue(50);
}
void n0_pesoActualPopCallback(void *ptr)
{
    dbSerialPrintln("evento de peso inicial inicializado");
}
void btnNextP13PopCallback(void *ptr){
    dbSerialPrintln("evento de boton inicializado");
    uint32_t tara;
    n0_tara.getValue(&tara);
    n0_pesoActual.setValue(peso);
    n1_tara.setValue(tara);
}
void n1_taraPopCallback(void *ptr){
    dbSerialPrintln("evento de tara inicializado");
}

#include "HX711.h"
//codigo nanoethernet
/*#include <Ethernet.h> 
#include <SPI.h>

byte mac[] = {0x90,0xA2,0xDA,0x0D,0x78,0xEE};    // Direccion MAC
byte ip[] = { 192,168,0,101 }; // Direccion IP del Arduino
byte server[] = { 192,168,0,109 }; // Direccion IP del servidor
EthernetClient client;
 */
 
byte pinData = A2;
byte pinClock = A3;
//creamos el objeto bascula
HX711 bascula;

float f_calibracion = 29780.0; //calibracion exacta bascula 150 kg

//libreria https://github.com/itead/ITEADLIB_Arduino_Nextion ITEADLIB;
//declarar los objetos de nextion a usar
//NexGauge [pagina id:0, componente id:1, nombre de componente:"txtData"]
//Objeto Tara Nextion
//NexNumber n1 = NexNumber(14, 5, "n1"); 
//Objeto Peso Actual Nextion
//NexNumber n0 = NexNumber(14, 3, "n0");
//Objeto Peso Final Nextion
//NexNumber objeto = NexNumber(pagina, id, "nombre"); 
/* para agregar el peso usar el objeto con el comando setText() y el pesodentro*/

void setup() {
nexInit();
n0_tara.attachPop(n0_taraPopCallback);
n0_pesoActual.attachPop(n0_pesoActualPopCallback);
btnNextP13.attachPop(btnNextP13PopCallback);
n1_tara.attachPop(n1_taraPopCallback);
dbSerialPrintln("setup done");

/*Serial.println("Prueba de calibracion. ");
Serial.println("presiona + para aumenta el valor de la calibracion. ");
Serial.println("presiona - para disminuir el valor de la calibracion. ");*/
//Ethernet.begin(mac, ip); // Inicializamos el Ethernet Shield
//delay(1000); // Esperamos 1 segundo de cortesia

bascula.begin(pinData, pinClock);

bascula.set_scale();

bascula.tare();
long factor = bascula.read_average();
Serial.print("factor: ");
Serial.println(factor);
}

void loop() {
  
  medicion_datos();
  nexLoop(nex_listen_list);
  delay(2000);
  //int tara = miNextion.getComponentValue("carga2.n1");
}

void medicion_datos(){
   bascula.set_scale(f_calibracion);
  peso = bascula.get_units(5), 1;
  peso = peso*1000;
  /* codigo nanoethernet
  Serial.println("Connecting...");
  if (client.connect(server, 80)>0) {  // Conexion con el servidor
    client.print("GET /iot/iot.php?valor="); // Enviamos los datos por GET
    client.print(peso);
    client.print("&balanza=");
    client.print("balanza1");
    client.println(" HTTP/1.0");
    client.println("User-Agent: Arduino 1.0");
    client.println();
    Serial.println("Conectado");
  } else {
    Serial.println("Fallo en la conexion");
  }
  if (!client.connected()) {
    Serial.println("Disconnected!");
  }
  */
//mostrar primera lectura de la bascula
  Serial.print("Leyendo: ");
  Serial.print(peso);
  Serial.print(" gs. ");
  Serial.print(" Calibracion: ");
  Serial.print(f_calibracion);
  Serial.println();
  //client.stop();
  //client.flush();
//obtener datos del monitor serie
  /* calibracion   
   
   if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+'){
      f_calibracion += 100;
    } else if (temp == '-'){
      f_calibracion -= 100;
    }
  }*/  
}
