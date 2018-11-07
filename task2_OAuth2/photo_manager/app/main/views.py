from flask import render_template, redirect, url_for, request,make_response
from . import main
from .forms import Login,Signup
from .. import db
from ..models import User, Photo
import os
import base64
#from flask_login import login_required, current_user

basedir=os.path.abspath(os.path.dirname(__file__))

@main.route('/')
def index():
    return render_template('index.html')

@main.route('/user/<username>',methods=['GET','POST'])
def user(username):
    return 'test'

@main.route('/login', methods=['GET','POST'])
def login():
    form=Login()
    username=None
    if form.validate_on_submit():
        username=form.username.data
        user=User.query.filter_by(username=username).first()
        if user is not None and form.password.data==user.password:
            redirect_uri=request.args.get('redirect_uri')
            if redirect_uri is not None:
                return redirect('auth?redirect_uri='+redirect_uri+'&correct=true')
            return redirect(url_for('.user',username=username))
        form.username.data=' '
    return render_template('login.html',form=form)

@main.route('/signup',methods=['GET','POST'])
def signup():
    form=Signup()
    if form.validate_on_submit():
        username=form.username.data
        password=form.password.data
        user=User(username=username,password=password)
        db.session.add(user)
        db.session.commit()
        return redirect(url_for('main.login'))
    return render_template('signup.html',form=form)

@main.route('/upload')
#@login_required
def upload():
    return render_template('up.html')


@main.route('/up',methods=['POST'])
#@login_required
def up():
    photo_file=request.files.get('photo')
    upload_path = os.path.join(basedir, 'static/images', photo_file.filename)
    photo=Photo(name=photo_file.filename,user_id=1,path=upload_path)
    db.session.add(photo)
    db.session.commit()
    photo_file.save(upload_path)
    return redirect(url_for('.user',username='qingyu.wang@aliyun.com'))#to be change

@main.route('/download/<int:id>')
def download(id):
    photo=Photo.query.filter_by(id=id).first()
    if photo is None:
        return 'no such photo'
    else:
        upload_path = os.path.join(basedir, 'static/images', photo.name)
        photo_file=open(upload_path,"rb").read()
        response = make_response(photo_file)
        response.headers['Content-Type'] = 'image/png'
        return response

@main.route('/delete/<int:id>')
def delete(id):
    photo=Photo.query.filter_by(id=id).first()
    db.session.delete(photo)
    db.session.commit()
    return redirect(url_for('.user',username='qingyu.wang@aliyun.com'))#to be change
