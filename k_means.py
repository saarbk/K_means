import sys

EPSILON = 0.001
K, input_file, output_file, max_iter = (0, '', '', int(200))
data, centroids, eror_rate = ([], [], False)

sys_input = sys.argv

def k_means():
    """
    Main function Implement k-means clustering algorithm
    """
    cnt = 0
    while cnt < max_iter or eror_rate:
        clusters = [[] for i in range(K)]
        cnt += 1
        eror_rate = True
        """ line 3-4: Assign x_i to closest cluster."""
        for x_i in data:
            index = argmin(x_i)
            clusters[index].append(x_i)
        old_centroids = centroids

        """ line 5-6: update centroids."""
        for mu_k in range(K):
            centroids[mu_k] = [float(sum(l)) / len(l) for l in zip(*clusters[mu_k])]
            if distance(centroids[mu_k], old_centroids[mu_k]) <= EPSILON:
                eror_rate = False
    return centroids


def argmin(point):
    min_index = [distance(point, c) for c in centroids]
    return min(range(len(min_index)), key=min_index.__getitem__)


def distance(list1, list2):
    """Distance between two vectors."""
    squares = [(p - q) ** 2 for p, q in zip(list1, list2)]
    return sum(squares) ** .5


""" Set user inputs """
try:
    if len(sys_input) == 5:
        K, input_file, output_file, max_iter = (int(sys_input[1]), sys_input[3], sys_input[4], int(sys_input[2]))
    else:
        if len(sys.argv) == 4:
            K, input_file, output_file = (int(sys_input[1]), sys_input[2], sys_input[3])
        else:
            breakpoint("Invalid Input!")

        """ Read data from src """
    for line in open(input_file):
        point = [float(i) for i in line.strip('\n').split(',')]
        data.append(point)

except ValueError:
    raise ValueError("Invalid Input!") from None

try:
    """ Create K-clusters from k first elements. """
    centroids = data[:K]

    """ K-means run """
    result = k_means()

except ValueError:
    raise ValueError(" An Error Has Occurred. ") from None

""" Write the centroids to Data file. """
W = open(output_file, 'w')
write_data = []
for line in result:
    form = '{:.4f},' * len(line)
    form = (form.format(*line))
    write_data.append(form[:-1] + '\n')

W.writelines(write_data)
# close files
W.close()
