# Verus - Delay Profile
Verus is an end-to-end congestion control protocol that uses delay measurements to react quickly to the capacity changes in cellular networks without explicitly attempting to predict the cellular channel dynamics. The key idea of Verus is to continuously learn a delay profile that captures the relationship between end-to-end packet delay and outstanding window size over short epochs and uses this relationship to increment or decrement the window size based on the observed short-term packet delay variations.

Delay profile is the implementation of an approach to find and maintain a monotonically increasing curve for window/delay value pairs.

##Compiling with g++ for Testing Purposes
g++ -fpermissive -std=c++11 DelayProfile.cpp PerlinNoise.cpp


