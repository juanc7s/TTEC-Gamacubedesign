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

    top_frame = ttk.LabelFrame(master=left_frame,text="Rx ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.rx_addh_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_rx_addh).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_rx_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text=" Rx ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.rx_addl_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_rx_addl).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_rx_addl).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Tx ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.tx_addh_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_addh).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text=" Tx ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.tx_addl_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_addl).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_addl).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Frequency")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.frequency_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_frequency).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_frequency).pack(side='left',fill='both')

    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='SPI frequency')
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.spi_frequency_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_spi_frequency).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_spi_frequency).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Spreading factor')
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.spreading_factor_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_spreading_factor).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_spreading_factor).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission power')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['transmission_power_dict'].keys()), textvariable=self.tx_power_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_power).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_power).pack(side='left',fill='both')
  
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
  
  def set_rx_addh(self):
    self.ser.write([protocol['control_commands'].index("SET_RX_ADDH"), int(self.rx_addh_variable.get(), 16)])
  def read_rx_addh(self):
    self.ser.write([protocol['control_commands'].index("READ_RX_ADDH")])
  
  def set_rx_addl(self):
    self.ser.write([protocol['control_commands'].index("SET_RX_ADDL"), int(self.rx_addl_variable.get(), 16)])
  def read_rx_addl(self):
    self.ser.write([protocol['control_commands'].index("READ_RX_ADDL")])
  
  def set_tx_addh(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_ADDH"), int(self.tx_addh_variable.get(), 16)])
  def read_tx_addh(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_ADDH")])
  
  def set_tx_addl(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_ADDL"), int(self.tx_addl_variable.get(), 16)])
  def read_tx_addl(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_ADDL")])
  
  def set_frequency(self):
    self.ser.write([protocol['control_commands'].index("SET_FREQUENCY"), int(self.frequency_variable.get(), 16)])
  def read_frequency(self):
    self.ser.write([protocol['control_commands'].index("READ_FREQUENCY")])
  
  def set_spi_frequency(self):
    self.ser.write([protocol['control_commands'].index("SET_SPI_FREQUENCY"), int(self.spi_frequency_variable.get())])
  def read_spi_frequency(self):
    self.ser.write([protocol['control_commands'].index("READ_SPI_FREQUENCY")])
  
  def set_spreading_factor(self):
    self.ser.write([protocol['control_commands'].index("SET_SPREADING_FACTOR"), int(self.spreading_factor_variable.get())])
  def read_spreading_factor(self):
    self.ser.write([protocol['control_commands'].index("READ_SPREADING_FACTOR")])
  
  def set_tx_power(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_POWER"), protocol['transmission_power_dict'][self.tx_power_variable.get()]])
  def read_tx_power(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_POWER")])