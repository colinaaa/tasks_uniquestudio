from flask import render_template, redirect
from . import auth

@auth.routh('/auth')
def auth():
    return 'authorization server'
