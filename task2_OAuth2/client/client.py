from flask import Flask, render_template, redirect,request

app=Flask(__name__)

client_id='abc'
redirect_uri='http://localhost:5050/edit_photo'

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/login')
def login():
    uri='http://localhost:5000/oauth?response_type=code&client_id={}&redirect_uri={}'.format(client_id,redirect_uri)
    return redirect(uri)

@app.route('/passport')
def passport():
    code=request.args.get('code')
    uri='http://localhost:5000/oauth?grant_type=authorization&code={}&redirect_uri=http://localhost:5050/edit_photo&client_id={}'.format(code,client_id)
    return redirect(uri)

@app.route('/edit_photo')
def edit_photo():
    username=request.args.get('user')
    token=request.args.get('token')
    premission=request.args.get('token')
    return render_template('edit_photo.html',username=username,token=token,redirect_uri=redirect_uri)

upload_api_url='http://localhost:5000/upload_api'
@app.route('/upload',methods=['GET','POST'])
def upload():
    username=request.args.get('username')
    token=request.args.get('token')
    premission=request.args.get('premission')
    redirect_uri=request.args.get('redirect_uri')
    return render_template('upload.html',upload_api_url=upload_api_url,username=username,client_id=client_id,token=token,premission=premission,redirect_uri=redirect_uri)
