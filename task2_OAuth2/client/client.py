from flask import Flask, render_template, redirect,request

app=Flask(__name__)

client_id='abc'
redirect_uri='http://localhost:5050/passport'

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/login')
def login():
    uri='http://localhost:5000/auth?response_type=code&client_id={}&redirect_uri={}'.format(client_id,redirect_uri)
    return redirect(uri)

@app.route('/passport')
def passport():
    code=request.args.get('code')
    uri='http://localhost:5000/auth?grant_type=authorization&code={}&redirect_uri=http://localhost:5050/edit_photo&client_id={}'.format(code,client_id)
    return redirect(uri)

@app.route('/edit_photo')
def edit_photo():
    user=request.args.get('user')
    return render_template('edit_photo.html')
