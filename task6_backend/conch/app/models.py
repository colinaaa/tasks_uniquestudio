from . import db


class User(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.Unicode(128), unique=True, index=True)
    email = db.Column(db.String(128), unique=True)
    school = db.Column(db.Unicode(128))
    gps = db.Column(db.UnicodeText, nullable=True)
    questions = db.relationship('Question', backref='author')
    answers = db.relationship('Answer', backref='author')

    def __repr__(self):
        return '<User %r>' % self.username


class Question(db.Model):
    __tablename__ = 'questions'
    id = db.Column(db.Integer, primary_key=True)
    body = db.Column(db.UnicodeText)
    author_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    answers = db.relationship('Answer', backref='question', lazy='dynamic')

    def __repr__(self):
        return '<Question %d by %d>' % self.id, self.author_id


class Answer(db.Model):
    __tablename__ = 'answers'
    id = db.Column(db.Integer, primary_key=True)
    body = db.Column(db.UnicodeText)
    author_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    question_id = db.Column(db.Integer, db.ForeignKey('questions.id'))

    def __repr__(self):
        return '<Answer of question %d by %d>' % self.question_id, self.author_id
