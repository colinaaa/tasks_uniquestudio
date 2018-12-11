from . import main


@main.route('/')
def index_main():
    return 'hello'
