import numpy as np
import matplotlib.pyplot as plt

class Perceptron(object):
    def __init__(self, _xn_matrix, _results, _learning_rate=0.5, _max_iters = 20):
        self.xn_matrix = _xn_matrix #[[BIAS], [INPUT 1], [INPUT 2]]
        self.results = _results
        self.learning_rate = _learning_rate
        self.num_entries = len(self.results)
        self.weights = [0,0,0]  # [w0, w1, w2] #inicialize default weights
        self.max_iters = _max_iters

        self.plotPoint( self.xn_matrix[1][0], self.xn_matrix[2][0])
        self.plotPoint( self.xn_matrix[1][1], self.xn_matrix[2][1])
        self.plotPoint( self.xn_matrix[1][2], self.xn_matrix[2][2])
        self.plotPoint( self.xn_matrix[1][3], self.xn_matrix[2][3])

    def train(self):
        iters = 0
        while iters < self.max_iters:
            testing_list = []
            for i in range(0, self.num_entries):
                entrie = self.get_entrie(i)
                desired = self.results[i]
                y_out = self.compute_y_out(entrie, self.weights)
                testing_list.append(y_out == desired)

                if y_out != desired:
                    self.weights = self.update_weights(self.weights, entrie, desired, y_out)

            all_accepted = self.check_all_accepted(testing_list)
            print self.weights
            self.plotLine( self.weights[1], self.weights[2], self.weights[0], True)
            if (all_accepted):
                break
            iters = iters + 1

        self.plotLine( self.weights[1], self.weights[2], self.weights[0], False)
        return self.weights

    def check_all_accepted(self, test_list):
        accepted = True
        for i in range(0, len(test_list)):
            if( test_list[i] == False):
                accepted = False
        return accepted

    def compute_y_out(self, entries_list, weight_list):
        sumatory = 0
        for i in range(0, self.num_entries-1):
            sumatory += weight_list[i]*entries_list[i]

        return self.activation_func(sumatory)

    def activation_func(self,x):
        return -1 if x <= 0 else 1
        #return 0 if x <= 0 else 1

    def get_entrie(self, j):
        entrie = []
        for i in range(0, self.num_entries -1):
            entrie.append(xn_matrix[i][j])
        return entrie

    def update_weights(self, weight_list, entrie_list ,desired_res, _y_out):
        weight_updated = []
        for i in range(0, len(self.xn_matrix)):
            weight_updated.append( weight_list[i] + self.learning_rate*(desired_res-_y_out)*entrie_list[i] )
            #print weight_list[i],"+",self.learning_rate,"*(",desired_res,"-",_y_out,")*",entrie_list[i]
        #print "  >> updating weights ",weight_updated;
        return weight_updated

    def plotLine(self, a, b, bias, remove):
        x = np.linspace(-2, 2, 100)
        try:
            ln, = plt.plot( x, -(b/a)*x-(bias/a) )
            #print "[DEBUG] Ploteando: Y = %s X - %s " %(str(-(b/a)), str((bias/a)))
        except ZeroDivisionError:
            ln, = plt.plot( x, -(b/(a+0.1))*x-(bias/(a+0.1)))
        except Exception:
            raise("[ERROR]: Exception unrecognized")

        plt.pause(0.1)

        if remove:
            ln.remove()

    def plotPoint(self, x, y):
        plt.plot(x,y,"bo")
        

if __name__ == '__main__':

    xn_matrix = [[1,1,1,1],[-1,-1,1,1],[-1,1,-1,1]] # matriz = [ [X_0],[X_1],[X_2] ]
    #results = [-1,-1,-1,1] #AND
    #results = [-1, 1, 1, 1] #OR
    results = [-1,1,1,-1] #XOR
    perceptron = Perceptron( xn_matrix, results)
    
    result = perceptron.train()

    print "\nEcuacion final :"
    print "\t %s*X + %s+Y + %s = 0" % (str(result[1]), str(result[2]), str(result[0]) )

    plt.show()
