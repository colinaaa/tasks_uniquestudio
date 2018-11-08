from flask import render_template, redirect, request, url_for
from . import auth
from ..models import User
import random

auth_code={}
def generate_code(uri):
    code=random.randint(1,100)
    auth_code[code]=uri
    return code

def generate_token(client_id):
    token=str(client_id)+'token'
    return token

auth_redir_uri=[]
user=[]
@auth.route('/auth',methods=['GET','POST'])
def auth():
    redirect_uri=request.args.get('redirect_uri')
    if redirect_uri is not None:
        auth_redir_uri.append(redirect_uri)
    if request.args.get('correct')=='true':#用户登录后生成code
        uri=auth_redir_uri[0]+'?code={}'.format(generate_code(auth_redir_uri[0]))
        user.append(request.args.get('user'))
        return redirect(uri)
    if request.args.get('code')and request.args.get('grant_type')=='authorization':#验证code和redirect_uri的对应性
        if auth_code.get(int(request.args.get('code')))==redirect_uri:
            token=generate_token(request.args.get('client_id'))
            return redirect(auth_redir_uri[1]+'?token='+token+'&user='+user[0])
    return redirect('login?redirect_uri='+auth_redir_uri[0])

