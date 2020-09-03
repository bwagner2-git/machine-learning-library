import numpy as np
import easynn as nn

# Create a numpy array of 10 rows and 5 columns.
# Set the element at row i and column j to be i+j.
def Q1():
    n=np.ones((10,5))
    for i in range(10):
        for j in range(5):
            n[i,j]=i+j
    return n

# Add two numpy arrays together.
def Q2(a, b):
    return a+b

# Multiply two 2D numpy arrays using matrix multiplication.
def Q3(a, b):
    return np.dot(a,b)

# For each row of a 2D numpy array, find the column index
# with the maximum element. Return all these column indices.
def Q4(a):
    ans=[]
    row=0
    while True:
        try:    ###start with first row
            col=0 ###start with first column in row
            big=a[row,0]
            c=0
            while True:
                try:
                    if a[row,col]>big:
                        c=col
                    col+=1
                except:
                    ans.append(int(c))
                    row+=1
                    break
        except:
            break
    return ans

# Solve Ax = b.
def Q5(A, b):
    return np.linalg.inv(A)*b

# Return an EasyNN expression for a+b.
def Q6():
    return None

# Return an EasyNN expression for a+b*c.
def Q7():
    return None

# Given A and b, return an EasyNN expression for Ax+b.
def Q8(A, b):
    return None

# Given n, return an EasyNN expression for x**n.
def Q9(n):
    return None

# Return an EasyNN expression to compute
# the element-wise absolute value |x|.
def Q10():
    return None
