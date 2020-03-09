import cv2
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation

numBits = 32

# Conversion function for integer -> binary. Returns a list
# Format int X -> binary repr
# binary_repr -> string -> list
# map (int) over list
# create new list of ints from original list
# create np array L from then new list of ints
# return np array L
def conv(x):
    a = format(x, '032b')
    l = list(str(a))
    l = np.array(list(map(int, l)))
    return l

trainExamples = 10000

model = Sequential()

# create input / hidden layer
# input dim = 64 due to 2 32bit inputs (ints in 32bit binary)
model.add(Dense(units=128, activation='relu', input_dim=2*numBits))  
model.add(Dense(units=1, activation='sigmoid')) # connect hidden layer to output layer
model.summary()

# configure the learning process, choose loss function, loss minimizer (optimizer)
# and metrics
model.compile(loss='binary_crossentropy',
              optimizer='rmsprop',
              metrics=['accuracy'])

# x_1 are all the first inputs
x_1 =  np.random.randint(100, size=trainExamples)
# we will convert each int in x_1 to its 32bit binary representation
x_1b = np.zeros(shape=(trainExamples, numBits))

# x_2 are all the second inputs
x_2 = np.random.randint(100, size=trainExamples)
x_2b = np.zeros(shape=(trainExamples, numBits))

for i in range(trainExamples):
    x_1b[i] = conv(x_1[i])
    x_2b[i] = conv(x_2[i])
xs = np.column_stack([x_1b, x_2b])

# ys = the training labels
ys = np.abs(x_1 - x_2)

# label each entry for y as either even or odd (1= even 0= odd)
for i in range(trainExamples):
    if (ys[i] % 2 == 0):
        ys[i] = 1
    else:
        ys[i] = 0

# train the AI on the 2 feature input data, and the labels
model.fit(xs, ys, epochs=15, batch_size=128)
#print("Weights = ", model.get_weights(), "\n\n")

# save state
model.save('absModel.h5')

while(True):
    # set int value, create array of shape (1,32), set that row to the binary
    # representation of the int value.
    x = int(input('X value? '))
    x_b = np.zeros(shape=(1,numBits))
    x_b[0] = conv(x)
   
    # set int value, create array of shape (1,32), set that row to the binary
    # representation of the int value.
    y = int(input('Y value? '))
    y_b = np.zeros(shape=(1,numBits))
    y_b[0] = conv(y)

    # stack both columns to create input vector (x_1, x_2)
    test = np.column_stack([x_b,y_b])
    print("DIFFERENCE: ", np.abs(x-y))

    # the model will predict close to 1 if the difference of both numbers is even, 
    # close to 0 if it is odd
    if (model.predict(test) > 0.98):
        print(np.abs(x-y), "is EVEN")
    else:
        print(np.abs(x-y), "is ODD")
