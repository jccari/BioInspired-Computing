from random import uniform, randint
from math import *
from Tools import floatToBin, binToFloat, printList

XMIN = -4
XMAX = 4

error = 0.00000001

def eval(x):
	try:
		return pow((sin(pi*abs(x)))/(pi*abs(x)),2)
	finally:
		x = x - error
		return pow((sin(pi*abs(x)))/(pi*abs(x)),2)

class Chromosome(): ##Individuo
	def __init__(self, value):
		self.x = value
		self.y = eval(self.x)

	def setX(self, newx):
		self.x = newx
		self.y = eval(self.x)

class GeneticAlg():
	def __init__(self, _maxPopulation):
		self.maxPopulation = _maxPopulation 	# size of population
		self.populationList = []		# chromosomes in a generation

		self.bestChromosome = None	# the best in a population
		self.worstChromosomesList = []	# worstChromosomes[eliminated]
		self.eliminated = 2			# max number of chromosomes that were eliminatedd in each generation

		self.crossingProb = 0		# probability of crossover ????????????
		self.mutationProb = 60		# probability of mutation  ????????????

		self.inicializeRandom()

	def inicializeRandom(self):
		for x in xrange(0, self.maxPopulation ):
			self.populationList.append( Chromosome( uniform(XMIN, XMAX) ) )

	def distance(self, a , b):
		return abs(a) - 0

	def init(self):
		i = 0
		while True:
			print "[DEBUG] Generation", i
			self.printPopulation()

			self.selection()
			if (self.distance(self.bestChromosome.y , 0) < error):
				break;

			self.crossOver()
			self.mutation()
			i += 1

		print "Best Solution:"
		print "(x,y) = ","(", self.bestChromosome.x,"," , self.bestChromosome.y ,")"

	def selection(self):
		self.sort(self.populationList)

		self.bestChromosome = self.populationList[0]
		
		self.populationList = self.removeChromosomes(self.populationList, self.eliminated)
		

	def crossOver(self):
		tmpList = []
		for x in range(0,self.eliminated):
			num1bin = floatToBin(self.populationList[randint(0,len(self.populationList)-1)].x)
			num2bin = floatToBin(self.populationList[randint(0,len(self.populationList)-1)].x)
			numbin = self.crossing( num1bin, num2bin )
			numfloat = binToFloat(numbin)
			self.populationList.append( Chromosome(numfloat) )


	def crossing(self, num1, num2):
		tmp = ""
		for x in xrange(0,len(num1)):
			if( x%2==0 ):
				tmp = tmp + num1[x]
			else:
				tmp = tmp + num2[x]
		return tmp

	def mutation(self):
		indiv = None
		for i in xrange(0,len(self.populationList)):
			indiv = self.populationList[i]
			ybinstr = floatToBin( indiv.y )
			ybinstr = self.changeBits(ybinstr)
			indiv.setX( binToFloat(ybinstr) )
				
				
	def changeBits(self, numbinstr):
		for i in xrange(0,8):
			if( randint(0,100) < self.mutationProb ): 
				if numbinstr[i] =="1":
					numbinstr.replace("1","0",i)
					#numbinstr[i] = "0"
				else:
					numbinstr.replace("0","1",i)
					#numbinstr[i] = "0"
		return numbinstr

	def printPopulation(self):
		print "Indiv", "		x 	", "	y "
		for x in xrange(0, len(self.populationList) ):
			print str(x+1) +"	"+ str(self.populationList[x].x) + "	" + str(self.populationList[x].y)

		print "\n----------------------------------------------------------\n"

	def removeChromosomes(self, array, number):
		return array[:len(array)-number]

	def sort(self, array):
		array.sort(key=self.chromosomesComp)

	def chromosomesComp(self, chromosome):
		return chromosome.y




