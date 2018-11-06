from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, PasswordField

class Login(FlaskForm):
    username=StringField('username:')
    password=PasswordField('password:')
    submit=SubmitField('Log in')

class Signup(FlaskForm):
    username=StringField('username:')
    password=PasswordField('password:')
    submit=SubmitField('Sign up')
