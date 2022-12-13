import serial
import time
import kbhit
import json

# COM port setting
COM_PORT = "your_COM_Port"

# WiFI Credentials
SSID = "your_SSID"
PASSPHRASE = "your_PASSPHRASE" 

# Azure Application/Device Information
ID_SCOPE = "your_ID_SCOPE"
DEVICE_ID = "your_DEVICE_ID"
MODEL_ID = "your_MODEL_ID"

COM_PORT = "COM4"
SSID = "JasonWiFi"
PASSPHRASE = "rauch:net:secure" 
ID_SCOPE = "0ne006ED32B"
DEVICE_ID = "sn0123FE0CF960432D01"
MODEL_ID = "dtmi:com:Microchip:SAM_IoT_WM;2"

class Polling_KB_CMD_Input:
  def __init__(self):
    self.kb = kbhit.KBHit()
    self.input_buf = ""
    self.cmd = ""
    self.EXIT_KEY = 27 #ESC
  def poll_keyboard(self):
    if self.kb.kbhit():
      c = self.kb.getch()
      if ord(c) == self.EXIT_KEY:
        return False
      else:
        self.input_buf += c
        if ord(c) == 13: #Carriage Return (CR)
          self.cmd = self.input_buf
          self.input_buf = ""
          return True
      return True
  def cmd_get(self):
    return self.cmd
  def cmd_received(self):
    if(self.cmd != ""):
      return True
    else:
      return False
  def cmd_clear(self):
    self.cmd = ""
  def __del__(self):
    self.kb.set_normal_term()  

class Delay_Non_Blocking:
  def __init__(self):
    self.isStarted = False
    self.time_start = 0

  def delay_time_start(self):
    if self.isStarted == False:
      self.time_start = time.time()
      self.isStarted = True
    
  def delay_sec_poll(self, delay_sec):
    if time.time() - self.time_start > delay_sec :
      self.isStarted = False
      return True
    else:
      return False

class AnyCloud:
  def __init__(self, port, baud, debug):
    self.PORT = port
    self.BAUD = baud
    self.DEBUG = debug
    self.SER_TIMEOUT = 0.1
    self.ser_buf = ""
    self.init_state = 0
    self.sub_state = 0
    self.app_state = 0
    self.wifi_connected = False
    self.broker_connected = False
    self.broker_topics_subs = 0
    self.pub_topic = ""
    self.pub_payload = ""
    self.sub_payload = ""
    self.opId = ""
    self.ser = serial.Serial(self.PORT, self.BAUD, timeout = self.SER_TIMEOUT)
    self.delay = Delay_Non_Blocking()
    self.kb = Polling_KB_CMD_Input()
    self.evt_handler = None
  def cmd_issue(self, command):
    self.ser.write(bytearray(command, 'utf-8'))
  
  def serial_recieve(self):
    read_val = self.ser.read(1)
    if read_val != b'':
      if self.DEBUG == True:
        print(read_val)
      self.ser_buf = self.ser_buf + read_val.decode('utf8', errors='backslashreplace')
      if read_val == b'>':
        ret_val = self.ser_buf
        self.ser_buf = ""
        return ret_val
    return ""        
  
  def sm_initialize(self):
      # start initialization
      if self.init_state == 0:
        print("\r\nStart Initialization...\r\n.............................\r\n")
        self.init_state = 1
      self.delay.delay_time_start()
      if self.delay.delay_sec_poll(0.100) :
        #enable serial command echo
        if self.init_state == 1:
          self.cmd_issue('ATE1\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        
        # default verbosity set to 5, which is max.
        #if self.init_state == 1:
        #  self.cmd_issue('ATV=5\r\n')
        #  self.init_state = self.init_state + 1
        #  return self.init_state
                
        # wifi initialization, starting with check for existing WiFi connection
        elif self.init_state == 2:
          self.cmd_issue('AT+WSTA\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 3:
          self.cmd_issue('AT+WSTAC=1,"'+SSID+'"\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 4:
          self.cmd_issue('AT+WSTAC=2,3\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 5:
          self.cmd_issue('AT+WSTAC=3,"' + PASSPHRASE + '"\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 6:
          self.cmd_issue('AT+WSTAC=4,255\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 7:
          self.cmd_issue('AT+WSTAC=12,"pool.ntp.org"\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 8:
          self.cmd_issue('AT+WSTAC=13,1\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 9:
          self.cmd_issue('AT+WSTA=1\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        
        #delay until event changes init_state
        elif self.init_state == 10: 
          # don't advance state automatically, do it from evt_handler
          pass
        
        elif self.init_state == 11:
          self.cmd_issue('AT+MQTTCONN\r\n')
          self.init_state = self.init_state + 1
          return self.init_state  
        
        # configure and connect to MQTT broker
        elif self.init_state == 12: #dps broker
          self.cmd_issue('AT+MQTTC=1,"iotc-524cd5bf-f583-46a6-9ac4-873783da764e.azure-devices.net"\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 13: #dps broker port (TLS)
          self.cmd_issue('AT+MQTTC=2,8883\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 14: #IoTC MQTT Client ID
          self.cmd_issue('AT+MQTTC=3,"' + DEVICE_ID + '"\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 15: #IoTC Username
          self.cmd_issue('AT+MQTTC=4,"iotc-524cd5bf-f583-46a6-9ac4-873783da764e.azure-devices.net/sn0123FE0CF960432D01"\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
        elif self.init_state == 16: #enable TLS
          self.cmd_issue('AT+MQTTC=7,1\r\n')
          self.init_state = self.init_state + 1
          return self.init_state        
        elif self.init_state == 17:
          self.cmd_issue('AT+MQTTCONN=1\r\n') # connect
          #goto delay state and wait for connect to broker event
          self.init_state = 10                            
          return self.init_state
        
        # init complete        
        elif self.init_state == 254:
          return self.init_state          
        else:
          return 255
      else:
        return 0
  
  def sm_iotc_register(self):
      if self.sub_state == 0:
     #   print("subscribe to topics")
     #   self.sub_state = self.sub_state + 1
     #   return self.sub_state
     # elif self.sub_state == 1:
     #   self.mqtt_subscribe("\"$dps/registrations/res/#\"", 0)
     #   self.sub_state = 2 #delay for evt_topic_subscribed
     #   return self.sub_state
     # elif self.sub_state == 2:
     #   #delay here until event moves to next state
     #   return self.sub_state
     # elif self.sub_state == 3:
     #   self.mqtt_publish(0,0,"$dps/registrations/PUT/iotdps-register/?rid=1",('{\\\"payload\\\" : {\\\"modelId\\\" : \\\"' + MODEL_ID + '\\\"}}'))
     #   self.sub_state = 2 #delay for evt_topic_published
     #   return self.sub_state  
     # elif self.sub_state == 4:
        self.delay.delay_time_start()
        if self.delay.delay_sec_poll(2.0):
          self.mqtt_publish(0,0,"devices/sn0123FE0CF960432D01/messages/events/","{\\\"telemetry_Str_1\\\" : \\\"Hello World\\\"}" )
          self.sub_state = 254 #delay for evt_topic_published
        return self.sub_state    
      elif self.sub_state == 254:
        return self.sub_state
      else:
        print("subscriptions error")
        
  def mqtt_subscribe(self, topic, iQOS):
    cmd = "AT+MQTTSUB=" + topic +","+str(iQOS) +'\r\n'
    self.cmd_issue(cmd)
  
  def mqtt_publish(self, iQoS, iRetain, strTopic, strPayload):
    try:  #try blick looks for CR, and removes it if present before joining CMD
      loc = strPayload.index('\r')
    except ValueError:
      pass
    else:
      strPayload = strPayload[0:loc]
    cmd = "AT+MQTTPUB=0," + str(iQoS)+','+ str(iRetain)+ ',\"' + strTopic + '\",\"' + strPayload + '\"\r\n'
    self.cmd_issue(cmd)
    
  def evt_init_error(self):
    self.init_state = 254
    print("Event: Error,stopping initialization")
  
  def evt_wifi_connected(self):
    if self.wifi_connected == True :
      self.init_state = 11 #no WiFi Init needed, jump to MQTT configure and connect
      print("Event: WiFi connected")    
    else :
      print("Event: WiFi not connected, initialializing")
  
  def evt_dps_broker_connected(self):
     self.init_state = 254
     self.sub_state = 0
     #self.app_state = 2
     self.broker_connected = True
     print("Event: MQTT broker connected")
  
  def evt_dps_topic_subscribed(self):
    print("\r\nEvent: Subscribed to DPS topics, publish registration request....\r\n")
    self.broker_topics_subs = self.broker_topics_subs + 1
    if self.broker_topics_subs == 1 :
      self.sub_state = 3 #publish registration
    else:
      print("Event:  Error, wasn't expecting multiple subscriptions for DPS server")
    
  def evt_dps_topic_notified(self):
    print("Event: DPS subscription recieved notification")
    if self.opId == "":
      topic_start = self.sub_payload.find(',') + 1
      topic_len = int(self.sub_payload[10:self.sub_payload.find(',')])+2
      topic = self.sub_payload[topic_start:topic_start+topic_len]
      print("topic: " + topic)
      payload_len_str_start = topic_start + topic_len + 1
      payload_len_str_end = payload_len_str_start + int(self.sub_payload[(payload_len_str_start):].find(','))
      payload_len = int(self.sub_payload[payload_len_str_start:payload_len_str_end])
      #print("S:" + str(payload_len_str_start) + " P:" + str(payload_len_str_end) + " payload len: " + str(payload_len))
      json_payload = json.loads(self.sub_payload[(payload_len_str_end+2):(payload_len_str_end + payload_len + 2)])
      print(json.dumps(json_payload, indent = 4))
      #print("operationId: ", json_payload["operationId"])
      self.opId = json_payload["operationId"]
      self.sub_payload = ""
      self.sub_state = 4

  def kb_data_process(self, received):
    if received.startswith("AT"):
      self.pub_topic = None
      self.sub_topic = None
      return True
    else:
      return False
    
  def rx_data_process(self, received):
    # if error setting echo on, bail
    if ("ATE1" in received) and ("ERROR:" in received) :
      self.evt_handler = self.evt_init_error
      retval = 0 #error state
    
    if ("AT+WSTAC" in received) and ("ERROR:" in received) :
      self.evt_handler = self.evt_init_error
      retval = 0 #error state
    
    if ("+WSTA:1" in received) :
      self.wifi_connected = True
      self.evt_handler = self.evt_wifi_connected
      ret_val = 1 #operating state
    
    if ("+WSTA:0" in received) :
      self.wifi_connected = False
      self.evt_handler = self.evt_wifi_connected    
      ret_val = 1 #operating state
    
    if ("+WSTAAIP:" in received) :
      self.wifi_connected = True
      self.evt_handler = self.evt_wifi_connected    
      ret_val = 1 #operating state
    
    if ("+MQTTCONN:0" in received) :
      print("\r\nBroker connection not found, connecting...\r\n");
      ret_val = 1 #operating state
      
    if ("+MQTTCONN:1" in received) :
      if self.app_state == 1:
        self.evt_handler = self.evt_dps_broker_connected    
      ret_val = 1 #operating state
    
    if("+MQTTSUB:0" in received) :
      self.evt_handler = self.evt_dps_topic_subscribed    
      ret_val = 1
      
    if ("+MQTTPUB:" in received and "$dps/registrations/res" in received) :
      if self.sub_payload == "" :
        self.sub_payload = received
        self.evt_handler = self.evt_dps_topic_notified
      else:
        print("warning: topic notification ignored, last topic still being processed") 
      ret_val = 1 #operating state    
  
  def runApp(self):
    # wait for keyboard events
    if self.kb.poll_keyboard() == False:
      exit()
    else:
      if self.kb.cmd_received():
        kb_received = self.kb.cmd_get()
        print(kb_received)
        pubFound = self.kb_data_process(kb_received)
        if pubFound == True:
          if self.pub_topic == None :
            print("AT Command = " +kb_received);
            self.ser.write((kb_received + '\n').encode())
          else:
            self.mqtt_publish(0,0, self.pub_topic, self.pub_payload)
        else:
          print("publish command not found")
        
      self.kb.cmd_clear()
 
    #top level app state machine
    if self.app_state == 0:  # start of application
      print("\r\n--------------------------------------------------------------------------------")
      print("Starting the AnyCloud Azure IoT Central Demonstration")
      print("--------------------------------------------------------------------------------\r\n")
      print('Once device provisioning is complete, enter AT commands to continue.\r\nPress ESC to Exit')
      self.app_state = 1
    
    elif self.app_state == 1:  # init AnyCloud
      init_resp = self.sm_initialize()
      if init_resp == 254 :
        print("\r\nStart DPS registration...\r\n")
        self.app_state = 2
    
    elif self.app_state == 2: # subscribe to dps topics
      sub_resp = self.sm_iotc_register()
      if sub_resp == 254 :
        print("Registration complete")
        self.app_state = 3
    
    elif self.app_state == 3:
      print("Enter AT command to execute")
      self.app_state = 4
      
    elif self.app_state == 4:
      
      self.app_state = 5
      pass
    
    elif self.app_state == 5:
      print("press ESC to end")
      self.app_state = self.app_state + 1
    
    elif self.app_state == 6:
      #if you get here, do nothing while waiting for ESC
      pass
      
    rx_data = self.serial_recieve()
    # parse received data
    if rx_data != "":
      print(rx_data)
      self.rx_data_process(rx_data)
      if self.evt_handler != None :
        self.evt_handler()
        self.evt_handler = None  
  
  def __del__(self):
    self.ser.close()

ac = AnyCloud(COM_PORT, 230400, False)

while True:
  
  ac.runApp()

  
      