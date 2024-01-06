import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

import serial
from enum import Enum

control_commands = [
  "READ_ADDH",
  "READ_ADDL",
  "READ_CHAN",
  "READ_AIR_DATA_RATE",
  "READ_UART_BAUD_RATE",
  "READ_PARITY",
  "READ_TRANSMISSION_POWER",
  "READ_TRANSMISSION_MODE",
  "READ_ALL",
  "SET_ADDH",
  "SET_ADDL",
  "SET_CHAN",
  "SET_AIR_DATA_RATE",
  "SET_UART_BAUD_RATE",
  "SET_PARITY",
  "SET_TRANSMISSION_POWER",
  "SET_ALL",
  "TOGGLE_COMMUNICATION",
  "COMMUNICATE",
  "SET_TX_ADDH",
  "SET_TX_ADDL",
  "SET_TX_CHAN",
  "FLUSH",
  "SET_MESSAGE_DATA"]

ControlCommand = Enum('Control command', 
  [
    'READ_ADDH',
    'READ_ADDL',
    'READ_CHAN',
    'READ_AIR_DATA_RATE',
    'READ_UART_BAUD_RATE',
    'READ_PARITY',
    'READ_TRANSMISSION_POWER',
    'READ_TRANSMISSION_MODE',
    'READ_ALL',
    'SET_ADDH',
    'SET_ADDL',
    'SET_CHAN',
    'SET_AIR_DATA_RATE',
    'SET_UART_BAUD_RATE',
    'SET_PARITY',
    'SET_TRANSMISSION_POWER',
    'SET_ALL',
    'TOGGLE_COMMUNICATION',
    'COMMUNICATE',
    'SET_TX_ADDH',
    'SET_TX_ADDL',
    'SET_TX_CHAN',
    'FLUSH',
    'SET_MESSAGE_DATA'
  ])

# class Comm:
class ControlFrame(ttk.LabelFrame):
  def __init__(self, root):
    tk.LabelFrame.__init__(self, master=root, text="Ground Station Control")
    self.root = root

    self.configuration = {
      "Head": None,
      "ADDH": None,
      "ADDL": None,
      "CHAN": None,
      "UART parity bit": None,
      "TTL UART baud rate (bps)": None,
      "Air data rate (bps)": None,
      "Transmission mode": None,
      "Transmission power": None
    }

    self.ser = None

    self.init_variables()
    self.init_layout()
  
  def init_variables(self):
    self.addh_variable = tk.StringVar()
    self.addl_variable = tk.StringVar()
    self.chan_variable = tk.StringVar()

  def init_layout(self):
    top_frame = ttk.LabelFrame(master=self,text="Set ADDH")
    top_frame.pack()
    ttk.Entry(master=top_frame,textvariable=self.addh_variable,width=4,justify='center').pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_ADDH).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_ADDH).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=self,text="Set ADDL")
    top_frame.pack()
    ttk.Entry(master=top_frame,textvariable=self.addl_variable,width=4,justify='center').pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_ADDL).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_ADDL).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=self,text="Set CHAN")
    top_frame.pack()
    ttk.Entry(master=top_frame,textvariable=self.chan_variable,width=4,justify='center').pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_CHAN).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_CHAN).pack(side='left',fill='both')
    
  def begin(self, port, baudrate):
    if self.ser is not None:
      if self.ser.is_open:
        self.ser.close()
    
    try:
      self.ser = serial.Serial(port=port.device,baudrate=baudrate,timeout=0.1)
      self.pack(fill='both')
    except Exception as e:
      print(e)
      return False
    return True

  def close(self):
    if self.ser is not None:
      if self.ser.is_open:
        self.ser.close()
    
    self.pack_forget()
  
  def write(self, message):
    print(self.encode(message)+[10])
    self.ser.write(self.encode(message)+[10])
  
  def read(self):
    # print(b"Device initiated successfully")
    while self.ser.in_waiting > 0:
      msg = self.ser.readline()
      msg = str(msg,encoding='utf8').rstrip()
      print(msg[:])
      if len(msg)>2:
        # msg = msg[:-2]
        v = msg.split(':')
        if v[0] == "CONTROL":
          cmd = v[1]
          args = v[2:]
          if cmd=="Device initiated successfully":
            self.on_device_ready()
          elif cmd == "ADDH":
            self.addh_variable.set(hex(int(args[0], 16))[2:].upper())
          elif cmd == "ADDL":
            self.addl_variable.set(hex(int(args[0], 16))[2:].upper())
          elif cmd == "CHAN":
            self.chan_variable.set(hex(int(args[0], 16))[2:].upper())
        else:
          pass
            # v = msg.split(sep=': ')
            # parameter = v[0]
            # value = v[1]
  
  def update(self):
    self.read()
  
  def encode(self, message):
    out = []
    for c in message:
      if c==10:
        out += [13]
      if c==13:
        out += [13]
      out += [c]
    return out

  def on_device_ready(self):
    self.read_ADDH()
    self.read_ADDL()
    self.read_CHAN()
  
  def set_ADDH(self):
    self.write([control_commands.index("SET_ADDH"), int(self.addh_variable.get(), 16)])
  def read_ADDH(self):
    self.write([control_commands.index("READ_ADDH")])
  
  def set_ADDL(self):
    pass
  def read_ADDL(self):
    self.write([control_commands.index("READ_ADDL")])
  
  def set_CHAN(self):
    pass
  def read_CHAN(self):
    self.write([control_commands.index("READ_CHAN")])
  
  def set_UART_baud_rate(self, uart_baud_rate):
    pass
  def read_UART_baud_rate(self, uart_baud_rate):
    pass
  
  def set_air_data_rate(self, air_data_rate):
    pass
  def read_air_data_rate(self, air_data_rate):
    pass
