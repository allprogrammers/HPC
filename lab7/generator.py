#!python3
import random

NUM_ELEMENTS = 1000000

filename1 = "vector_x.txt"
filename2 = "vector_y.txt"

file1 = open(filename1,"w")
file2 = open(filename2,"w")

ftext1 = [str(random.randint(1,1000000)) for x in range(NUM_ELEMENTS)]
ftext2 = [str(random.randint(1,1000000)) for x in range(NUM_ELEMENTS)]



file1.write("\n".join(ftext1))
file1.close()

file2.write("\n".join(ftext2))
file2.close()