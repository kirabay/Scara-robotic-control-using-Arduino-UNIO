
/////control of SCARA
#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD-VARIANT-COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);
unsigned char buf6[8]; // buffer to save contents of length 8 byte
unsigned char buf5[8];
unsigned char buf7[8];
unsigned char buf8[8];
unsigned long p,p1,p2;
int theta;
char arr[14]; // declare array for reciving data
int i = 0, j = 0, n;
unsigned long cc,cc1,cc2, cc3;
void setup() {32
Serial.begin(9600); // setting baudrate for communication
while (CAN_OK != CAN.begin(CAN_1000KBPS)){
SERIAL.println("CAN BUS SHIELD init fail"); // print on the serial monitor
SERIAL.println("Init CAN BUS Shield again");
delay(100);}
}
void loop(){
while(Serial.available()>0) { //If anything comes in Serial (USB),
char asd = Serial.read();
arr[i]=asd-48; // convert ASCI
//Serial.write(arr[i]);
if(i == 13){
cc = arr[0]*100 + arr[1]*10 + arr[2]; // store data in array
cc1 = arr[3]*100 + arr[4]*10 + arr[5];
cc2 = arr[6]*100 + arr[7]*10 + arr[8];
cc3 = arr[9]*100 + arr[10]*10 + arr[11];
n = arr[12]*10 + arr[13];
}
i++;
if(i==14){
i=0;33
break;
}
}
Serial.write(cc); // write data to serial port
Serial.write(cc1);
Serial.write(cc2);
Serial.write(cc3);
Serial.write(n);
// acceleration control
buf6[0] = 0x34; buf6[1] = 0x00; buf6[2] = 0x00; buf6[3] = 0x00; buf6[4] = 0x01;
buf6[5] = 0x02; buf6[6] = 0x00; buf6[7] = 0;
CAN.sendMsgBuf(0x141,0,8,buf6); // send acceleration control command to joint2
CAN.sendMsgBuf(0x142,0,8,buf6);// send acceleration control command to end
effector
CAN.sendMsgBuf(0x143,0,8,buf6);// send acceleration control command to joint1
while(j<n){
fun(cc);
CAN.sendMsgBuf(0x143,0,8,buf5); // send position control command to joint1
delay(2000);
fun1(cc1);
CAN.sendMsgBuf(0x141,0,8,buf7);// send position control command to joint2
delay(3000);
// command end effector to pick object
fun2(360);
CAN.sendMsgBuf(0x142,0,8,buf8);34
delay(5000);
fun2(0);
CAN.sendMsgBuf(0x142,0,8,buf8);
delay(5000);
fun(cc2);
CAN.sendMsgBuf(0x143,0,8,buf5);// send position control command to joint1
delay(1000);
fun1(cc3);
CAN.sendMsgBuf(0x141,0,8,buf7);// send position control command to joint2
delay(3000);
// command end effector to drop object
fun2(360);
CAN.sendMsgBuf(0x142,0,8,buf8);
delay(5000);
fun2(0);
CAN.sendMsgBuf(0x142,0,8,buf8);
delay(5000);
j++;
}
}
// store position command in buf5
void fun(unsigned long a){35
p = 600 * a;
buf5[0] = 0xA3; buf5[1] = 0x00; buf5[2] = 0x00; buf5[3] = 0x00; buf5[4] = p;
buf5[5] = p>>8; buf5[6] = p>>16; buf5[7] = p>>24;
}
// store position command in buf7
void fun1(unsigned long a){
p1 = 600 * a;
buf7[0] = 0xA3; buf7[1] = 0x00; buf7[2] = 0x00; buf7[3] = 0x00; buf7[4] = p1;
buf7[5] = p1>>8; buf7[6] = p1>>16; buf7[7] = p1>>24;
}
// store position command in buf8
void fun2(unsigned long a){
p2 = 600 * a;
buf8[0] = 0xA3; buf8[1] = 0x00; buf8[2] = 0x00; buf8[3] = 0x00; buf8[4] = p2;
buf8[5] = p2>>8; buf8[6] = p2>>16; buf8[7] = p2>>24;
}
////Limit switch
#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD-VARIANT-COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);
unsigned char buf6[8]; //declare buffer to save contents of length 8 byte
unsigned char buf5[8];
unsigned long cp5=0; // cp5= desired position
const int Lswitch =8; //
unsigned long p1;
int x = 0;
void setup()
{
pinMode(Lswitch,INPUT);// set pin 8 as input
}
void loop()
{
//if the limit switch is not pressed
if ((digitalRead(Lswitch) == HIGH) && x==0){37
p1 = 600*cp5;
buf5[0] = 0xA3; buf5[1] = 0x00; buf5[2] = 0x00; buf5[3] = 0x00; buf5[4] = p1;
buf5[5] = p1>>8; buf5[6] = p1>>16; buf5[7] = p1>>24;//store position command in buf5
buf6[0] = 0x34; buf6[1] = 0x00; buf6[2] = 0x00; buf6[3] = 0x00; buf6[4] = 0xE0;
buf6[5] = 0x04; buf6[6] = 0x00; buf6[7] = 0;//store acceleration command in buf6
CAN.sendMsgBuf(0x142,0,8,buf6); // send command for the actuator
CAN.sendMsgBuf(0x142,0,8,buf5);
}
// while the limit switch is pressed (if Lswitch = low)
while((digitalRead(Lswitch) == !HIGH)){
x=1;
}
if(x==1){
int p2 = 600*0;
//store command in buf5 and buf6
buf5[0] = 0xA3; buf5[1] = 0x00; buf5[2] = 0x00; buf5[3] = 0x00; buf5[4] = p2;
buf5[5] = p2>>8; buf5[6] = p2>>16; buf5[7] = p2>>24;
buf6[0] = 0x34; buf6[1] = 0x00; buf6[2] = 0x00; buf6[3] = 0x00; buf6[4] = 0xE0;
buf6[5] = 0x04; buf6[6] = 0x00; buf6[7] = 0;
// send command for the actuator
CAN.sendMsgBuf(0x142,0,8,buf6);
CAN.sendMsgBuf(0x142,0,8,buf5);
}
}
Processing Codes
///SCARA GUI
import processing.serial.*;
import controlP5.*; // Instantiate ControlP5
import static processing.core.PApplet.*;
Serial myport;
ControlP5 cp5;
PFont font;
String t0,t11,t22,t33,t44,n;
String t0i,t1i,t2i;
String xT,yT;
float theta0,theta11,theta22,theta33,theta44;
float x,y;
float L2 = 213.12, L3 = 215.45;//430.45;
String xPS,yPS;
// home position at
int yP=-215;//643.57
int xP=213;//=0;
int xD,yD;
void setup() {
size(550, 700);
myport = new Serial(this, "COM4", 9600); // connecting processing with arduino39
cp5 = new ControlP5(this); //
font = createFont("Arial", 20);
cp5.addTextfield("Textfield1")//Name of the text field
.setPosition(60, 220) //(x,y) top left Corner
.setSize(170, 40) //(x,y) top left Corner
.setFont(font)
.setColor(100)
.setCaptionLabel("143 THETA 1") // display caption
;
cp5.addTextfield("Textfield2")//Name of the text field
.setPosition(280, 220)//(x,y) top left Corner
.setSize(170, 40)//(x,y) top left Corner
.setFont(font)
.setColor(100)
.setCaptionLabel("141 THETA 2")
;
cp5.addButton("done") //Name of the Button
.setPosition(220, 600) //(x,y) top left Corner
.setSize(80, 40) //(x,y) top left Corner
.setFont(font)
// .setColorBackground(#3269c2)
.setCaptionLabel("DONE")
;
cp5.addTextfield("Textfield3")
.setPosition(60, 360)40
.setSize(170, 40)
.setFont(font)
.setColor(100)
.setCaptionLabel("143 THETA 1")
;
cp5.addTextfield("Textfield4")
.setPosition(280 , 360)
.setSize(170, 40)
.setFont(font)
.setColor(100)
.setCaptionLabel("141 THETA 2")
;
cp5.addTextfield("loop") //Name of the text field
.setPosition(60 , 500) //(x,y) top left Corner
.setSize(100, 40)
.setFont(font)
.setColor(100)
.setCaptionLabel("no of loop") ; // display caption
}
void draw() {
background(50); //background color of window (r, g, b)
fill(255); //text color (r, g, b)
textFont(font);
textSize(30);
text("SCARA Robot Control", 125, 110);41
textSize(40);
text("N.E.R.D Center", 130, 60);
textSize(20);
text("Pick @ : " + "(" + xP +" , " + yP + ")" , 60, 205); // display pick position
text("Drop @ : " + "(" + xD + " , " + yD + ")" , 60, 345); // display drop position
}
void done() {
t11 = cp5.get(Textfield.class,"Textfield1").getText(); // (143) get value for joint 1 for pick
position
t22 = cp5.get(Textfield.class,"Textfield2").getText(); // (141)get value for joint 2 for pick
position
t33= cp5.get(Textfield.class,"Textfield3").getText(); // (143)get value for joint 1 for drop
position
t44 = cp5.get(Textfield.class,"Textfield4").getText(); // (141)get value for joint 2 for drop
position
n = cp5.get(Textfield.class,"loop").getText();
theta11 = Float.parseFloat(t11); // change data type into float
theta22 = Float.parseFloat(t22);
theta33 = Float.parseFloat(t33);
theta44 = Float.parseFloat(t44);
forwardKinematics();
//send the angles in 142-141-143 order
myport.write(t11);
myport.write(t22);
myport.write(t33);42
myport.write(t44);
myport.write(n);
}
// FORWARD KINEMATICS
void forwardKinematics() {
float theta1F = theta11 * PI / 180; // degrees to radians
float theta2F = theta22 * PI / 180;
float theta3F = theta33 * PI / 180; // degrees to radians
float theta4F = theta44 * PI / 180;
xP = round(L2 * cos(theta1F) + L3 * cos(theta1F + theta2F)); // apply forward kinematics
yP = round(L2 * sin(theta1F) + L3 * sin(theta1F + theta2F));
xD = round(L2 * cos(theta3F) + L3 * cos(theta3F + theta4F));
yD = round(L2 * sin(theta3F) + L3 * sin(theta3F + theta4F));
}
void serialEvent(Serial myPort) {
// If anything comes in Serial (USB),
println(myPort.read()); // read it and send it out Serial1 (pins 0 & 1) }