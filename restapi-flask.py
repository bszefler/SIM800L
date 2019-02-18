from flask import Flask, jsonify, request, abort, render_template

app = Flask(__name__)
dataSensor = {'value' : [], 'timestamp' : []}
average = 0.0


@app.route('/')
@app.route('/home')
def home():
    return render_template('index.html', dataList = dataToList(), avg = average )

@app.route('/data', methods=['GET'])
def sendData():
    return jsonify({'data' : dataSensor})


@app.route('/data', methods=['POST'])
def addData():
    global average
    if not request.is_json:
        abort(400)
    content = request.get_json()
    if type(content['value']) != float or type(content['timestamp']) != int:
        abort(400, "Invalid values")

    dataSensor['value'].append(content['value'])
    dataSensor['timestamp'].append(content['timestamp'])
    updateAverage(content['value'], len(dataSensor['value']))

    return jsonify({'data' : dataSensor})

def updateAverage(nextValue, length):
    global average
    average = average + (nextValue- average) / length

def dataToList():
    n = len(dataSensor['value'])
    return [(dataSensor['timestamp'][i], dataSensor['value'][i]) for i in range(n)]

if __name__ == '__main__':
    app.run(host='192.168.0.241', port=8080, threaded=True)