import requests
import re
R_ser='127.0.0.1:5050'
A_ser='http://127.0.0.1:5060'
client_id='my_laptop'

def main():
    params1={
        'response_type':'code',
        'client_id':id
        }
    r1=requests.get(R_ser+'/login',params=params1)
    code=str(r1.response.text)
    params2={
        'grant_type':'authorization_code',
        'code':code,
        'client_id':id
    }
    r2=requests.get(A_ser+'/auth/token',params=params2)
    text=r2.response.text
    token= re.findall('token=(.*?)!',text)
    for i in token:
        token=i
    r3=requests.get(R_ser+'/success',params=token)
    return r3.reponse.text


if __name__=='__main__':
    main()



