import os

from flask import Flask
from flask import url_for
from flask import render_template
from flask_wtf.csrf import CSRFProtect

csrf = CSRFProtect()

app = Flask(__name__)

SECRET_KEY = os.urandom(32)
app.config['SECRET_KEY'] = SECRET_KEY

csrf.init_app(app)


@app.route("/")
def trading_page():
    return render_template("index.html")

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=8000, debug=True)
    url_for('static', filename='index.html')