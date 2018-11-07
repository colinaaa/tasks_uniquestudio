from . import db

class User(db.Model):
    __tablename__='user'
    id=db.Column(db.Integer,primary_key=True)
    username=db.Column(db.String(128),index=True)
    password=db.Column(db.String(128))
    photos=db.relationship('Photo',backref='user')

class Photo(db.Model):
    __tablename__='photo'
    id=db.Column(db.Integer,primary_key=True)
    data=db.Column(db.Text)
    user_id=db.Column(db.Integer,db.ForeignKey('user.id'))
