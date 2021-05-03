import paho.mqtt.client as mqtt
import json
import time 
import random

# BROKER CONFIG
BROKER_IP = "202.148.1.57"
BROKER_PORT = 1883

# CLIENT CONFIG
# APPKEY = app-DemoEsp32.DRr0p0m3l1UsG1hLgwT1QfVlmLA4FD
CLIENT_USERNAME = "app-DemoEsp32"
CLIENT_PASSWORD = "DRr0p0m3l1UsG1hLgwT1QfVlmLA4FD"

TOPIC = "perangkat1"

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))    

client = mqtt.Client()
client.on_connect = on_connect
client.username_pw_set(CLIENT_USERNAME, CLIENT_PASSWORD)
client.connect(BROKER_IP, BROKER_PORT, 60)

while True: 
    time.sleep(5)
    temperature = random.randint(20,25)
    humidity = random.randint(0,100)
    message ={
                "payload": {
                    "temperature": temperature, 
                    "humidity": humidity
                } 
            }
    messageStr = json.dumps(message)
    client.publish(topic=TOPIC, payload=messageStr)