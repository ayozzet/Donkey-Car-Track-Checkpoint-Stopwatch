# Donkey Car Track Checkpoint Stopwatch
 10 Checkpoints Donkey Car Standard Track Stopwatch

Equipments:
1. Receiver (1pc) - ESP32 Dev Module
2. Sender (10pcs) - ESP32 Dev Module + Laser Light Receiver Module + 5V Laser Beam Module
3. Donkey Track (Standard)
4. 3D printed holder for Sender
5. Conductive Ink Pen

How it works:
1. Reset the counter by putting ESP32 EN pin on GND
2. Start the counter EITHER ID11 Sender trip the laser line OR ESP32 pin 32 get input high
3. ID1-ID10 Sender as checkpoints sender will send signal to Receiver
4. Even the laser being tripped more than once will assume as single trip
5. All output will appeared in Serial Monitor
6. All output will also send to webpage created by Python Flask (INCOMING)
