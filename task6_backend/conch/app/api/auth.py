from flask_httpauth import HTTPBasicAuth, HTTPTokenAuth, MultiAuth
from ..models import User
from . import api
from flask import g, current_app, jsonify
from itsdangerous import JSONWebSignatureSerializer as Serializer
basic_auth = HTTPBasicAuth()
token_auth = HTTPTokenAuth(scheme='Token')
multi_auth = MultiAuth(basic_auth, token_auth)


@basic_auth.verify_password
def verify_password(email, password):
    if email == '':
        return False
    user = User.query.filter_by(email=email).first()
    if not user:
        return False
    g.current_user = user
    g.token_used = False
    return user.verify_password(password)


@token_auth.verify_token
def verify_token(token):
    g.current_user = User.verify_token(token)
    g.token_used = True
    return g.current_user is not None


@api.before_request
@multi_auth.login_required
def before_request():
    pass


@api.route('/tokens/', methods=['POST'])
def get_token():
    if g.token_used:
        pass
    return jsonify({
        'token':
        g.current_user.generate_auth_token(expiration=3600),
        'expiration':
        3600
    })
