from random import uniform, randint
from math import sin, cos, pi, exp

XMIN = -4
XMAX = 4
error = 0.0000001

def eval(x):
	try:
		return pow((sin(pi*abs(x)))/(pi*abs(x)),2)
	finally:
		x = x - error
		return pow((sin(pi*abs(x)))/(pi*abs(x)),2)

class Point(): ##Possible Solution
	def __init__(self):
		self.x = uniform(XMIN, XMAX)
		self.y = eval(self.x)

	def setX(self, newx):
		self.x = newx
		self.y = eval(self.x)
	def __str__(self):
		return " Solution: (%f,%f)"%(self.x, self.y)

class SimAnnealing():
	def __init__(self):
		self.solution = Point()
		self.temp = 100
		self.alpha = 0.50
		self.probability = 0.6

	def init(self):
		currentSol = self.solution
		neighbor = None
		print "Initial Solution"
		self.printPoint(currentSol)
		iters = 0
		while True:
			neighbor = Point()

			diff = self.f(currentSol) - self.f(neighbor)

			if(self.temp <= 0):
				break
			if(self.solution.y < 1.0e-3):
				print "solution!"
				break

			if( diff <= 0 ):
				currentSol = neighbor
			else:
				#prob = (exp(self.f(currentSol)/self.temp))/ (exp(self.f(currentSol)/self.temp) + exp(self.f(neighbor)/self.temp))
				prob = exp(-diff/self.temp)
				if(prob > self.probability):
					#print "prob"
					currentSol = neighbor	
			#print self.temp
			self.temp = self.alpha*self.temp
			iters += 1
			#if(self.temp < 1.0e-150):
				#self.temp = 0

		print "\nSolution Founded!"
		self.printPoint(currentSol)
		

	def f(self, sol):
		return float(abs(sol.y) - 0.0 )

	def printPoint(self, point):
		print point