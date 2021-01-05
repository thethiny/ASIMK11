import time
from random import random

client_name = "Unlocker"
pipe_name = r"\\.\\pipe\MK11Unlocker"

def read_buffer(file_handle, buffer_size):
    return file_handle.read(buffer_size)

def read_line(file_handle):
    EOF = False
    data = ""
    while True:
        try:
            c = file_handle.read(1).decode("utf-8")
        except OSError: # EOF
            EOF = True
            break
        if c == '\n':
            break
        if not c:
            EOF = True
            break
        data += c
    return data, EOF

print(f"{client_name}::Connecting to Pipe")
MAX_TIMEOUT = 20
current_timeout = 0
while True:
    if current_timeout >= MAX_TIMEOUT: exit() # Enough is Enough
    try:
        f = open(pipe_name, 'rb+', 0)
        break
    except FileNotFoundError:
        time.sleep(1)
        current_timeout+= 1
        continue

print(f"{client_name}::Connected")

print(f"{client_name}::Reading Data")

def write_line(line):
    f.seek(0)
    line = (line).encode('ascii')
    f.write(line)
    f.truncate()

while True:
    if (random() < 0.5):
        write_line("EXP_Koin")
    else:
        write_line("EXP_MERCEDEDSBENS")
    data, EOF = read_line(f)
    if EOF: break
    print(f"{client_name}::Received", data)
    time.sleep(2)