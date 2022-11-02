#
# Copyright © 2016-2022 The Thingsboard Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import paho.mqtt.client as mqtt
from time import sleep
import random
import serial

broker="iot.eie.ucr.ac.cr"
topic_pub='v1/devices/me/telemetry'
client = mqtt.Client()
device = "cbd8e400-57cb-11ed-b795-b917a83a16b3"
port = 1883
client.username_pw_set(device)
client.connect(broker, port)

dict = dict()
ser = serial.Serial(port = '/dev/ttyACM0', baudrate=115200, timeout=1) 
print("Connected")
data_rows = []
header = ['x', 'y', 'z']
print(header)

while(1):
    data = ser.readline().decode('utf-8').replace('\r', "").replace('\n', "")
    data = data.split('\t')
    if len(data) == 3:
        print(data)
   
    msg = '{"Datos giroscopio":"'+ str(data) + '"}'
    client.publish(topic_pub, msg)
    sleep(0.1)
