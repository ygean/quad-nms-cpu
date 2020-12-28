import cv2
import numpy as np

original_quads = [
	95,71,80,255,332,250,374,80,0.9,
	43,175,461,171,409,308,55,309,0.8,
	62,107,417,114,408,227,34,216,0.85,
	215,426,469,424,474,595,219,595,0.8,
	175,457,440,460,451,635,178,624,0.75,
	255,371,264,609,467,618,449,370,0.6
]
original_quads = np.array(original_quads, dtype=np.int)
original_quads = original_quads.reshape((-1, 9))

quads = [
	95,71,80,255,332,250,374,80,0.9,
	215,426,469,424,474,595,219,595,0.8
	]


quads = np.array(quads, dtype=np.int)
quads = quads.reshape((-1, 9))
width = height = 768
canvas0 = np.zeros((height, width, 3), dtype=np.uint8)
canvas1 = np.zeros((height, width, 3), dtype=np.uint8)
## nms前的颜色(255, 255, 0)
cv2.polylines(canvas0, original_quads[:, :8].reshape((-1, 4, 2)), True, (255, 255, 0), 2, 16)
cv2.imshow("before nms", canvas0)
## nms后的颜色(0, 255, 255)
cv2.polylines(canvas1, quads[:, :8].reshape((-1, 4, 2)), True, (0, 255, 255), 2, 16)
cv2.imshow("after nms", canvas1)
cv2.waitKey()
