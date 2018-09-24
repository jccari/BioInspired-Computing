import struct

def integerToBinary(integer):
	return '{0:04b}'.format(integer)

def printList(array):
	for x in xrange(0,len(array)):
		print array[x]

def floatToBin(num):
    return bin(struct.unpack('!I', struct.pack('!f', num))[0])[2:].zfill(32)

def binToFloat(binary):
    return struct.unpack('!f',struct.pack('!I', int(binary, 2)))[0]
    
'''
a = floatToBin(0.53234234)
b = floatToBin(-1.45444)
#print a
#print b
#print c, type(c)
#print binToFloat(c) , type(binToFloat(c))

a = 1000
for x in xrange(1,50):
	print a , floatToBin(a)
	a = a + 100
'''