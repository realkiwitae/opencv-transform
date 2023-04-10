
import itertools
import numpy as np
import math

def my_function(array, angle_in_degrees):
    (h,w) = array.shape[:2]
    
    result = np.zeros_like(array).astype(np.uint8)
    
    angle = angle_in_degrees * math.pi / 180
    (cosine, sine) = np.cos(angle), np.sin(angle)
    rotation_matrix = [[cosine, sine], [-sine, cosine]]
    
    for x,y in itertools.product(range(w), range(h)):
        pixels = array[y,x,:]
        new_x, new_y = np.matmul(rotation_matrix, [x,y])
        new_x, new_y = int(new_x), int(new_y)
        if 0 <= new_x < w and 0 <= new_y < h:
            result[new_y, new_x, :] = pixels
            
    return result