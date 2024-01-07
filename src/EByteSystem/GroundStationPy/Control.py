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
  "READ_OPERATION_MODE",
  "READ_ALL",
  "SET_ADDH",
  "SET_ADDL",
  "SET_CHAN",
  "SET_AIR_DATA_RATE",
  "SET_UART_BAUD_RATE",
  "SET_PARITY",
  "SET_TRANSMISSION_POWER",
  "SET_TRANSMISSION_MODE",
  "SET_OPERATION_MODE",
  "SET_ALL",
  "TOGGLE_COMMUNICATION",
  "COMMUNICATE",
  "SET_TX_ADDH",
  "SET_TX_ADDL",
  "SET_TX_CHAN",
  "FLUSH",
  "SET_MESSAGE_DATA"]

uart_baud_rate_list = [
  '1200 bps',
  '2400 bps',
  '4800 bps',
  '9600 bps',
  '19200 bps',
  '38400 bps',
  '57600 bps',
  '115200 bps'
]

air_data_rate_list = [
  '300 bps',
  '1200 bps',
  '2400 bps',
  '4800 bps',
  '9600 bps',
  '19200 bps'
]

parity_list = [
  '8N1',
  '8O1',
  '8E1'
]

transmission_power_list = [
  '10 dBm',
  '14 dBm',
  '17 dBm',
  '20 dBm'
]

transmission_mode_list = [
  'TRANSPARENT TRANSMISSION',
  'FIXED TRANSMISSION'
]

operation_mode_list = [
  'NORMAL',
  'WAKE UP',
  'POWER SAVING',
  'SLEEP'
]

head_list = [
  'SAVE ON POWER DOWN',
  'DONT SAVE ON POWER DOWN'
]

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
    self.head_variable = tk.StringVar()
    self.addh_variable = tk.StringVar()
    self.addl_variable = tk.StringVar()
    self.chan_variable = tk.StringVar()
    self.uart_baud_rate_variable = tk.StringVar()
    self.air_data_rate_variable = tk.StringVar()
    self.tx_power_variable = tk.StringVar()
    self.tx_mode_variable = tk.StringVar()
    self.operation_mode_variable = tk.StringVar()
    self.parity_variable = tk.StringVar()

  def init_layout(self):
    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.addh_variable,width=4,justify='center').pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_addh).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=self,text="ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.addl_variable,width=4,justify='center').pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_addl).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_addl).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=self,text="Channel")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.chan_variable,width=4,justify='center').pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_channel).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_channel).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Parity')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=parity_list, textvariable=self.parity_variable).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_parity).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_parity).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Operation mode')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=operation_mode_list, textvariable=self.operation_mode_variable).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_operation_mode).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_operation_mode).pack(side='left',fill='both')

    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='UART baud rate')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=uart_baud_rate_list, textvariable=self.uart_baud_rate_variable).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_uart_baud_rate).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_uart_baud_rate).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Air data rate')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=air_data_rate_list, textvariable=self.air_data_rate_variable).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_air_data_rate).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_air_data_rate).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission power')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=transmission_power_list, textvariable=self.tx_power_variable).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_tx_power).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_tx_power).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission mode')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=transmission_mode_list, textvariable=self.tx_mode_variable).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_tx_mode).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_tx_mode).pack(side='left',fill='both')

    # top_frame = ttk.LabelFrame(master=left_frame, text='')
    # top_frame.pack(side='top',fill='both')
    # ttk.Combobox(master=top_frame, values=_list, textvariable=self._variable).pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    # ttk.Button(master=top_bottom_frame,text="read",command=self.read_).pack(side='left',fill='both')
    # ttk.Button(master=top_bottom_frame,text="set",command=self.set_).pack(side='left',fill='both')
    
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
    while self.ser.in_waiting > 0:
      msg = self.ser.readline()
      print(msg)
      msg = str(msg,encoding='utf8').rstrip()
      if len(msg)>2:
        v = msg.split(':')
        if v[0] == "CONTROL":
          cmd = v[1]
          args = v[2:]
          if cmd=="Device initiated successfully":
            self.on_device_ready()
          elif cmd == "HEAD":
            self.head_variable.set(hex(int(args[0], 16))[2:].upper())
          elif cmd == "ADDH":
            self.addh_variable.set(hex(int(args[0], 16))[2:].upper())
          elif cmd == "ADDL":
            self.addl_variable.set(hex(int(args[0], 16))[2:].upper())
          elif cmd == "CHAN":
            self.chan_variable.set(hex(int(args[0], 16))[2:].upper())
          elif cmd == "PARITY":
            self.parity_variable.set(args[0])
          elif cmd == "UART_BAUD_RATE":
            self.uart_baud_rate_variable.set(args[0])
          elif cmd == "AIR_DATA_RATE":
            self.air_data_rate_variable.set(args[0])
          elif cmd == "TXPW":
            self.tx_power_variable.set(args[0])
          elif cmd == "TXMO":
            self.tx_mode_variable.set(args[0])
          elif cmd == "OPMO":
            self.operation_mode_variable.set(args[0])
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
    self.read_addh()
    self.read_addl()
    self.read_channel()
  
  def set_head(self):
    pass
  def read_head(self):
    pass
  
  def set_addh(self):
    self.write([control_commands.index("SET_ADDH"), int(self.addh_variable.get(), 16)])
  def read_addh(self):
    self.write([control_commands.index("READ_ADDH")])
  
  def set_addl(self):
    self.write([control_commands.index("SET_ADDL"), int(self.addl_variable.get(), 16)])
  def read_addl(self):
    self.write([control_commands.index("READ_ADDL")])
  
  def set_channel(self):
    self.write([control_commands.index("SET_CHAN"), int(self.chan_variable.get(), 16)])
  def read_channel(self):
    self.write([control_commands.index("READ_CHAN")])
  
  def set_parity(self):
    pass
  def read_parity(self):
    pass
  
  def set_uart_baud_rate(self):
    self.write([control_commands.index("SET_UART_BAUD_RATE", int(self.uart_baud_rate_variable.get()[:-4]))])
  def read_uart_baud_rate(self):
    self.write([control_commands.index("READ_UART_BAUD_RATE")])
  
  def set_air_data_rate(self):
    self.write([control_commands.index("SET_AIR_DATA_BAUD_RATE", int(self.air_data_rate_variable.get()[:-4]))])
  def read_air_data_rate(self):
    self.write([control_commands.index("READ_AIR_DATA_RATE")])
  
  def set_tx_power(self):
    pass
  def read_tx_power(self):
    pass
  
  def set_tx_mode(self):
    pass
  def read_tx_mode(self):
    pass
  
  def set_operation_mode(self):
    pass
  def read_operation_mode(self):
    pass
  
  # def entry_changed(in):






# ControlCommand = Enum('Control command', 
#   [
#     'READ_ADDH',
#     'READ_ADDL',
#     'READ_CHAN',
#     'READ_AIR_DATA_RATE',
#     'READ_UART_BAUD_RATE',
#     'READ_PARITY',
#     'READ_TRANSMISSION_POWER',
#     'READ_TRANSMISSION_MODE',
#     'READ_ALL',
#     'SET_ADDH',
#     'SET_ADDL',
#     'SET_CHAN',
#     'SET_AIR_DATA_RATE',
#     'SET_UART_BAUD_RATE',
#     'SET_PARITY',
#     'SET_TRANSMISSION_POWER',
#     'SET_ALL',
#     'TOGGLE_COMMUNICATION',
#     'COMMUNICATE',
#     'SET_TX_ADDH',
#     'SET_TX_ADDL',
#     'SET_TX_CHAN',
#     'FLUSH',
#     'SET_MESSAGE_DATA'
#   ])