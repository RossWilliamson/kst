#!/usr/bin/python2.7
import pykst as kst

client=kst.Client("TestX2")

v1 = client.new_generated_vector(-10, 10, 1000)
e1 = client.new_equation(v1, "sin(x)")
c1 = client.new_curve(e1.x(), e1.y())
p1 = client.new_plot()
p1.add(c1)
