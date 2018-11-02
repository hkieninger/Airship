#!/usr/bin/python

#functions to calculate stuff about a volume created through rotation of a function around the x-axis

import math

A1 = 1.000000
A2 = 3.462153
A3 = -26.960996
A4 = 59.357210
A5 = -56.480030
A6 = 19.621670

LENGTH = 2373.0 #Millimeter
DIAMETER = 593.0 #Millimeter
BORDER = 8.0 #Millimeter

GORES = 3.0 #Amount
PRINT_STEP = 10.0 #Millimeter
DELTA_H = 0.01 #Millimeter

def pythagoras(dx, dy):
	return math.sqrt(dx ** 2 + dy ** 2)

def crossSectionFunc(x, length, diameter):
	x /= length
	polynom = A1 * x + A2 * x ** 2 + A3 * x ** 3 + A4 * x ** 4 + A5 * x ** 5 + A6 * x ** 6
	root = math.sqrt(polynom)
	return root * diameter

#returns the volume in mm^2
def calcVolume(length, diameter, deltah):
	x = 0
	volume = 0
	while x < length:
		volume += crossSectionFunc(x, length, diameter) ** 2 * math.pi * deltah
		x += deltah
	return volume

#returns the surface in mm^2
def calcSurface(length, diameter, deltah, gores, border):
	lastFx = 0
	x = 0
	s = 0
	surface = 0
	while x < length:
		fx = crossSectionFunc(x, length, diameter)
		s = pythagoras(fx - lastFx, deltah)
		surface += (fx + lastFx) * math.pi * s
		surface += s * border * 2 * gores
		lastFx = fx
		x += deltah
	return surface

#returns the x coordinate, where the half of the volume is reached
def calcMidVolume(length, diameter, deltah):
	x = 0
	halfVolume = calcVolume(length, diameter, deltah) / 2
	lastVolume = 0
	volume = 0
	while x < length:
		lastVolume = volume
		volume += crossSectionFunc(x, length, diameter) ** 2 * math.pi * deltah
		if abs(halfVolume - lastVolume) < abs(halfVolume - volume):
			return x - deltah
		x += deltah
	return False

#returns the x coordinate, where the half of the surface is reached
def calcMidSurface(length, diameter, deltah, gores, border):
	x = 0
	halfSurface = calcSurface(length, diameter, deltah, gores, border) / 2
	lastSurface = 0
	surface = 0
	lastFx = 0
	while x < length:
		fx = crossSectionFunc(x, length, diameter)
		lastSurface = surface
		s = pythagoras(fx - lastFx, deltah)
		surface += (lastFx + fx) * math.pi * s
		surface += s * border * 2 * gores
		if abs(halfSurface - lastSurface) < abs(halfSurface - surface):
			return x - deltah
		lastFx = fx
		x += deltah
	return False

#returns the points which describe half a gore
def calcHalfGore(length, diameter, deltah, step, gores):
	points = []
	lastFx = 0
	nextStep = 0
	x = 0
	s = 0
	while x < length:
		fx = crossSectionFunc(x, length, diameter)
		s += pythagoras(fx - lastFx, deltah)
		if nextStep < s:
			points.append((s, fx * math.pi / gores))
			nextStep += step
		lastFx = fx
		x += deltah
	points.append((s, fx * math.pi / gores))
	return points

print("Information")
print("Volume is " + str(calcVolume(LENGTH, DIAMETER, DELTA_H) / 10 ** 9) + " m^3")
print("Surface is " + str(calcSurface(LENGTH, DIAMETER, DELTA_H, GORES, BORDER) / 10 ** 6) + " m^2")
print("Midvolume is reached at: " + str(calcMidVolume(LENGTH, DIAMETER, DELTA_H)) + " mm")
print("Midsurface is reached at: " + str(calcMidSurface(LENGTH, DIAMETER, DELTA_H, GORES, BORDER))+ " mm")
print("")
print("Gore coordinates")
halfgore = calcHalfGore(LENGTH, DIAMETER, DELTA_H, PRINT_STEP, GORES)
for p in halfgore:
	x, y = p
	print(str(x) + ";" + str(y))
