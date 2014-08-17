# -*- coding: utf-8 -*-

import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from numpy.linalg import norm

maxTempPtoCritico = 235.0

def positionCovered(zi,spos,r):
	for i in xrange(len(spos)):
		x = spos[i,:]
		if norm(zi - x,ord=2) <= r:
			return True

def validTemp(tij,i,j):
	if abs(tij[i,j] - (tij[i+1,j] + tij[i-1,j] + tij[i,j+1] + tij[i,j-1])/4.0) <= 0.0001:
		return True
	return False

if __name__ == '__main__':
	if (len(sys.argv) != 3):
		print 'Usange python judge.py input_file sol_file'
		sys.exit()
		
	spos = []
	# Read input file.
	with open(sys.argv[1],'r') as f:
		#a, b, n, m, r, ts, k = [float(x) for x in f.readline().split()]
		a, b, h, r, ts, k = [float(x) for x in f.readline().split()]
		n = int(b/h)
		m = int(a/h)
		k = int(k)

		spos = np.zeros(shape=(k,2))
		for i in xrange(k):
			spos[i,:] = [float(x) for x in f.readline().split()]

	if n % 2 != 0 or m % 2 != 0:
		print "Juez solo válido para instancias con discretización impar"
		sys.exit(1)

	# Create empty matrix. 
	tij = np.zeros(shape=((n+1),(m+1)))

	# Read solution.
	with open(sys.argv[2],'r') as f:
		for row in f.readlines():
			row = row.split()
			i = int(row[0])
			j = int(row[1])
			val = float(row[2])
			tij[i,j] = val

	# Chequear solucion valida.
	zi = np.array([0.0, 0.0])
	for i in xrange(1,n):
		for j in xrange(1,m):
			zi[0] = float(j)*float(a)/float(m)
			zi[1] = float(i)*float(b)/float(n)
			if not(positionCovered(zi,spos,r) or validTemp(tij,i,j)):
				print 'Temperatura no válida en posición',i,j,tij[i,j]
				sys.exit(1)

	# Si llegamos hasta aca, la solucion es valida. Analizar el punto critico.
	if tij[m/2,n/2] <= maxTempPtoCritico:
		print '¡Bien! ¡El parabrisas no se rompe campeón!'
	else:
		print 'La temperatura en el punto critico excede los ',maxTempPtoCritico

