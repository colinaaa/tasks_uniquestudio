from flask import render_template, redirect, request, url_for, make_response, jsonify
from . import auth
from ..models import User,Photo
from .. import db
from ..main.views import basedir
import random
import os

auth_code={}
def generate_code(uri):
    code=random.randint(1,100)
    auth_code[code]=uri
    return code

def generate_token(client_id,premission):
    token=str(client_id)+'token'+str(premission)
    return token

premission=[]
auth_redir_uri=[]
users=[]
@auth.route('/oauth',methods=['GET','POST'])
def oauth():
    redirect_uri=request.args.get('redirect_uri')
    if redirect_uri is not None:
        auth_redir_uri.append(redirect_uri)
    if request.args.get('correct')=='true':#用户登录后生成code
        premission.append(request.args.get('premission'))
        uri='http://localhost:5050/passport?code={}'.format(generate_code(auth_redir_uri[0]))
        users.append(request.args.get('user'))
        return redirect(uri)
    if request.args.get('code')and request.args.get('grant_type')=='authorization':#验证code和redirect_uri的对应性
        if auth_code.get(int(request.args.get('code')))==auth_redir_uri[0]:
            token=generate_token(request.args.get('client_id'),premission[0])
            return redirect(auth_redir_uri[0]+'?token='+token+'&user='+users[0]+'&premission='+premission[0])
        #靠，permission都拼成premission了！！
        #费了很大劲把permission传给client结果发现token里其实有permission的信息。。。
    return redirect('login_api')

@auth.route('/login_api',methods=['GET','POST'])
def login_api():
    username=request.form.get('username')
    password=request.form.get('password')
    premission=request.form.getlist('premission')
    user=User.query.filter_by(username=username).first()
    if user is not None:
        if user.password==password:
            return redirect('http://localhost:5000/oauth?correct=true&user='+username+'&premission='+premission[0])
    return render_template('login_api.html')
#其实并没有使用渲染，只是懒得写地址了。。。

@auth.route('/upload_api/<username>',methods=['POST'])
def upload_api(username):
    photo_file=request.files.get('photo')
    client_id=request.args.get('client_id')
    token=request.args.get('token')
    premission_api=request.args.get('premission')
    redirect_uri=request.args.get('redirect_uri')
    if token==generate_token(client_id,'read'):
        response=make_response('no permission')
        response.status_code=403
        return response
    else:
        if token==generate_token(client_id,premission_api):#检验token
            user=User.query.filter_by(username=username).first()
            upload_path=os.path.join(basedir,'static/images',photo_file.filename)
            photo=Photo(name=photo_file.filename,user_id=user.id,path=upload_path)
            db.session.add(photo)
            db.session.commit()
            photo_file.save(upload_path)
            return redirect(redirect_uri)
        else:
            return 'bad token'

@auth.route('/info_api/<username>')
def info_api(username):
    user=User.query.filter_by(username=username).first()
    print(user)
    photos=[]
    photo_info={}
    for p in user.photos:
        photo_info[p.id]=p.name
    return jsonify(photo_info)

