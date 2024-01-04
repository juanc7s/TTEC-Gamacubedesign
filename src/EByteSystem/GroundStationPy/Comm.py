import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

import serial

class Comm:
  def __init__(self):
    self.ser = None
    self.control_frame = None
    
  def begin(self, root, port, baudrate):
    if self.ser is not None:
      if self.ser.is_open:
        self.ser.close()
    
    self.control_frame = ControlFrame(self, root)
    self.ser = serial.Serial(port=port.device,baudrate=baudrate)

  def close(self):
    if self.ser is not None:
      if self.ser.is_open:
        self.ser.close()
        self.control_frame.destroy()

class ControlFrame(ttk.LabelFrame):
  def __init__(self, channel, root):
    tk.LabelFrame.__init__(self, master=root, text="Ground Station Control")
    self.channel = channel
    self.root = root

    self.init_variables()
    self.init_layout()

    self.pack(fill='both')
  
  def init_variables(self):
    self.addh = tk.StringVar()
    self.addl = tk.StringVar()
    self.chan = tk.StringVar()

  def init_layout(self):
    top_frame = ttk.LabelFrame(master=self,text="Set ADDH")
    top_frame.pack()
    ttk.Entry(master=top_frame,textvariable=self.addh).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_ADDH).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_ADDH).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=self,text="Set ADDL")
    top_frame.pack()
    ttk.Entry(master=top_frame,textvariable=self.addl).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_ADDL).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_ADDL).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=self,text="Set CHAN")
    top_frame.pack()
    ttk.Entry(master=top_frame,textvariable=self.chan).pack(side='left',fill='both')
    top_bottom_frame = ttk.Frame(master=top_frame)
    top_bottom_frame.pack(side='bottom',fill='both')
    ttk.Button(master=top_bottom_frame,text="read",command=self.read_CHAN).pack(side='left',fill='both')
    ttk.Button(master=top_bottom_frame,text="set",command=self.set_CHAN).pack(side='left',fill='both')
  
  def set_ADDH(self):
    pass
  def read_ADDH(self):
    pass
  
  def set_ADDL(self):
    pass
  def read_ADDL(self):
    pass
  
  def set_CHAN(self):
    pass
  def read_CHAN(self):
    pass
  
  def set_UART_baud_rate(self, uart_baud_rate):
    pass
  
  def set_air_data_rate(self, air_data_rate):
    pass
