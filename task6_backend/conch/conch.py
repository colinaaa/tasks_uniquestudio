import os
import click
import sys
from app import create_app, db
from app.models import User, Answer, Question
from flask_migrate import Migrate

app = create_app(os.getenv("FLASK_CONFIG") or "default")
migrate = Migrate(app, db=db)
COV = None
if os.environ.get('FLASK_COVERAGE'):
    import coverage
    COV = coverage.coverage(branch=True, include='app/*')
    COV.start()


@app.shell_context_processor
def make_context():
    return dict(db=db, User=User, Answer=Answer, Question=Question)


@app.cli.command()
@click.option(
    '--coverage/--no-coverage',
    default=False,
    help='Run tests with code coverage')
def tests(coverage):
    """run the unit tests"""
    if coverage and not os.environ.get('FLASK_COVERAGE'):
        os.environ['FLASK_COVERAGE'] = '1'
        os.execvp(sys.executable, [sys.executable] + sys.argv)
    # codes above to avoid codes execute before the test
    import unittest
    tests = unittest.TestLoader().discover('tests')
    unittest.TextTestRunner(verbosity=2).run(tests)
    if COV:
        COV.stop()
        COV.save()
        print('Coverage Summary:')
        COV.report()
        COV.erase()
