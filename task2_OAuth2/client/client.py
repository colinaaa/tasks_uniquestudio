import requests
from flask import Flask, redirect, render_template, request

client=Flask(__name__)

client_id='abc'
redirect_uri='http://localhost:5050/edit_photo'
passport_uri='http://localhost:5050/passport'

@client.route('/')
def index():
    return render_template('index.html')

@client.route('/login')
def login():
    uri='http://localhost:5000/oauth?response_type=code&client_id={}&redirect_uri={}&passport_uri={}'.format(client_id,redirect_uri,passport_uri)
    return redirect(uri)

@client.route('/passport')
def passport():
    code=request.args.get('code')
    uri='http://localhost:5000/oauth?grant_type=authorization&code={}&redirect_uri=http://localhost:5050/edit_photo&client_id={}'.format(code,client_id)
    return redirect(uri)

@client.route('/edit_photo')
def edit_photo():
    username=request.args.get('user')
    token=request.args.get('token')
    premission=request.args.get('premission')
    return render_template('edit_photo.html',username=username,token=token,redirect_uri=redirect_uri,premission=premission,client_id=client_id)

upload_api_url='http://localhost:5000/upload_api'
@client.route('/upload',methods=['GET','POST'])
def upload():
    username=request.args.get('username')
    token=request.args.get('token')
    premission=request.args.get('premission')
    redirect_uri=request.args.get('redirect_uri')
    return render_template('upload.html',upload_api_url=upload_api_url,username=username,client_id=client_id,token=token,premission=premission,redirect_uri=redirect_uri)

@client.route('/photos/<username>')
def photos(username):
    token=request.args.get('token')
    premission=request.args.get('premission')
    redirect_uri=request.args.get('redirect_uri')
    api_url='http://localhost:5000/info_api/'+username
    url=api_url+'?token='+token+'&premission='+premission+'&client_id='+client_id+'&redirect_uri'+redirect_uri
    photos_dict=requests.get(url).json()
    download_url='http://localhost:5000/download'
    delete_url='http://localhost:5000/delete_api'
    photo_ids=[]#这里有个小问题，如果没有图片，点lookup会出错误。。
    for k in photos_dict:
        photo_ids.append(k)
    return render_template('photos.html',photo_ids=photo_ids,download_url=download_url,delete_url=delete_url,photos_dict=photos_dict,token=token,premission=premission,client_id=client_id,redirect_uri=redirect_uri)
