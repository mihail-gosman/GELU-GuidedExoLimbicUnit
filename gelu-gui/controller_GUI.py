############################
##  pip install pyserial  ##
##  pip install tk        ##
############################
import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
import serial
import time

class DarkFiledialog(filedialog.Dialog):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

    def body(self, master):
        self.configure(bg='#2E2E2E')
        return super().body(master)

class ArduinoControlGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Arduino Control GUI")
        self.root.minsize(650, 520) 
        self.root.maxsize(650, 520)  
        self.root.configure(bg='#2E2E2E')

        # Header
        header_label = tk.Label(self.root, text="Send button:commands->arduino\nRun button: saved commands->arduino\nSave button: commands->saved commands list", font=("Helvetica", 16), foreground='white', background='#2E2E2E')
        header_label.grid(row=0, column=0, columnspan=4, pady=10)

        # Serial Port Entry
        self.serial_port_var = tk.StringVar()
        self.serial_port_label = ttk.Label(self.root, text="Enter Serial Port:")
        self.serial_port_entry = ttk.Entry(self.root, textvariable=self.serial_port_var, width=20)
        self.serial_port_label.grid(row=1, column=0, padx=10, pady=10)
        self.serial_port_entry.grid(row=1, column=1, padx=10, pady=10)

        # Commands Listbox
        self.commands_listbox = tk.Listbox(self.root, selectbackground='#404040', selectforeground='white', bg='#2E2E2E')
        self.commands_listbox.grid(row=2, column=0, columnspan=4, padx=10, pady=10)

        # Servo 1 Entry
        self.servo1_var = tk.StringVar()
        self.servo1_label = ttk.Label(self.root, text="Servo 1:", foreground='white', background='#2E2E2E')
        self.left_arrow1_button = ttk.Button(self.root, text="<", command=lambda: self.update_servo_value(self.servo1_var, -1))
        self.servo1_entry = ttk.Entry(self.root, textvariable=self.servo1_var, width=5)
        self.right_arrow1_button = ttk.Button(self.root, text=">", command=lambda: self.update_servo_value(self.servo1_var, 1))

        self.servo1_label.grid(row=3, column=0, pady=10)
        self.left_arrow1_button.grid(row=3, column=1, pady=10)
        self.servo1_entry.grid(row=3, column=2, pady=10)
        self.right_arrow1_button.grid(row=3, column=3, pady=10)

        # Servo 2 Entry
        self.servo2_var = tk.StringVar()
        self.servo2_label = ttk.Label(self.root, text="Servo 2:", foreground='white', background='#2E2E2E')
        self.left_arrow2_button = ttk.Button(self.root, text="<", command=lambda: self.update_servo_value(self.servo2_var, -1))
        self.servo2_entry = ttk.Entry(self.root, textvariable=self.servo2_var, width=5)
        self.right_arrow2_button = ttk.Button(self.root, text=">", command=lambda: self.update_servo_value(self.servo2_var, 1))

        self.servo2_label.grid(row=4, column=0, pady=10)
        self.left_arrow2_button.grid(row=4, column=1, pady=10)
        self.servo2_entry.grid(row=4, column=2, pady=10)
        self.right_arrow2_button.grid(row=4, column=3, pady=10)

        # Run Button
        self.run_button = ttk.Button(self.root, text="Run", command=self.run_commands, style='DarkButton.TButton')
        self.run_button.grid(row=5, column=0, padx=10, pady=10)

        # Send Command Button
        self.send_command_button = ttk.Button(self.root, text="Send Command", command=self.send_command, style='DarkButton.TButton')
        self.send_command_button.grid(row=5, column=1, padx=10, pady=10)

        # Save Command Button
        self.save_button = ttk.Button(self.root, text="Save Command", command=self.save_command, style='DarkButton.TButton')
        self.save_button.grid(row=5, column=2, padx=10, pady=10)

        # Styling
        self.style = ttk.Style()
        self.style.configure('DarkButton.TButton', foreground='white', background='#404040', padding=(10, 5))

        # Menu
        self.create_menu()

    def create_menu(self):
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)

        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="Open", command=self.open_file)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.root.destroy)

    def update_servo_value(self, servo_var, increment):
        try:
            value = int(servo_var.get())
            value += increment
            if 0 <= value <= 180:
                servo_var.set(str(value))
        except ValueError:
            pass

    def save_command(self):
        try:
            servo1_value = int(self.servo1_var.get())
            servo2_value = int(self.servo2_var.get())
            if 0 <= servo1_value <= 180 and 0 <= servo2_value <= 180:
                command1 = f"<SERVO1_POS {servo1_value}>"
                command2 = f"<SERVO2_POS {servo2_value}>"
                self.commands_listbox.insert(tk.END, command1)
                self.commands_listbox.insert(tk.END, command2)
        except ValueError:
            pass

    def open_file(self):
        file_path = DarkFiledialog(self.root, title="Open File", defaultextension=".txt", filetypes=[("Text files", "*.txt")]).show()
        if file_path:
            with open(file_path, 'r') as file:
                commands = file.readlines()
                self.commands_listbox.delete(0, tk.END)
                for command in commands:
                    self.commands_listbox.insert(tk.END, command.strip())

    def run_commands(self):
        serial_port = self.serial_port_var.get()
        commands = self.commands_listbox.get(0, tk.END)

        if serial_port and commands:
            print(f"Running commands on {serial_port}:")
            for command in commands:
                print(command)
            try:
                arduino = serial.Serial(serial_port, 9600, timeout=1)
                for command in commands:
                    arduino.write(f'{command}\n'.encode())
                    time.sleep(0.1)
                arduino.close()
                print(f"Commands executed on {serial_port}")
            except serial.SerialException:
                print(f"Error: Unable to open {serial_port}. Make sure the port is correct and not in use.")
            finally:
                self.commands_listbox.delete(0, tk.END)
        else:
            print("Please enter a serial port and at least one command.")

    def send_command(self):
        serial_port = self.serial_port_var.get()
        servo1_value = self.servo1_var.get()
        servo2_value = self.servo2_var.get()

        if serial_port and servo1_value and servo2_value:
            command1 = f"<SERVO1_POS {servo1_value}>"
            command2 = f"<SERVO2_POS {servo2_value}>"
            print(f"Sending commands to {serial_port}: {command1}, {command2}")
            try:
                arduino = serial.Serial(serial_port, 9600, timeout=1)
                arduino.write(f'{command1}\n'.encode())
                time.sleep(0.1)
                arduino.write(f'{command2}\n'.encode())
                arduino.close()
                print(f"Commands sent to {serial_port}")
                self.servo1_var.set('')
                self.servo2_var.set('')
            except serial.SerialException:
                print(f"Error: Unable to open {serial_port}. Make sure the port is correct and not in use.")
        else:
            print("Please enter a serial port and servo values.")

if __name__ == "__main__":
    root = tk.Tk()
    app = ArduinoControlGUI(root)
    root.mainloop()

