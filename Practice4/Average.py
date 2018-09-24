from random import randint
#from math import abs

class Average():
	def __init__(self):
		self.alpha1 = 0.25
		self.alpha2 = 0.50
		self.alpha3 = 0.75
		self.alpha4 = 0.05
		self.alpha5 = 0.95
		self.real_avg = []
		self.current_avg1 = []
		self.current_avg2 = []
		self.current_avg3 = []
		self.current_avg4 = []
		self.current_avg5 = []

	def get_average(self):
		pass

	#By default this function generates a list of random number	with 500 elements in range of 0 to 100
	def get_random_list(self, max_elements=500):
		randomList = []
		for x in xrange(0,max_elements):
			number = randint(0,100)
			randomList.append(number)
		return randomList

	def calculate_averages(self, listElements):
		last_avg1 = listElements[0] # default
		last_avg2 = listElements[0] # default
		last_avg3 = listElements[0] # default
		last_avg4 = listElements[0] # default
		last_avg5 = listElements[0] # default
		#last_real_avg = listElements[0]
		
		for i in xrange(0, len(listElements)):
			tmp1 = self.alpha1*last_avg1 + (1-self.alpha1)*listElements[i]
			tmp2 = self.alpha2*last_avg2 + (1-self.alpha2)*listElements[i]
			tmp3 = self.alpha3*last_avg3 + (1-self.alpha3)*listElements[i]
			tmp4 = self.alpha4*last_avg4 + (1-self.alpha4)*listElements[i]
			tmp5 = self.alpha5*last_avg5 + (1-self.alpha5)*listElements[i]
			#tmp6 = (last_real_avg*(i-1)+listElements[i])/i
			self.current_avg1.append( tmp1 )
			self.current_avg2.append( tmp2 )
			self.current_avg3.append( tmp3 )
			self.current_avg4.append( tmp4 )
			self.current_avg5.append( tmp5 )
			#self.real_avg.append( tmp6 )
			last_avg1 = tmp1
			last_avg2 = tmp2
			last_avg3 = tmp3
			last_avg4 = tmp4
			last_avg5 = tmp5
			#last_real_avg = tmp6

	#Main function
	def init(self, max_elements):
		randomList = self.get_random_list(max_elements)
		self.calculate_averages(randomList);
		print "i","real avg","avg 1","avg 2","avg 3","avg 4","avg 5", "i nearest","dist nearest"
		for i in xrange(0,len(self.current_avg1)):
			tmpList = []
			subList = randomList[:i+1]
			real_avg = self.real_average(subList)
			tmpList.append(self.current_avg1[i])
			tmpList.append(self.current_avg2[i])
			tmpList.append(self.current_avg3[i])
			tmpList.append(self.current_avg4[i])
			tmpList.append(self.current_avg5[i])
			nearest_pos = self.nearest(real_avg, tmpList)
			print i+1, real_avg, randomList[i], tmpList[0], tmpList[1], tmpList[2], tmpList[3], tmpList[4], nearest_pos, abs(tmpList[nearest_pos]-real_avg)
			# print i+1, randomList[i], self.current_avg1[i], self.current_avg2[i], self.current_avg3[i], self.current_avg4[i], self.current_avg5[i]
			
	def real_average(self, elementList):
		elementSum = 0
		for i in xrange(0, len(elementList)):
			elementSum += elementList[i]
		return float(elementSum)/len(elementList)

	def nearest(self, number, elementList):
		lower_dist = abs(elementList[0]-number)
		pos = 0
		for i in xrange(1,len(elementList)):
			#print abs(elementList[i]-number)
			if (abs(elementList[i]-number) < lower_dist) :
				pos = i
				lower_dist = abs(elementList[i]-number)

		return pos
