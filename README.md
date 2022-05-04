## Introduction
The K-means algorithm is a popular clustering method for finding a partition of N 
unlabeled observations into K distinct clusters, where K is a parameter of the method.
we willimplement this algorithm in both Python and C. 
## K-means
Given a set of ![equation](https://latex.codecogs.com/svg.image?\mathbb{N}) datapoints such that ![equation](https://latex.codecogs.com/svg.image?x_1,x_2\dots&space;x_n&space;\in&space;\mathbb{R}^d). the goal is to group the data into ![equation](https://latex.codecogs.com/svg.image?k\in&space;\mathbb{N}) clusters,
each datapoint is assigned to exactly one cluster and the number of clusters ![equation](https://latex.codecogs.com/svg.image?k) is such that ![equation](https://latex.codecogs.com/svg.image?1<k<n).
We will denote the group of clusters by ![equation](https://latex.codecogs.com/svg.image?S_1,&space;S_2,&space;\dots&space;,&space;S_k), each cluster ![equation](https://latex.codecogs.com/svg.image?S_j)  is represented by it’s centroid
which is the mean ![equation](https://latex.codecogs.com/svg.image?\mu_j&space;\in&space;\mathbb{R}^d) of the cluster’s members.
