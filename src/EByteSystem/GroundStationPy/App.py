import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

import serial.tools.list_ports

import Comm

class App(tk.Tk):
  def __init__(self):
    tk.Tk.__init__(self)
    self.title('GamaCubedesign Groud Station')

    self.comm = Comm.Comm()
    
    self.init_variables()
    self.init_layout()
    # self.mcu_frame = tk.Column([[self.active_mcu]])
    self.detect_ports()

  def init_variables(self):
    self.port_variable = tk.StringVar()
    self.baudrate = tk.StringVar()
    self.baudrate.set('57600')
    self.open_close_variable = tk.StringVar()
    self.open_close_variable.set("Open channel")
  
  def init_layout(self):
    top_frame = ttk.Frame(master=self)
    top_frame.pack()
    port_frame = ttk.LabelFrame(master=top_frame, text="Port")
    port_frame.pack(side='left', fill='both')
    ttk.Button(master=port_frame,text='refresh',command=self.detect_ports).pack(side='left', fill='both')
    self.port_combobox = ttk.Combobox(master=port_frame,values=[],textvariable=self.port_variable)
    self.port_combobox.pack(side='left', fill='both')
    baudrate_frame = ttk.LabelFrame(master=top_frame,text='Baud rate')
    baudrate_frame.pack(side='left', fill='both')
    ttk.Entry(master=baudrate_frame,textvariable=self.baudrate).pack(side='top', fill='both')
    open_frame = ttk.Frame(master=top_frame)
    open_frame.pack(side='left',fill='both')
    ttk.Button(master=open_frame,textvariable=self.open_close_variable,command=self.open_close_channel).pack(side='top',fill='both')

    self.control_frame = tk.Frame(master=self)
  
  def detect_ports(self):
    self.ports_dict = dict()
    ports = serial.tools.list_ports.comports()
    for port in ports:
      self.ports_dict[port.name] = port
    self.port_combobox['values']=[port for port in self.ports_dict.keys()]
    if len(self.ports_dict)>0:
      self.port_variable.set(list(self.ports_dict.values())[0].name)
  
  def open_close_channel(self):
    if self.open_close_variable.get()=="Open channel":
      self.comm.begin(self.control_frame, self.ports_dict[self.port_combobox.get()], self.baudrate.get())
      self.open_close_variable.set("Close channel")
      self.control_frame.pack(side='bottom',fill='both')
    elif self.open_close_variable.get()=="Close channel":
      self.comm.close()
      self.open_close_variable.set("Open channel")
      self.control_frame.pack_forget()
      # self.control_frame.pack()


if __name__ == "__main__":
  app = App()

  app.mainloop()