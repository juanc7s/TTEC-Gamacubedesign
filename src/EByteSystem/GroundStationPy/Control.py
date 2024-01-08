import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

import serial
from enum import Enum

control_commands = [
  "READ_HEAD",
  "READ_ADDH",
  "READ_ADDL",
  "READ_CHAN",
  "READ_AIR_DATA_RATE",
  "READ_UART_BAUD_RATE",
  "READ_PARITY",
  "READ_TX_POWER",
  "READ_TX_MODE",
  "READ_OPERATION_MODE",
  "READ_ALL",
  "SET_HEAD",
  "SET_ADDH",
  "SET_ADDL",
  "SET_CHAN",
  "SET_AIR_DATA_RATE",
  "SET_UART_BAUD_RATE",
  "SET_PARITY",
  "SET_TX_POWER",
  "SET_TX_MODE",
  "SET_OPERATION_MODE",
  "SET_ALL",
  "TOGGLE_TX",
  "SEND_TX",
  "SET_TX_ADDH",
  "SET_TX_ADDL",
  "SET_TX_CHAN",
  "FLUSH",
  "SET_MESSAGE_DATA"]

uart_baud_rate_dict = {
  '1200 bps':0,
  '2400 bps':1,
  '4800 bps':2,
  '9600 bps':3,
  '19200 bps':4,
  '38400 bps':5,
  '57600 bps':6,
  '115200 bps':7
}

air_data_rate_dict = {
  '300 bps':0,
  '1200 bps':1,
  '2400 bps':2,
  '4800 bps':3,
  '9600 bps':4,
  '19200 bps':5
}

parity_dict = {
  '8N1':ord('N'),
  '8O1':ord('O'),
  '8E1':ord('E')
}

transmission_power_dict = {
  '10 dBm':10,
  '14 dBm':14,
  '17 dBm':17,
  '20 dBm':20
}

transmission_mode_dict = {
  'TRANSPARENT TRANSMISSION':0,
  'FIXED TRANSMISSION':1
}

operation_mode_dict = {
  'NORMAL':0,
  'WAKE UP':1,
  'POWER SAVING':2,
  'SLEEP':3
}

head_dict = {
  'SAVE ON POWER DOWN':0xC0,
  'DONT SAVE ON POWER DOWN':0xC2
}

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
    self.transmission_flag = False
    self.receiving_buffer = bytes()
    self.receiving_flag = False

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
    self.toggle_transmission_variable = tk.StringVar(value='OFF')

  def init_layout(self):
    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.addh_variable,width=4,justify='center').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_addh).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.addl_variable,width=4,justify='center').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_addl).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_addl).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Channel")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.chan_variable,width=4,justify='center').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_channel).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_channel).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Parity')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(parity_dict.keys()), textvariable=self.parity_variable, state='readonly').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_parity).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_parity).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Operation mode')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(operation_mode_dict.keys()), textvariable=self.operation_mode_variable, state='readonly').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_operation_mode).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_operation_mode).pack(side='left',fill='both')

    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='UART baud rate')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(uart_baud_rate_dict.keys()), textvariable=self.uart_baud_rate_variable, state='readonly').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_uart_baud_rate).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_uart_baud_rate).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Air data rate')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(air_data_rate_dict.keys()), textvariable=self.air_data_rate_variable, state='readonly').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_air_data_rate).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_air_data_rate).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission power')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(transmission_power_dict.keys()), textvariable=self.tx_power_variable, state='readonly').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_power).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_power).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission mode')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(transmission_mode_dict.keys()), textvariable=self.tx_mode_variable, state='readonly').pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_mode).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_mode).pack(side='left',fill='both')

    # top_frame = ttk.LabelFrame(master=left_frame, text='')
    # top_frame.pack(side='top',fill='both')
    # ttk.Combobox(master=top_frame, values=_dict.keys(), textvariable=self._variable).pack(side='left',fill='both')
    # top_bottom_frame = ttk.Frame(master=top_frame)
    # top_bottom_frame.pack(side='bottom',fill='both')
    # ttk.Button(master=top_bottom_frame,text="read",command=self.read_).pack(side='left',fill='both')
    # ttk.Button(master=top_bottom_frame,text="set",command=self.set_).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission')
    top_frame.pack(side='top',fill='both')
    top_left_frame = ttk.LabelFrame(master=top_frame, text='Toggle')
    top_left_frame.pack(side='left',fill='both')
    tk.Button(master=top_left_frame, textvariable=self.toggle_transmission_variable, relief='raised', command=self.toggle_transmission).pack(side='left',fill='both')
    tk.Button(master=top_frame, text='SEND', relief='raised', command=self.send_transmission).pack(side='left',fill='both')
    
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
      b = self.ser.read()[0]
      if self.receiving_flag:
        if b==10:
          self.parse_message()
          self.receiving_buffer = bytes()
        else:
          self.receiving_buffer += b.to_bytes()
      elif b==13:
        self.receiving_flag = True
      else:
        self.receiving_buffer += b.to_bytes()
  
  def parse_message(self):
      # print(self.receiving_buffer)
      try:
        msg = str(self.receiving_buffer,encoding='utf8').rstrip()
        print(msg)
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
      except Exception as e:
        print(e)
  
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
    self.read_head()
    self.read_addh()
    self.read_addl()
    self.read_channel()
    self.read_parity()
    self.read_uart_baud_rate()
    self.read_air_data_rate()
    self.read_tx_mode()
    self.read_tx_power()
    self.read_operation_mode()
  
  def set_head(self):
    self.write([control_commands.index("SET_HEAD"), int(self.head_variable.get(), 16)])
  def read_head(self):
    self.write([control_commands.index("READ_HEAD")])
  
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
    self.write([control_commands.index("SET_PARITY"), parity_dict[self.parity_variable.get()]])
  def read_parity(self):
    self.write([control_commands.index("READ_PARITY")])
  
  def set_uart_baud_rate(self):
    self.write([control_commands.index("SET_UART_BAUD_RATE"), uart_baud_rate_dict[self.uart_baud_rate_variable.get()]])
  def read_uart_baud_rate(self):
    self.write([control_commands.index("READ_UART_BAUD_RATE")])
  
  def set_air_data_rate(self):
    self.write([control_commands.index("SET_AIR_DATA_RATE"), air_data_rate_dict[self.air_data_rate_variable.get()]])
  def read_air_data_rate(self):
    self.write([control_commands.index("READ_AIR_DATA_RATE")])
  
  def set_tx_power(self):
    self.write([control_commands.index("SET_TX_POWER"), transmission_power_dict[self.tx_power_variable.get()]])
  def read_tx_power(self):
    self.write([control_commands.index("READ_TX_POWER")])
  
  def set_tx_mode(self):
    self.write([control_commands.index("SET_TX_MODE"), transmission_mode_dict[self.tx_mode_variable.get()]])
  def read_tx_mode(self):
    self.write([control_commands.index("READ_TX_MODE")])
  
  def set_operation_mode(self):
    self.write([control_commands.index("SET_OPERATION_MODE"), operation_mode_dict[self.operation_mode_variable.get()]])
  def read_operation_mode(self):
    self.write([control_commands.index("READ_OPERATION_MODE")])

  def toggle_transmission(self):
    if self.toggle_transmission_variable.get() == 'ON':
      self.transmission_flag = False
      self.toggle_transmission_variable.set('OFF')
    elif self.toggle_transmission_variable.get() == 'OFF':
      self.transmission_flag = True
      self.toggle_transmission_variable.set('ON')
    self.write([control_commands.index("TOGGLE_TX"), self.transmission_flag])
  
  def send_transmission(self):
    self.write([control_commands.index("SEND_TX")])

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
#     'SEND_TX',
#     'SET_TX_ADDH',
#     'SET_TX_ADDL',
#     'SET_TX_CHAN',
#     'FLUSH',
#     'SET_MESSAGE_DATA'
#   ])