from werkzeug.routing import Map, Rule 
from werkzeug.wrappers import Request , Response
from werkzeug.serving import run_simple
from werkzeug.exceptions import HTTPException
from werkzeug.utils import redirect
import os

class Colin:
    def __call__(self, environ, start_response):
        return self.wsgi_app(environ, start_response)

    def __init__(self):
        self.url_map = Map([
            Rule('/auth/confirm',endpoint='confirm'),
            Rule('/auth/token',endpoint='token')
            ])
        self.token=None

    def wsgi_app(self,environ, start_response):
        request=Request(environ)
        response=self.dispatch_request(request)
        return response(environ, start_response)

    def confirmed(self):
        return True  #用户确认流程,确认返回True

    def dispatch_request(self, request):
        adapter=self.url_map.bind_to_environ(request.environ)
        try:
            endpoint,values=adapter.match()
            return getattr(self,'on_'+endpoint)(request,**values)
        except HTTPException as e:
            return e
    def on_confirm(self,request):
        #省略用户确认操作
        if self.confirmed():
            params='code=test_auth_code'
            return Response(params)

    def on_token(self,request):
        #省略确认code操作
        return Response('token=test_token!')



if __name__=='__main__':
    app=Colin()
    run_simple('127.0.0.1',5060,app,use_reloader=True)