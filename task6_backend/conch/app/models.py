import base64
from . import db
from flask import current_app, url_for
from itsdangerous import TimedJSONWebSignatureSerializer as Serializer
from werkzeug.security import generate_password_hash, check_password_hash


#TESTED
class User(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.Unicode(128), unique=True, index=True)
    password_hash = db.Column(db.String(128))
    email = db.Column(db.String(128), unique=True)
    school = db.Column(db.Unicode(128))
    gps = db.Column(db.UnicodeText, nullable=True)
    questions = db.relationship('Question', backref='author')
    answers = db.relationship('Answer', backref='author')

    def to_json(self):
        json_user = {
            'url': url_for('api.get_user', id=self.id),
            'username': self.username,
            'email': self.email,
            'school': self.school,
            'gps': self.gps,
            'questions': [question.to_json() for question in self.questions],
            'answers': [answer.to_json() for answer in self.answers],
        }
        return json_user

    @property
    def password(self):
        raise AttributeError('password not readable')

    @password.setter
    def password(self, password):
        self.password_hash = generate_password_hash(password)

    def verify_password(self, password):
        return check_password_hash(self.password_hash, password)

    def generate_auth_token(self, expiration):
        jws = Serializer(
            current_app.config['SECRET_KEY'],
            expires_in=expiration)  #expires_in=expiration
        print(jws.dumps({'id': self.id}))
        return jws.dumps({'id': self.id}).decode('utf-8')

    @staticmethod
    def verify_token(token):
        jws = Serializer(current_app.config['SECRET_KEY'])
        try:
            data = jws.loads(token)
        except:
            return None
        return User.query.get(data['id'])

    def __repr__(self):
        return '<User %r>' % self.username


class Question(db.Model):
    __tablename__ = 'questions'
    id = db.Column(db.Integer, primary_key=True)
    body = db.Column(db.UnicodeText)
    author_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    answers = db.relationship('Answer', backref='question', lazy='select')

    def to_json(self):
        answer_num = len(self.answers)
        json_question = {
            'url': url_for('api.get_question', id=self.id),
            'body': self.body,
            'author_url': url_for('api.get_user', id=self.author_id),
            'answer_num': answer_num,
        }
        #BUG free
        if answer_num > 0:
            json_question['answer_url'] = url_for(
                'api.get_answer', id=self.answers[0].id)
        else:
            json_question['answer_url'] = 'no answer'
        return json_question

    def __repr__(self):
        return '<Question %d by %d>' % (self.id, self.author_id)


class Answer(db.Model):
    __tablename__ = 'answers'
    id = db.Column(db.Integer, primary_key=True)
    body = db.Column(db.UnicodeText)
    author_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    question_id = db.Column(db.Integer, db.ForeignKey('questions.id'))

    def to_json(self):
        json_answer = {
            'url': url_for('api.get_answer', id=self.id),
            'author_url': url_for('api.get_user', id=self.author_id),
            'body': self.body,
            'question_url': url_for('api.get_question', id=self.question_id),
        }
        #BUG FREE
        answers = self.question.answers
        answers_id = []
        for val in answers:
            answers_id.append(val.id)
        index = answers_id.index(self.id)
        if len(answers) > index + 1:
            json_answer['next_answer_id'] = answers_id[index + 1]
        else:
            json_answer['next_answer_id'] = 'no more answer'  #or -1

        return json_answer

    def __repr__(self):
        return '<Answer of question %d by %d>' % (self.question_id,
                                                  self.author_id)
