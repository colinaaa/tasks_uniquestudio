#2:04
#2:21

from werkzeug.routing import Map, Rule 
from werkzeug.wrappers import Request , Response
from werkzeug.serving import run_simple
from werkzeug.exceptions import HTTPException
from werkzeug.utils import redirect
import os
import re

class Colin:

    def __call__(self, environ, start_response):
        return self.wsgi_app(environ,start_response)

    def __init__(self):
        self.url_map=Map([
            Rule('/login',endpoint='login'),
            Rule('/success',endpoint='success')
        ])
        self.token=None

    def wsgi_app(self,environ, start_response):
        request=Request(environ)
        response=self.dispatch_request(request)
        return response(environ, start_response)

    def dispatch_request(self, request):
        adapter=self.url_map.bind_to_environ(request.environ)
        try:
            endpoint,values=adapter.match()
            return getattr(self,'on_'+endpoint)(request,**values)
        except HTTPException as e:
            return e

    def on_login(self,request):
        url=request.url
        response_type= re.findall('response_type=(.*?)&',url)
        for i in response_type:
            response_type=i
        if response_type=='code':
            return redirect('http://127.0.0.1:5060/auth/confirm')
        
    def on_succsee(self,request):
        url=request.url
        token= re.findall('token=(.*?)!',url)
        for i in token:
            token=i
        if token=='test_token':
            return  Response('succeed')
        else: 
            return Response('failed')


if __name__=='__main__':
    app=Colin()
    run_simple('127.0.0.1',5050,app,use_reloader=True)



