from flask import Flask, jsonify, request, abort, render_template

app = Flask(__name__)
dataSensor = {'value' : [], 'timestamp' : []}


@app.route('/')
@app.route('/home')
def home():
    return render_template('index.html', dataList = dataToList())

@app.route('/data', methods=['GET'])
def sendData():
    return jsonify({'data' : dataSensor})


@app.route('/data', methods=['POST'])
def addData():
    if not request.is_json:
        abort(400, "sssss")
    content = request.get_json()
    if type(content['value']) != float or type(content['timestamp']) != int:
        abort(400, "Invalid values")
    dataSensor['value'].append(content['value'])
    dataSensor['timestamp'].append(content['timestamp'])
    return jsonify({'data' : dataSensor})

def dataToList():
    n = len(dataSensor['value'])
    return [(dataSensor['timestamp'][i], dataSensor['value'][i]) for i in range(n)]

if __name__ == '__main__':
    app.run(host='192.168.0.241', port=5000, threaded=True)