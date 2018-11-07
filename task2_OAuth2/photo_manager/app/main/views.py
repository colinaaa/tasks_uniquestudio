from flask import render_template, redirect, url_for, request
from . import main
from .forms import Login,Signup
from .. import db
from ..models import User, Photo
import os
import base64
#from flask_login import login_required, current_user

basedir=os.path.abspath(os.path.dirname(__file__))

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
            next=request.args.get('next')
            if next is not None:
                return redirect(next)
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
    photo_file=request.files['photo']
    pic_data=base64.urlsafe_b64encode(photo_file.read())
    if pic_data is not None:
        photo=Photo(data=pic_data,user_id=1)
        db.session.add(photo)
        db.session.commit()
    return redirect(url_for('.user',username='qingyu.wang@aliyun.com'))

@main.route('/download/<int:id>')
def download(id):
    photo=Photo.query.filter_by(id=id).first()
    pic_data=photo.data
    return render_template('photo.html',pic_data=pic_data)
