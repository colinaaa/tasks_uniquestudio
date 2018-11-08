from flask import Flask, render_template
from config import dev_config
from flask_bootstrap import Bootstrap
from flask_sqlalchemy import SQLAlchemy

bootstrap=Bootstrap()
db=SQLAlchemy()

def create_app(config):
    app=Flask(__name__)
    app.config.from_object(dev_config)

    bootstrap.init_app(app)
    db.init_app(app)

    from .main import main as main_blueprint
    app.register_blueprint(main_blueprint)
    from .auth import auth as auth_blueprint
    app.register_blueprint(auth_blueprint)

    return app

