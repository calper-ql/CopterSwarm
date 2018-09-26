from CopterLib import *

s = Session('127.0.0.1', 30000)

c = Copter([0.0, 0.0, 0.0], 1.0, 1.2)

m1 = Motor(300.0, 0.05, [-1.0, 0.0, 0.0], 0.1, 0.1)
r1 = Rotor(4.0, 0.1, True, [0.0, 0.0, 0.1], 0.1, 0.1)
m2 = Motor(300.0, 0.05, [1.0, 0.0, 0.0], 0.1, 0.1)
r2 = Rotor(4.0, 0.1, True, [0.0, 0.0, 0.1], 0.1, 0.1)

es = EnergySource(30000, 40000, [0.0, 0.0, 0.0], 0.1, 0.2)

s.add_entity(c)
s.add_entity(m1)
s.add_entity(m2)
s.add_entity(r1)
s.add_entity(r2)

c.add_actuator(m1, r1)
c.add_actuator(m2, r2)

for i in range(300):
    s.gravity([0.0, 0.0, -9.8/30])
    print(c.get_position(), m1.get_position())
    s.step(30)

