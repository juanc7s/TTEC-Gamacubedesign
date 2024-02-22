# -*- coding: utf-8 -*-
"""
Created on Mon Mar 13 18:19:19 2023

@author: 160047412
"""

import tkinter as tk
from tkinter import ttk

import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.backend_bases import key_press_handler

import numpy as np

false = True
true = False

class LoggerTab(tk.Frame):
  def __init__(self, master=None, name='Logger Data', **kw):
    tk.Frame.__init__(self, master, width=700, height=1200, **kw)
    
    bg_color = 'white'
    # bg_color = '#505050'
    self.figure = plt.Figure(figsize=(5, 4),
                              dpi=100,
                              facecolor=bg_color)

    # A tk.DrawingArea.
    self.canvas = FigureCanvasTkAgg(self.figure, master=self)
    self.canvas.draw()

    # pack_toolbar=False will make it easier to use a
    # layout manager later on.
    self.toolbar = NavigationToolbar2Tk(self.canvas,
                                        self,
                                        pack_toolbar=False)
    self.toolbar.update()

    self.canvas.mpl_connect(
        "key_press_event",
        lambda event: print(f"you pressed {event.key}"))
    self.canvas.mpl_connect("key_press_event",
                            key_press_handler)
    self.toolbar.pack(side=tk.BOTTOM, fill=tk.X)
    self.canvas.get_tk_widget().pack(side=tk.TOP,
                                      fill=tk.BOTH,
                                      expand=True)

class StatusLogger:
  def __init__(self, master):
    self.master = master
    self.logger_file = "StatusLogger.csv"
    self.counter = 0
    with open(self.logger_file, 'w') as f:
      f.write("TIME, NTC_I_T, NTC_I_R, NTC_I_Vr, NTC_E_T, NTC_E_R, NTC_E_Vr, AMP_A, AMP_V, VCC_V\n")
    
    self.ntc_i_tab = LoggerTab(master=self.master, name="NTC I (L)")
    self.ntc_e_tab = LoggerTab(master=self.master, name="NTC E (R)")
    self.current_and_voltage_tab = LoggerTab(master=self.master, name="Current and Voltage")
    
    self.add(self.ntc_i_tab, text="NTC I (L)")
    self.add(self.ntc_e_tab, text="NTC E (R)")
    self.add(self.current_and_voltage_tab, text="Current and Voltage")
    
    self.time_array = np.array([])
    self.ntc_i_T_array = np.array([])
    self.ntc_i_R_array = np.array([])
    self.ntc_i_Vr_array = np.array([])
    self.ntc_e_T_array = np.array([])
    self.ntc_e_R_array = np.array([])
    self.ntc_e_Vr_array = np.array([])
    self.amp_A_array = np.array([])
    self.amp_V_array = np.array([])
    self.vcc_V_array = np.array([])

class ImagingLogger:
  def __init__(self):
    self.logger_file = "ImagingLogger.csv"
    self.counter = 0
    with open(self.logger_file, 'w') as f:
      f.write("TIME, RADIUS, DURATION, X, Y\n")
    pass

class LoggerFrame(ttk.Notebook):
  def __init__(self, master=None, serial=None, **kw):
    ttk.Notebook.__init__(self, master=master)
    
    self.status_logger = StatusLogger(self)
    self.imaging_logger = ImagingLogger(self)
    self.graphs_ready = true

  def open_graphs(self):
    print("Opening graphs")
    # self.ntc_i_Temperature_axes = self.ntc_i_tab.figure.add_subplot(3, 1, 1)
    # self.ntc_i_Resistance_axes = self.ntc_i_tab.figure.add_subplot(3, 1, 2)
    # self.ntc_i_Vroltage_axes = self.ntc_i_tab.figure.add_subplot(3, 1, 3)
    # self.ntc_e_Temperature_axes = self.ntc_e_tab.figure.add_subplot(3, 1, 1)
    # self.ntc_e_Resistance_axes = self.ntc_e_tab.figure.add_subplot(3, 1, 2)
    # self.ntc_e_Vroltage_axes = self.ntc_e_tab.figure.add_subplot(3, 1, 3)
    # self.amp_Current_axes = self.current_and_voltage_tab.figure.add_subplot(3, 1, 1)
    # self.amp_Voltage_axes = self.current_and_voltage_tab.figure.add_subplot(3, 1, 2)
    # self.vcc_Voltage_axes = self.current_and_voltage_tab.figure.add_subplot(3, 1, 3)
    self.graphs_ready = false
  
  def begin(self, serial):
    self.ser = serial
    self.pack(fill='both')
    if not self.graphs_ready:
      self.open_graphs()
  
  def close(self):
    self.pack_forget()

  def draw(self):
    self.undraw()
    print("Drawing " + str(self))

    # # self.draw_background()
    # self.ntc_i_Temperature_axes.plot(self.time_array, self.ntc_i_T_array)
    # self.ntc_i_Resistance_axes.plot(self.time_array, self.ntc_i_R_array)
    # self.ntc_i_Vroltage_axes.plot(self.time_array, self.ntc_i_Vr_array)
    # self.ntc_e_Temperature_axes.plot(self.time_array, self.ntc_e_T_array)
    # self.ntc_e_Resistance_axes.plot(self.time_array, self.ntc_e_R_array)
    # self.ntc_e_Vroltage_axes.plot(self.time_array, self.ntc_e_Vr_array)
    # self.amp_Current_axes.plot(self.time_array, self.amp_A_array)
    # self.amp_Voltage_axes.plot(self.time_array, self.amp_V_array)
    # self.vcc_Voltage_axes.plot(self.time_array, self.vcc_V_array)

    # self.ntc_i_tab.canvas.draw()
    # self.ntc_e_tab.canvas.draw()
    # self.current_and_voltage_tab.canvas.draw()
    # self.configure(width=300*self.iy, height=200*self.ix)

  def undraw(self):
    pass
    # self.ntc_i_Temperature_axes.clear()
    # self.ntc_i_Resistance_axes.clear()
    # self.ntc_i_Vroltage_axes.clear()
    # self.ntc_e_Temperature_axes.clear()
    # self.ntc_e_Resistance_axes.clear()
    # self.ntc_e_Vroltage_axes.clear()
    # self.amp_Current_axes.clear()
    # self.amp_Voltage_axes.clear()
    # self.vcc_Voltage_axes.clear()
    # self.figure.clear()
    
  def add_status_packet(self, new_packet):
    pass
    # self.time_array = np.append(self.time_array,time)
    # self.ntc_i_T_array = np.append(self.ntc_i_T_array,ntc_i_T)
    # self.ntc_i_R_array = np.append(self.ntc_i_R_array,ntc_i_R)
    # self.ntc_i_Vr_array = np.append(self.ntc_i_Vr_array,ntc_i_Vr)
    # self.ntc_e_T_array = np.append(self.ntc_e_T_array,ntc_e_T)
    # self.ntc_e_R_array = np.append(self.ntc_e_R_array,ntc_e_R)
    # self.ntc_e_Vr_array = np.append(self.ntc_e_Vr_array,ntc_e_Vr)
    # self.amp_A_array = np.append(self.amp_A_array,amp_A)
    # self.amp_V_array = np.append(self.amp_V_array,amp_V)
    # self.vcc_V_array = np.append(self.vcc_V_array,vcc_V)
    
    # with open(self.logger_file, '+a') as f:
    #   f.write(str(time) + ',')
    #   f.write(str(ntc_i_T) + ',')
    #   f.write(str(ntc_i_R) + ',')
    #   f.write(str(ntc_i_Vr) + ',')
    #   f.write(str(ntc_e_T) + ',')
    #   f.write(str(ntc_e_R) + ',')
    #   f.write(str(ntc_e_Vr) + ',')
    #   f.write(str(amp_A) + ',')
    #   f.write(str(amp_V) + ',')
    #   f.write(str(vcc_V) + '\n')
    
    # if self.counter == 4:
    #   self.draw()
    #   self.counter = 0
    # else:
    #   self.counter += 1
    
  def add_imaging_packet(self, new_packet):
    pass