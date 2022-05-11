import math

def kmeans(k, input_filename, output_filename, max_iter = 200):
    if max_iter <=0 or k <=0:
        print("Invalid Input!")
        return
    input_vector_list = []

    input = open(input_filename, 'r')
    line = input.readline()
    while line != "":
        temp_vector = line.split(',')
        for index in range(len(temp_vector)):
            temp_vector[index] = float(temp_vector[index])
        input_vector_list.append(temp_vector)
        line = input.readline()
    input.close()

    if k > len(input_vector_list): # less vectors than k
        print("An Error Has Occurred")
        return
    d = len(input_vector_list[0])

    previuos_center = [input_vector_list[i] for i in range(k)]

    for iter in range(max_iter):

        current_center = [input_vector_list[i] for i in range(k)]
        cluster_sum = [[0 for i in range(d+1)] for j in range(k)]  # calculates the sum of each vector in all clusters
                                                                   # first cell is a counter
        for vector in input_vector_list: # prepping for vector cluster matching
            min =  float('inf')
            centeroid_index = 0

            for i in range(len(previuos_center)):# the loop to find which input vector goes to which cluster
                temp_distance = distance(vector, previuos_center[i])
                if temp_distance < min:
                    min = temp_distance
                    centeroid_index = i

            cluster_sum[centeroid_index][0] += 1 # the cluster which a vector was matched with gets +1 counter
            for i in range(d):
                cluster_sum[centeroid_index][i+1]+= vector[i] # adds that vector's value to the centeroid

        convergence = [0 for i in range(k)] # array for delta mew's convergence
        for i in range(len(cluster_sum)): # update the centeroids
            cluster_size = cluster_sum[i][0]
            if(cluster_size == 0):
                print("An Error Has Occurred")
                return
            current_center[i] = [cluster_sum[i][j]/cluster_size for j in range(1,d+1)]
            if distance(current_center[i],previuos_center[i]) < 0.001:
                convergence[i] = 1

        if sum(convergence) == k:
            break
        else:
            previuos_center = current_center

    output = open(output_filename, 'w')
    for i in range(len(current_center)):
        current_center[i] = [str("%.4f" % round(current_center[i][j], 4)) for j in range(d)]
        line = ','.join(current_center[i])
        output.write(line + '\n')
    output.close()

def distance(x , y):
    result = 0
    for i in range(len(x)):
        result += (x[i]-y[i])**2
    return (result**0.5)