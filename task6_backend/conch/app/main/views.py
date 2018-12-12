from . import main
from .. import db
from ..models import User
from flask import request, jsonify


@main.route('/register', methods=['POST'])
def register():
    if not request.is_json:
        raise MimetypeError('content-type not support')
    username = request.json.get('username')
    email = request.json.get('email')
    school = request.json.get('school')
    gps = request.json.get('gps')
    password = request.json.get('password')
    user = User(
        username=username,
        email=email,
        school=school,
        gps=gps,
        password=password,
    )
    db.session.add(user)
    db.session.commit()
    return jsonify(user.to_json())
