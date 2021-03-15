/*
	�������� ������ ������ (���������� ��� ������ ������������)
	��� �������� �� ������
	Author:     Andrew Mamohin
*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// 9 � 10 - ��� ������ ����� CE � CSN ������ nRF24L01+
RF24 radio(9, 10);
// ������, ��� ��������� ������
int data[2] = {};
const byte w = A2; // ��� �������
const byte jo = A3; // ��� JDY-40 OK
const byte ja = A4; // ��� JDY-40 ALARM
const byte wk = 4; // ��� ������
const byte wr = 2; // ��� �������� �����
unsigned long ti0 = 0; // ���������� �����
unsigned long ti = 0; // ����� � �������
int sleep = 33000; // ������� ���

void setup() {
	delay(4000);
	pinMode(w, OUTPUT);
	pinMode(wk, INPUT);
	pinMode(wr, OUTPUT);
	pinMode(jo, INPUT);
	pinMode(ja, INPUT);
	digitalWrite(w, LOW);
	digitalWrite(wr, LOW);

	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	radio.begin();
	delay(1000);
	radio.setChannel(64);
	radio.setDataRate(RF24_250KBPS);
	// radio.setCRCLength(RF24_CRC_8);
	radio.setPALevel(RF24_PA_MAX);
	radio.setAutoAck(false);
	delay(1000);
	radio.powerUp();
	radio.openReadingPipe(1, 0x1234567890LL);
	delay(2);
	radio.startListening();
	delay(2);
}

void loop() {
	ti = millis();

	if (radio.available()) {
		ti0 = ti + sleep;
		radio.read(&data, sizeof(data));
		if (data[0] == 66) {
			digitalWrite(wr, HIGH);
			pipa();
		}
		if (data[0] == 67) {
			digitalWrite(wr, HIGH);
			pipa();
		}
	}

	if (analogRead(jo) > 125) { ti0 = ti + sleep; }

	if (analogRead(ja) > 125) {
		ti0 = ti + sleep;
		digitalWrite(wr, HIGH);
		pipa();
	}

	if (ti0 < ti) { pipi(); }

	if (digitalRead(wk)) {
		digitalWrite(wr, LOW);
		pipu();
	}

}

void pipi() {
	tone(w, 3500);
	delay(200);
	noTone(w);
	delay(3000);
}

void pipu() {
	tone(w, 2000);
	delay(200);
	noTone(w);
	delay(200);
}

void pipa() {
	delay(500);
	tone(w, 1100);
	delay(500);
	tone(w, 800);
	delay(500);
	noTone(w);
}
