# AI based crop maintanance system
## Introduction
This project is based on idea of using AI to help farmers maintain crops using an AI powered smart car that will detect and spray pesticides on the crops. The project is based on the idea of using a edge processing for inference and a camera module to process the images and a motor driver to control the motors. The project is still in development and is not yet complete.
## Dependencies
specified in requirements.txt file 

# Usage
## step 1
- pip install -r requirements.txt
## step 2
change the current working directory to ./yolov5
- cd yolov5
## step 3
use the detect.py file to run the inferences and specify the source after the --source flag
- python detect.py --weights runs/train/exp/weights/best.pt --source 
