import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

if __name__ == '__main__':
	if (len(sys.argv) != 3):
		print 'Usange python graphsol.py input_file sol_file'
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

	# Display solution graphic.
	im = plt.imshow(tij, interpolation='nearest', cmap=cm.jet, origin='lower', extent=[0,a,0,b], vmax=tij.max(), vmin=tij.min())
	plt.colorbar()
	plt.show()

