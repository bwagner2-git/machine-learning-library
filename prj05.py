import numpy as np
import time

# save theta to p5_params.npz that can be used by easynn
def save_theta(theta):
    f1_W, f1_b, f2_W, f2_b = theta

    np.savez_compressed("p5_params.npz", **{
        "f1.weight": f1_W,
        "f1.bias": f1_b,
        "f2.weight": f2_W,
        "f2.bias": f2_b
    })


# initialize theta using uniform distribution [-bound, bound]          
# return theta as (f1_W, f1_b, f2_W, f2_b)
def initialize_theta(bound):
    f1_W = np.random.uniform(-bound, bound, (32, 784))
    f1_b = np.random.uniform(-bound, bound, 32)
    f2_W = np.random.uniform(-bound, bound, (10, 32))
    f2_b = np.random.uniform(-bound, bound, 10)
    return (f1_W, f1_b, f2_W, f2_b)


# forward:
#   x = Flatten(images)
#   g = Linear_f1(x)
#   h = ReLU(g)
#   z = Linear_f2(h)
# return (z, h, g, x)
def forward(images, theta):
    # number of samples
    N = images.shape[0]

    # unpack theta into f1 and f2
    f1_W, f1_b, f2_W, f2_b = theta

    # x = Flatten(images)
    x = images.astype(float).transpose(0,3,1,2).reshape((N, -1))

    # g = Linear_f1(x)
    g = np.zeros((N, f1_b.shape[0]))             #x is the input of images think of it as tensors?
    for i in range(N):
        g[i, :] = np.matmul(f1_W, x[i])+f1_b     ###think of x g h and z as tensors? 

    # h = ReLU(g)
    h = g*(g > 0)

    # z = Linear_f2(h)
    z = np.zeros((N, f2_b.shape[0]))
    for i in range(N):
        z[i, :] = np.matmul(f2_W, h[i])+f2_b

    return (z, h, g, x)


# backprop:
#   J = cross entropy between labels and softmax(z)
# return nabla_J
def backprop(labels, theta, z, h, g, x):      ###what are these
    # number of samples
    N = labels.shape[0]

    # unpack theta into f1 and f2
    f1_W, f1_b, f2_W, f2_b = theta

    # nabla_J consists of partial J to partial f1_W, f1_b, f2_W, f2_b
    p_f1_W = np.zeros(f1_W.shape)
    p_f1_b = np.zeros(f1_b.shape)
    p_f2_W = np.zeros(f2_W.shape)
    p_f2_b = np.zeros(f2_b.shape)

    for i in range(N):    ### one loop is one sample image right?
        # compute the contribution to nabla_J for sample i
                                                ### nabla is del
        # cross entropy and softmax
        #   compute partial J to partial z[i]
        #   scale by 1/N for averaging
        expz = np.exp(z[i]-max(z[i]))   ### why - max z[i] ? Isnt z[i] a double?  
        p_z = expz/sum(expz)/N     ### part A in hw 3 calculating partial of crossent loss with repsect to each one of the outputs why are we dividing by N here
        p_z[labels[i]] -= 1/N ### why are we doing this? this is the -1 if if label/yhat = to 1 isnt it

        # print(p_z)
        # print(np.transpose(p_z.reshape(10,1)))   ###testing making sure what is happening is what I thought
        # time.sleep(10)

        # z = Linear_f2(h)
        #   compute partial J to partial h[i]
        #   accumulate partial J to partial f2_W, f2_b
        # ToDo: uncomment code below to add your own code ## part b hw 3
        p_h = np.matmul(np.transpose(f2_W),p_z)   ### partial of j with respect to h    why not += here? 
        p_f2_W += np.matmul(p_z.reshape(10,1),np.transpose(h[i].reshape(32,1))) ##p_z has already been multiplied by 1/N so we do not need to again right?   ####add one nth of value obtained using equations in hw3
        p_f2_b += p_z
        

        # h = ReLU(g)
        #   compute partial J to partial g[i]
        # ToDo: uncomment code below to add your own code hw 3 part c
        p_g = np.multiply(np.heaviside(g[i],0),p_h) ###should 0 equal a 0 or a 1?

        # g = Linear_f1(x)
        #   accumulate partial J to partial f1_W, f1_b
        # ToDo: uncomment code below to add your own code
        p_f1_W += np.matmul(p_g.reshape(32,1),np.transpose(x[i].reshape(784,1)))/N
        
        p_f1_b += p_g/N                            ###try dividing everything added by N if this does not work

    return (p_f1_W, p_f1_b, p_f2_W, p_f2_b)


# apply SGD to update theta by nabla_J and the learning rate epsilon
# return updated theta
def update_theta(theta, nabla_J, epsilon):
    # ToDo: modify code below as needed
    updated_theta = (np.subtract(theta[0],np.multiply(epsilon,nabla_J[0])),
    np.subtract(theta[1],np.multiply(epsilon,nabla_J[1])),
    np.subtract(theta[2],np.multiply(epsilon,nabla_J[2])),
    np.subtract(theta[3],np.multiply(epsilon,nabla_J[3])))

    
    return updated_theta


# ToDo: set numpy random seed to the last 8 digits of your CWID
np.random.seed(20430439)

# load training data and split them for validation/training
mnist_train = np.load("mnist_train.npz")
validation_images = mnist_train["images"][:1000]
validation_labels = mnist_train["labels"][:1000]
training_images = mnist_train["images"][1000:]
training_labels = mnist_train["labels"][1000:]

# hyperparameters
bound = 1 # initial weight range
#epsilon = 0.00001 # learning rate
epsilon =.0000008
batch_size = 4

# start training
start = time.time()
theta = initialize_theta(bound)
batches = training_images.shape[0]//batch_size
for epoch in range(10):
    indices = np.arange(training_images.shape[0])
    np.random.shuffle(indices)
    for i in range(batches):
        batch_images = training_images[indices[i*batch_size:(i+1)*batch_size]]
        batch_labels = training_labels[indices[i*batch_size:(i+1)*batch_size]]

        z, h, g, x = forward(batch_images, theta)
        nabla_J = backprop(batch_labels, theta, z, h, g, x)
        theta = update_theta(theta, nabla_J, epsilon)

    # check accuracy using validation examples
    z, _, _, _ = forward(validation_images, theta)
    pred_labels = z.argmax(axis = 1)
    count = sum(pred_labels == validation_labels)
    print("epoch %d, accuracy %.3f, time %.2f" % (
        epoch, count/validation_images.shape[0], time.time()-start))

# save the weights to be submitted
save_theta(theta)
