from flask import render_template, redirect, url_for, request
from . import main
from .forms import Login,Signup
from .. import db
from ..models import User

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
