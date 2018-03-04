from flask import Flask, render_template

app =  Flask(__name__)

@app.route('/')

def hello():

	#return "Hello from Hongji on AWS!"
	return render_template("main.html")

app.run(host = '0.0.0.0', port = 80)