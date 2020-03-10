import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Activation

# This program takes as input 2 integers, and returns whether the
# absolute difference of them is even or odd using a neural network.
# Inputs to the network are integers in their binary representation in
# order for the network to learn the feature that differentiates odd 
# numbers from even (the rightmost bit).

# Conversion function (integer -> binary). Returns a list as np array.
# Formats int(x) -> binary repr -> string -> list -> np.array
# then maps (int) over list
# Creates a new list of ints from original list
# Creates a np array L from the new list of ints
# Return np array L
def conv(x):
    a = format(x, '032b')    # Must match numBits
    l = list(str(a))
    l = np.array(list(map(int, l)))
    return l

numBits = 32    # Must match format length
trainExamples = 10000

# Define the architecture
model = Sequential()
# Create input / hidden layer
model.add(Dense(units=128, activation='relu', input_dim=2*numBits))
# Connect hidden layer to output layer
model.add(Dense(units=1, activation='sigmoid'))
# Print the model (The NN architecture with weights as free params)
model.summary()

# Configure the learning process:
# Choose a loss function, a loss minimizer (optimizer),
# and metrics
model.compile(loss='binary_crossentropy',
              optimizer='rmsprop',
              metrics=['accuracy'])

# x_1 are all the first inputs
x_1 =  np.random.randint(100, size=trainExamples)
# We will convert each int in x_1 to its 32bit binary representation
x_1b = np.zeros(shape=(trainExamples, numBits))

# x_2 are all the second inputs
x_2 = np.random.randint(100, size=trainExamples)
x_2b = np.zeros(shape=(trainExamples, numBits))

for i in range(trainExamples):
    x_1b[i] = conv(x_1[i])
    x_2b[i] = conv(x_2[i])
xs = np.column_stack([x_1b, x_2b])

# ys will be the training labels / truth values
# Initially, it contains the absolute difference of all values from
# x_1 and x_2.
ys = np.abs(x_1 - x_2)

# Label each entry in ys as either even or odd (1 = even, 0 = odd)
for i in range(trainExamples):
    if (ys[i] % 2 == 0):
        ys[i] = 1
    else:
        ys[i] = 0

# Train the network on the 2 feature input data, and the labels
model.fit(xs, ys, epochs=15, batch_size=128)

# Save state
model.save('absModel.h5')

while(True):
    # Set int value, create array of shape (1,32), set that row to the 
    # binary representation of the int value.
    x = int(input('X value? '))
    x_b = np.zeros(shape=(1,numBits))
    x_b[0] = conv(x)
   
    # Set int value, create array of shape (1,32), set that row to the 
    # binary representation of the int value.
    y = int(input('Y value? '))
    y_b = np.zeros(shape=(1,numBits))
    y_b[0] = conv(y)

    # Stack both columns to create input vector 
    # (x_1,x_2) of shape (1,64)
    test = np.column_stack([x_b,y_b])
    print("DIFFERENCE: ", np.abs(x-y))

    # The model will predict close to 1 if the difference of both 
    # numbers is even, close to 0 if it is odd
    if (model.predict(test) > 0.98):
        print(np.abs(x-y), "is EVEN")
    else:
        print(np.abs(x-y), "is ODD")
