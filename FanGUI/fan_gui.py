import tkinter as tk
import tkinter.ttk as ttk

# set up master
t = tk.Tk()
t.title("F767ZI-FanControl - Control panel")
t.geometry("400x450")
t.resizable(0, 0)

fan1_setmode = tk.IntVar()
fan2_setmode = tk.IntVar()

t.grid_columnconfigure(0, weight=1)
t.grid_columnconfigure(1, weight=1)

## BEGIN Panel 1 - Serial port
serialport_panel = ttk.Frame(t)
serialport_panel.grid(column=0, row=0, columnspan=2, sticky="nsew")

# Declare elements of Panel 1
serialport_label = tk.Label(serialport_panel, text="Serial port:")
serialport_select_combobox = ttk.Combobox(serialport_panel)
serialport_connect_button = ttk.Button(serialport_panel, text="Connect")
serialport_conf_button = ttk.Button(serialport_panel, text="Configure")

# Position elements
serialport_label.pack(side="left", anchor="center")
serialport_select_combobox.pack(side="left", anchor="center", expand=1, fill="x")
serialport_connect_button.pack(side="left", anchor="center")
serialport_conf_button.pack(side="left", anchor="center")
## END Panel 1 - Serial port

## BEGIN Panel 2 - Temperature
# No frame here, just shamelessly pack into master

# Declare elements of Panel 2
temperature_label = tk.Label(t, text="Temperature:", anchor="e")
temperature_measurement_label = tk.Label(t, text="0\u00B0C", anchor="w", font=("Comic Sans MS", 16))

# Position elements
temperature_label.grid(column=0, row=1, sticky="e")
temperature_measurement_label.grid(column=1, row=1, sticky="w")
## END Panel 2 - Temperature

## BEGIN Panel 3 - Fan 1
fan1_panel = ttk.Frame(t)
fan1_panel.grid(column=0, row=2, sticky="ew", ipadx="3")

fan1_label = tk.Label(fan1_panel, text="Fan 1:", anchor="center")
fan1_mode_label = tk.Label(fan1_panel, text="Mode:", anchor="e")
fan1_mode_measurement_label = tk.Label(fan1_panel, text="Direct", anchor="w", font=("Comic Sans MS", 12))
fan1_target_label = tk.Label(fan1_panel, text="Requested value:", anchor="e")
fan1_target_measurement_label = tk.Label(fan1_panel, text="2137 RPM", anchor="w", font=("Comic Sans MS", 12))
fan1_speed_label = tk.Label(fan1_panel, text="Speed:", anchor="e")
fan1_speed_measurement_label = tk.Label(fan1_panel, text="1000 RPM", anchor="w", font=("Comic Sans MS", 16))
fan1_duty_label = tk.Label(fan1_panel, text="Duty cycle:", anchor="e")
fan1_duty_measurement_label = tk.Label(fan1_panel, text="30 %", anchor="w", font=("Comic Sans MS", 16))

fan1_ctrl_labelframe = ttk.LabelFrame(fan1_panel, text="Control")
fan1_ctrl_radio_direct = ttk.Radiobutton(fan1_ctrl_labelframe, text="Direct mode (duty cycle)", variable=fan1_setmode, value="1")
fan1_ctrl_radio_pcontrol = ttk.Radiobutton(fan1_ctrl_labelframe, text="P-Control mode (speed)", variable=fan1_setmode, value="2")
fan1_ctrl_speed_scale = ttk.Scale(fan1_ctrl_labelframe, from_=0, to=1000, orient="horizontal")
fan1_ctrl_value_label = tk.Label(fan1_ctrl_labelframe, text="2137 RPM", anchor="center")
fan1_ctrl_apply_button = ttk.Button(fan1_ctrl_labelframe, text="Apply")

fan1_calib_labelframe = ttk.LabelFrame(fan1_panel, text="Calibration")
fan1_calib_button = ttk.Button(fan1_calib_labelframe, text="Calibrate")
fan1_calib_min = tk.Label(fan1_calib_labelframe, text="Min. speed: 600 RPM")
fan1_calib_max = tk.Label(fan1_calib_labelframe, text="Max. speed: 2000 RPM")

fan1_label.grid(column=0, row=0, columnspan=2,sticky="nsew")
fan1_mode_label.grid(column=0, row=1, sticky="e")
fan1_mode_measurement_label.grid(column=1, row=1, sticky="w")
fan1_target_label.grid(column=0, row=2, sticky="e")
fan1_target_measurement_label.grid(column=1, row=2, sticky="w")
fan1_speed_label.grid(column=0, row=3, sticky="e")
fan1_speed_measurement_label.grid(column=1, row=3, sticky="w")
fan1_duty_label.grid(column=0, row=4, sticky="e")
fan1_duty_measurement_label.grid(column=1, row=4, sticky="w")

fan1_ctrl_labelframe.grid(column=0, row=5, columnspan=2, sticky="nsew")
fan1_ctrl_radio_direct.pack(side="top")
fan1_ctrl_radio_pcontrol.pack(side="top")
fan1_ctrl_speed_scale.pack(side="top")
fan1_ctrl_value_label.pack(side="top")
fan1_ctrl_apply_button.pack(side="top")

fan1_calib_labelframe.grid(column=0, row=6, columnspan=2, sticky="nsew")
fan1_calib_button.pack(side="top")
fan1_calib_min.pack(side="top")
fan1_calib_max.pack(side="top")

## END Panel 3 - Fan 1

## BEGIN Panel 4 - Fan 2
fan2_panel = ttk.Frame(t)
fan2_panel.grid(column=1, row=2, sticky="ew", ipadx="3")

fan2_label = tk.Label(fan2_panel, text="Fan 2:", anchor="center")
fan2_mode_label = tk.Label(fan2_panel, text="Mode:", anchor="e")
fan2_mode_measurement_label = tk.Label(fan2_panel, text="Direct", anchor="w", font=("Comic Sans MS", 12))
fan2_target_label = tk.Label(fan2_panel, text="Requested value:", anchor="e")
fan2_target_measurement_label = tk.Label(fan2_panel, text="2137 RPM", anchor="w", font=("Comic Sans MS", 12))
fan2_speed_label = tk.Label(fan2_panel, text="Speed:", anchor="e")
fan2_speed_measurement_label = tk.Label(fan2_panel, text="100 RPM", anchor="w", font=("Comic Sans MS", 16))
fan2_duty_label = tk.Label(fan2_panel, text="Duty cycle:", anchor="e")
fan2_duty_measurement_label = tk.Label(fan2_panel, text="30 %", anchor="w", font=("Comic Sans MS", 16))

fan2_ctrl_labelframe = ttk.LabelFrame(fan2_panel, text="Control")
fan2_ctrl_radio_direct = ttk.Radiobutton(fan2_ctrl_labelframe, text="Direct mode (duty cycle)", variable=fan2_setmode, value="1")
fan2_ctrl_radio_pcontrol = ttk.Radiobutton(fan2_ctrl_labelframe, text="P-Control mode (speed)", variable=fan2_setmode, value="2")
fan2_ctrl_speed_scale = ttk.Scale(fan2_ctrl_labelframe, from_=0, to=1000, orient="horizontal")
fan2_ctrl_value_label = tk.Label(fan2_ctrl_labelframe, text="2137 RPM", anchor="center")
fan2_ctrl_apply_button = ttk.Button(fan2_ctrl_labelframe, text="Apply")

fan2_calib_labelframe = ttk.LabelFrame(fan2_panel, text="Calibration")
fan2_calib_button = ttk.Button(fan2_calib_labelframe, text="Calibrate")
fan2_calib_min = tk.Label(fan2_calib_labelframe, text="Min. speed: 600 RPM")
fan2_calib_max = tk.Label(fan2_calib_labelframe, text="Max. speed: 2000 RPM")

fan2_label.grid(column=0, row=0, columnspan=2,sticky="nsew")
fan2_mode_label.grid(column=0, row=1, sticky="e")
fan2_mode_measurement_label.grid(column=1, row=1, sticky="w")
fan2_target_label.grid(column=0, row=2, sticky="e")
fan2_target_measurement_label.grid(column=1, row=2, sticky="w")
fan2_speed_label.grid(column=0, row=3, sticky="e")
fan2_speed_measurement_label.grid(column=1, row=3, sticky="w")
fan2_duty_label.grid(column=0, row=4, sticky="e")
fan2_duty_measurement_label.grid(column=1, row=4, sticky="w")

fan2_ctrl_labelframe.grid(column=0, row=5, columnspan=2, sticky="nsew")
fan2_ctrl_radio_direct.pack(side="top")
fan2_ctrl_radio_pcontrol.pack(side="top")
fan2_ctrl_speed_scale.pack(side="top")
fan2_ctrl_value_label.pack(side="top")
fan2_ctrl_apply_button.pack(side="top")

fan2_calib_labelframe.grid(column=0, row=6, columnspan=2, sticky="nsew")
fan2_calib_button.pack(side="top")
fan2_calib_min.pack(side="top")
fan2_calib_max.pack(side="top")
## END Panel 4 - Fan 2

## BEGIN Panel 5 - status bar
statusbar_panel = ttk.Frame(t)
statusbar_msg = tk.Label(statusbar_panel, text="my wife left me has")
statusbar_panel.grid(column=0, row=3, columnspan=2, sticky="w")
statusbar_msg.pack(side="left")
## END Panel 5 - status bar

# Run GUI
t.mainloop()
