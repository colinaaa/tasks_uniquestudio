from . import api
from flask import jsonify
from ..exceptions import ValidationError, MimetypeError


def not_found(message):
    response = jsonify({'error': 'not found', 'message': message})
    response.status_code = 404
    return response


def unauthorized(message):
    response = jsonify({'error': 'unauthorized', 'message': message})
    response.status_code = 401
    return response


def bad_request(message):
    response = jsonify({'error': 'bad request', 'message': message})
    response.status_code = 400
    return response


@api.app_errorhandler(ValidationError)
def validation_error(e):
    return bad_request(e.args[0])
