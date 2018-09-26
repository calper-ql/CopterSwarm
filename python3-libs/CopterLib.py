import numpy as np
import ctypes as ct
import struct
import socket
import sys

class Session:
    def __init__(self, ip, port):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((ip, port))

    def add_entity(self, entity):
        rval = self.send_command(entity.tobytes())
        val, package = unpack_command(rval)
        val, package = unpack_unsigned(package)
        entity.session_id = val
        entity.session = self
        if len(package) != 0:
            val, package = unpack_unsigned(package)
            entity.special_id = val

    def send_command(self, cmd):
        size = ct.c_uint(len(cmd))
        self.s.send(bytearray(size))
        self.s.send(cmd)
        r_val = self.s.recv(4)
        r_size, _ = unpack_unsigned(r_val)
        r_val = self.s.recv(r_size)
        return r_val

    def step(self, tick_rate):
        cmd = create_command("step")
        cmd = pack_unsigned(cmd, tick_rate)
        rval = self.send_command(cmd)

    def gravity(self, vector):
        cmd = create_command("gravity")
        cmd = pack_vec3(cmd, vector)
        rval = self.send_command(cmd)
        

class Entity:
    def __init__(self, position, radius, mass):
        assert(len(position) == 3)
        self.position = position
        self.radius = radius
        self.mass = mass
        self.session_id = -1
        self.session = None
        self.children = []

    def get_position(self):
        if self.session:
            cmd = pack_unsigned(create_command("get_pos"), self.session_id)
            rval = self.session.send_command(cmd)
            value, cmd = unpack_command(rval)
            value, cmd = unpack_vec3(cmd)
            self.position = value
            return value
        return None
        

    def tobytes(self):
        cmd = create_command("Entity")
        cmd = pack_vec3(cmd, self.position)
        cmd = pack_float(cmd, self.radius)
        cmd = pack_float(cmd, self.mass)
        return cmd

class Copter(Entity):
    def __init__(self, position, radius, mass):
        super().__init__(position, radius, mass)
        self.special_id = -1

    def tobytes(self):
        cmd = create_command("Copter")
        cmd = pack_vec3(cmd, self.position)
        cmd = pack_float(cmd, self.radius)
        cmd = pack_float(cmd, self.mass)
        return cmd

    def add_actuator(self, m, r):
        if m.session == self.session and r.session == self.session:
            cmd = create_command("add_actuator");
            cmd = pack_unsigned(cmd, self.special_id)
            cmd = pack_unsigned(cmd, m.special_id)
            cmd = pack_unsigned(cmd, r.special_id)
            rval = m.session.send_command(cmd)
            value, package = unpack_command(rval)
            if value == "Success":
                return True
            else:
                return False
        else: 
            return False

class Motor(Entity):
    def __init__(self, max_power, efficiency, position, radius, mass):
        self.max_power = max_power
        self.efficiency = efficiency
        super().__init__(position, radius, mass)
        self.special_id = -1

    def tobytes(self):
        cmd = create_command("Motor")
        cmd = pack_float(cmd, self.max_power)
        cmd = pack_float(cmd, self.efficiency)
        cmd = pack_vec3(cmd, self.position)
        cmd = pack_float(cmd, self.radius)
        cmd = pack_float(cmd, self.mass)
        return cmd

class Rotor(Entity):
    def __init__(self, lift_drag_ratio, drag_value, enclosed, position, radius, mass):
        self.lift_drag_ratio = lift_drag_ratio
        self.drag_value = drag_value
        self.enclosed = enclosed
        super().__init__(position, radius, mass)
        self.special_id = -1

    def tobytes(self):
        cmd = create_command("Rotor")
        cmd = pack_float(cmd, self.lift_drag_ratio)
        cmd = pack_float(cmd, self.drag_value)
        cmd = pack_bool(cmd, self.enclosed)
        cmd = pack_vec3(cmd, self.position)
        cmd = pack_float(cmd, self.radius)
        cmd = pack_float(cmd, self.mass)
        return cmd

class EnergySource(Entity):
    def __init__(self, energy, maximum, position, radius, mass):
        self.energy = energy
        self.maximum = maximum
        super().__init__(position, radius, mass)
        self.special_id = -1

    def tobytes(self):
        cmd = create_command("EnergySource")
        cmd = pack_float(cmd, self.energy)
        cmd = pack_float(cmd, self.maximum)
        cmd = pack_vec3(cmd, self.position)
        cmd = pack_float(cmd, self.radius)
        cmd = pack_float(cmd, self.mass)
        return cmd


###### COMMANDS ######

def create_command(cmd):
    size = ct.c_uint(len(cmd))
    string = bytearray(size)
    string.extend(cmd.encode('ASCII'))
    return string

def unpack_command(cmd):
    size = struct.unpack('<I', cmd[:4])[0]
    string = cmd[4:4+size].decode()
    return string, cmd[4+size:]


########################

def pack_unsigned(cmd, value):
    cnv = ct.c_uint(value)
    cmd.extend(cnv)
    return cmd

def unpack_unsigned(cmd):
    val = struct.unpack('<I', cmd[:4])[0]
    return val, cmd[4:]

########################

def pack_float(cmd, value):
    cnv = ct.c_float(value)
    cmd.extend(cnv)
    return cmd

def unpack_float(cmd):
    val = struct.unpack('<f', cmd[:4])[0]
    return val, cmd[4:]

########################

def pack_bool(cmd, value):
    cnv = ct.c_bool(value)
    cmd.extend(cnv)
    return cmd

def unpack_bool(cmd):
    val = struct.unpack('<?', cmd[:1])[0]
    return val, cmd[1:]

########################

def pack_vec3(cmd, value):
    value = np.array(value, dtype=np.float32)
    cmd.extend(value.tobytes())
    return cmd

def unpack_vec3(cmd):
    return np.frombuffer(cmd[0:12], dtype=np.float32), cmd[12:]

########################

def pack_mat3(cmd, value):
    value = np.array(value, dtype=np.float32)
    cmd.extend(value.tobytes())
    return cmd

def unpack_mat3(cmd):
    return np.frombuffer(cmd[0:12*3], dtype=np.float32), cmd[12*3:]

