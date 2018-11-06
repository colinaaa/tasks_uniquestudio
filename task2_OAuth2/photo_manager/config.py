import os

basedir=os.path.abspath(os.path.dirname(__file__))

class dev_config:
    SECRET_KEY='KEY'
    SQLALCHEMY_TRACK_MODIFICATIONS=False
    DEBUG=True
    SQLALCHEMY_DATABASE_URI='sqlite:///'+os.path.join(basedir,'data.sqlite')

    @staticmethod
    def init_app(app):
        pass
