# 如何成为第三方网站 #

- 首先，你需要有`client_id`，这个是用来申请`token`时用来身份验证的。
- 其次，你需要一个`uri`来作为成功认证后返回的地址，即`redirect_uri`。
- 再其次，你需要另一个`uri`来作为接收`code`并申请`token`的地址，即`passport_uri`。

## 申请token ##

你需要向我们的认证服务器发起`GET`请求，并附带四个参数`(response_type,client_id,redirect_uri,passport_uri)`。我们验证服务器的地址是：`localhost:5000/oauth`。

这时如果用户确认成功，我们会向`passport_uri`返回一个`code`，你应当接收`code`并用它再附上`redirect_uri和client_id`，来向我们验证服务器请求`token`。

如果`code`和`redirect_uri`与之前提供的相同的话，就能成功获取`token`。

## 图片信息 ##

我们开放了查看图片信息（有多少图片，图片的id和名字是什么）的接口，地址是`http://localhost:5000/info_api/<username>`，其中username是登录用户的用户名。

## 上传、下载和删除 ##

想要上传或删除，首先需要用户选择`edit`权限，然后你将请求发送至相应接口，并附上`token,client_id,permission,username,redirect_uri`这些参数即可，如果成功我们会返回至你提供的redirect_uri。
下载则需要所需下载图片的id和上述参数即可。

    upload_api:http://localhost:5000/upload_api
    delete_api:http://localhost:5000/delete_api
    download_api:http://localhost:5000/download