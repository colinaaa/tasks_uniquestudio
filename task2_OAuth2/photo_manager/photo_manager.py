from app import create_app, db
from app.models import User,Photo

app=create_app('dev')

@app.shell_context_processor
def make_shell_context():
    return dict(db=db,User=User,Photo=Photo)
