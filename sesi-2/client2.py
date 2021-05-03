import paho.mqtt.client as mqtt
import json

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
    client.subscribe(f"{TOPIC}/#")

def on_message(client, userdata, msg):
    try: 
        topic = msg.topic
        x = json.loads(msg.payload.decode('utf-8'))
        print(f'Pesan masuk : {x}')
        if (topic==f"{TOPIC}/light"):
            if x["message"] == "ON" :
                print("Turn ON")
            elif x["message"] == "OFF": 
                print("Turn OFF")
    except Exception as e:
        print("ERROR", e);

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(CLIENT_USERNAME, CLIENT_PASSWORD)
client.connect(BROKER_IP, BROKER_PORT, 60)

client.loop_forever()

# {\"payload\": {\"humidity\":12, \"temperature\": 25}}
# {"payload": {"humidity":12, "temperature": 25}}
# {"message":"OFF"}