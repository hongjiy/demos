import sys
sys.path.append('/usr/local/lib/python2.7/site-packages')
from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def hello():

	return render_template("index_edited - Copy.html")


if __name__ == "__main__":

	app.run(host = '127.0.0.1', port = 5005)