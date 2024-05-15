from flask import Flask, render_template, request
from yolov5.detect import run  # Assuming detect.py is in the 'folder' directory

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/predict', methods=['POST'])
def predict():
    # Run YOLOv5 detection on the predefined source (e.g., webcam or image file)
    run(weights=r'yolov5\runs\train\exp\weights\best.pt', source='http://192.168.100.57:81/stream',save_csv=True)  # Modify the source as needed
    
    # Return to the client
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)
