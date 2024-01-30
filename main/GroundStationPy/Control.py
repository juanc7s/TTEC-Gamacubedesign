import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

from enum import Enum

import Protocol
protocol = Protocol.get_protocol()

# class Comm:
class ControlFrame(ttk.LabelFrame):
  def __init__(self, root, serial):
    tk.LabelFrame.__init__(self, master=root, text="Radio Control")
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

    self.ser = serial
    
    self.ser.add_listener(self, 'CONTROL')

    self.init_variables()
    self.init_layout()
  
  def init_variables(self):
    self.rx_addh_variable = tk.StringVar()
    self.rx_addl_variable = tk.StringVar()
    self.tx_addh_variable = tk.StringVar()
    self.tx_addl_variable = tk.StringVar()
    
    self.frequency_variable = tk.StringVar()
    self.spi_frequency_variable = tk.StringVar()
    
    self.idle_variable = tk.StringVar()
    self.sleep_variable = tk.StringVar()
    
    self.spreading_factor_variable = tk.StringVar()
    self.bandwidth_variable = tk.StringVar()
    self.coding_rate_variable = tk.StringVar()
    self.preamble_length_variable = tk.StringVar()
    self.sync_word_variable = tk.StringVar()
    self.crc_variable = tk.StringVar()
    self.tx_power_variable = tk.StringVar()

  def init_layout(self):
    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="rx ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.addh_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_addh).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.addl_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_addl).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_addl).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Channel")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.chan_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_channel).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_channel).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Parity')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['parity_dict'].keys()), textvariable=self.parity_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_parity).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_parity).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Operation mode')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['operation_mode_dict'].keys()), textvariable=self.operation_mode_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_operation_mode).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_operation_mode).pack(side='left',fill='both')

    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='UART baud rate')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['uart_baud_rate_dict'].keys()), textvariable=self.uart_baud_rate_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_uart_baud_rate).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_uart_baud_rate).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Air data rate')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['air_data_rate_dict'].keys()), textvariable=self.air_data_rate_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_air_data_rate).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_air_data_rate).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission power')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['transmission_power_dict'].keys()), textvariable=self.tx_power_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_power).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_power).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission mode')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['transmission_mode_dict'].keys()), textvariable=self.tx_mode_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_mode).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_mode).pack(side='left',fill='both')
  
  def begin(self, serial):
      self.ser = serial
      self.pack(fill='both')
  
  def close(self):
    self.pack_forget()
  
  def parse_message(self, cmd, args):
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

  def on_device_ready(self):
    pass
    # self.read_head()
    # self.read_addh()
    # self.read_addl()
    # self.read_channel()
    # self.read_parity()
    # self.read_uart_baud_rate()
    # self.read_air_data_rate()
    # self.read_tx_mode()
    # self.read_tx_power()
    # self.read_operation_mode()
  
  def set_head(self):
    self.ser.write([protocol['control_commands'].index("SET_HEAD"), int(self.head_variable.get(), 16)])
  def read_head(self):
    self.ser.write([protocol['control_commands'].index("READ_HEAD")])
  
  def set_addh(self):
    self.ser.write([protocol['control_commands'].index("SET_ADDH"), int(self.addh_variable.get(), 16)])
  def read_addh(self):
    self.ser.write([protocol['control_commands'].index("READ_ADDH")])
  
  def set_addl(self):
    self.ser.write([protocol['control_commands'].index("SET_ADDL"), int(self.addl_variable.get(), 16)])
  def read_addl(self):
    self.ser.write([protocol['control_commands'].index("READ_ADDL")])
  
  def set_channel(self):
    self.ser.write([protocol['control_commands'].index("SET_CHAN"), int(self.chan_variable.get(), 16)])
  def read_channel(self):
    self.ser.write([protocol['control_commands'].index("READ_CHAN")])
  
  def set_parity(self):
    self.ser.write([protocol['control_commands'].index("SET_PARITY"), protocol['parity_dict'][self.parity_variable.get()]])
  def read_parity(self):
    self.ser.write([protocol['control_commands'].index("READ_PARITY")])
  
  def set_uart_baud_rate(self):
    self.ser.write([protocol['control_commands'].index("SET_UART_BAUD_RATE"), protocol['uart_baud_rate_dict'][self.uart_baud_rate_variable.get()]])
  def read_uart_baud_rate(self):
    self.ser.write([protocol['control_commands'].index("READ_UART_BAUD_RATE")])
  
  def set_air_data_rate(self):
    self.ser.write([protocol['control_commands'].index("SET_AIR_DATA_RATE"), protocol['air_data_rate_dict'][self.air_data_rate_variable.get()]])
  def read_air_data_rate(self):
    self.ser.write([protocol['control_commands'].index("READ_AIR_DATA_RATE")])
  
  def set_tx_power(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_POWER"), protocol['transmission_power_dict'][self.tx_power_variable.get()]])
  def read_tx_power(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_POWER")])
  
  def set_tx_mode(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_MODE"), protocol['transmission_mode_dict'][self.tx_mode_variable.get()]])
  def read_tx_mode(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_MODE")])
  
  def set_operation_mode(self):
    self.ser.write([protocol['control_commands'].index("SET_OPERATION_MODE"), protocol['operation_mode_dict'][self.operation_mode_variable.get()]])
  def read_operation_mode(self):
    self.ser.write([protocol['control_commands'].index("READ_OPERATION_MODE")])

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