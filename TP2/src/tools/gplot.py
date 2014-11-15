#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import networkx as nx
import matplotlib.pyplot as plt
import pylab

"""
Just need to install:

	sudo apt-get install python-networkx
	sudo apt-get install python-matplotlib

"""

def usage():
	print "usage: "
	print "		python gplot.py input.txt [output.png]"
	print

if __name__ == '__main__':
	ps=len(sys.argv)
	if (ps < 2 or ps > 3):
		usage()
		sys.exit()

	gpath= str(sys.argv[1])

	G=nx.read_adjlist(gpath,create_using=nx.DiGraph())

	# networkx draw()
	nx.draw_networkx(G,node_color='w')
	limits=pylab.axis('off') # turn of axis

	if (len(sys.argv) > 2):
		# save to file
		fout= str(sys.argv[2])
		plt.savefig(fout)
	else:
		# show plot
		plt.show()