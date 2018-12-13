from . import api
from flask import jsonify, request, g
from ..models import User, Answer, Question
from .. import db
from ..exceptions import ValidationError


@api.route('/')
def index():
    return 'hello'


# GET METHODS


@api.route('/users/<int:id>', methods=['GET'])
def get_user(id):
    user = User.query.get_or_404(id)
    return jsonify(user.to_json())


@api.route('/questions/<int:id>', methods=['GET'])
def get_question(id):
    question = Question.query.get_or_404(id)
    return jsonify(question.to_json())


# @api.route('/questions/<int:id>/answers/',methods=['POST'])
# def get_question_answer(id):


@api.route('/answers/<int:id>', methods=['GET'])
def get_answer(id):
    answer = Answer.query.get_or_404(id)
    return jsonify(answer.to_json())


#BUG FREE
# POST METHODS
@api.route('/questions/', methods=['POST'])
def new_question():
    if not request.is_json:
        raise ValidationError('content-type not support')
    body = request.json.get('body')
    if body is None or body == '':
        raise ValidationError('question has no body')
    question = Question(body=body, author_id=g.current_user.id)
    db.session.add(question)
    db.session.commit()
    return jsonify(question.to_json())


#BUG FREE
@api.route('/answers/', methods=['POST'])
def new_answer():
    body = request.json.get('body')
    question_id = request.json.get('question_id')
    if body is None or body == '':
        raise ValidationError('question has no body')
    answer = Answer(
        body=body,
        question_id=int(question_id),
        author_id=int(g.current_user.id))
    db.session.add(answer)
    db.session.commit()
    return jsonify(answer.to_json())
