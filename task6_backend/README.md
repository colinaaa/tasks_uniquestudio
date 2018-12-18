
<a id="markdown-目录" name="目录"></a>
# 目录

- [接口](#接口)
  - [主页](#主页)
  - [注册](#注册)
  - [获得token](#获得token)
  - [获得用户信息](＃获得用户信息)
  - [获得问题信息](＃获得问题信息)
  - [获得回答信息](＃获得回答信息)
  - [提交问题](＃提交问题)
  - [提交回答](＃提交回答)
- [运行](#运行)
  - [develop](#develop)
  - [product](#product)
  

<a id="markdown-接口" name="接口"></a>
# 接口 #
<a id="markdown-主页" name="主页"></a>
## 主页 ##
<a id="markdown-url" name="url"></a>
### url ###
`/`

<a id="markdown-请求方法" name="请求方法"></a>
### 请求方法
`GET`

<a id="markdown-请求参数" name="请求参数"></a>
### 请求参数
无

<a id="markdown-返回值" name="返回值"></a>
### 返回值
| json字段 | 类型  | 备注               |
| -------- | ----- | ------------------ |
| question | array | 所以问题对应的json |

<a id="markdown-example" name="example"></a>
### example
```json
{
  "questions": [
    {
      "answer_num": 3, 
      "answer_url": "/api/answers/1", 
      "author_url": "/api/users/2", 
      "body": "test question", 
      "url": "/api/questions/1"
    }, 
    {
      "answer_num": 0, 
      "answer_url": "no answer", 
      "author_url": "/api/users/2", 
      "body": "test question", 
      "url": "/api/questions/2"
    }
  ]
}
```

<a id="markdown-注册" name="注册"></a>
## 注册
<a id="markdown-url-1" name="url-1"></a>
### url
`/register`
<a id="markdown-请求方法-1" name="请求方法-1"></a>
### 请求方法
`POST`
<a id="markdown-请求参数-1" name="请求参数-1"></a>
### 请求参数
| 字段名   | 类型   | 备注   |
| -------- | ------ | ------ |
| username | string | 必须   |
| password | base64 | 必须   |
| email    | string | 必须   |
| gps      | text   | 非必须 |
| school   | string | 非必须 |

<a id="markdown-返回值-1" name="返回值-1"></a>
### 返回值
一个有user信息的json

| json字段  | 类型      | 备注               |
| --------- | --------- | ------------------ |
| url       | string    | 注册用户对应url    |
| username  | string    | /                  |
| email     | string    | /                  |
| school    | string    | /                  |
| gps       | string    | /                  |
| questions | more json | 如果没有，会返回"" |
| answers   | more json | 同上               |

<a id="markdown-example-1" name="example-1"></a>
### example
```json
{
    "answers": [],
    "email": "123@qq.com",
    "gps": "xxx",
    "questions": [],
    "school": "hust",
    "url": "/api/users/2",
    "username": "colin"
}
```

<a id="markdown-获得token" name="获得token"></a>
## 获得token
<a id="markdown-url-2" name="url-2"></a>
### url
`/api/tokens/`
<a id="markdown-请求方法-2" name="请求方法-2"></a>
### 请求方法
`POST`
<a id="markdown-请求参数-2" name="请求参数-2"></a>
### 请求参数
请求中添加header: `Authorization: Basic <email>:<password>`
<a id="markdown-返回值-2" name="返回值-2"></a>
### 返回值

| json字段 | 类型   | 备注                     |
| -------- | ------ | ------------------------ |
| token    | string | 可储存下来以后POST时使用 |

<a id="markdown-example-2" name="example-2"></a>
### example
```json
{
    "token": "eyJhbG...xwLog"
}
```

<a id="markdown-获得用户信息" name="获得用户信息"></a>
## 获得用户信息
<a id="markdown-url-3" name="url-3"></a>
### url
`/api/users/<id>`
<a id="markdown-请求方法-3" name="请求方法-3"></a>
### 请求方法
`GET`
<a id="markdown-请求参数-3" name="请求参数-3"></a>
### 请求参数
`id`，在URL中
<a id="markdown-返回值-3" name="返回值-3"></a>
### 返回值
一个有user信息的json

| json字段  | 类型       | 备注               |
| --------- | ---------- | ------------------ |
| url       | string     | 注册用户对应url    |
| username  | string     | /                  |
| email     | string     | /                  |
| school    | string     | /                  |
| gps       | string     | /                  |
| questions | json array | 如果没有，会返回"" |
| answers   | json array | 同上               |

<a id="markdown-example-3" name="example-3"></a>
### example
```json
{
  "answers": [
    {
      "author_url": "/api/users/2", 
      "body": "test answer", 
      "next_answer_id": 2, 
      "question_url": "/api/questions/1", 
      "url": "/api/answers/1"
    }, 
    {
      "author_url": "/api/users/2", 
      "body": "test answer", 
      "next_answer_id": 3, 
      "question_url": "/api/questions/1", 
      "url": "/api/answers/2"
    }, 
    {
      "author_url": "/api/users/2", 
      "body": "test answer", 
      "next_answer_id": "no more answer", 
      "question_url": "/api/questions/1", 
      "url": "/api/answers/3"
    }
  ], 
  "email": "123@qq.com", 
  "gps": "xxx", 
  "questions": [
    {
      "answer_num": 3, 
      "answer_url": "/api/answers/1", 
      "author_url": "/api/users/2", 
      "body": "test question", 
      "url": "/api/questions/1"
    }, 
    {
      "answer_num": 0, 
      "answer_url": "no answer", 
      "author_url": "/api/users/2", 
      "body": "test question", 
      "url": "/api/questions/2"
    }
  ], 
  "school": "hust", 
  "url": "/api/users/2", 
  "username": "colin"
}
```

<a id="markdown-获得问题信息" name="获得问题信息"></a>
## 获得问题信息
<a id="markdown-url-4" name="url-4"></a>
### url
`/api/questions/<id>`
<a id="markdown-请求方法-4" name="请求方法-4"></a>
### 请求方法
`GET`
<a id="markdown-请求参数-4" name="请求参数-4"></a>
### 请求参数
`id`，在URL中
<a id="markdown-返回值-4" name="返回值-4"></a>
### 返回值
一个含有问题信息的json

| json字段   | 类型   | 备注                        |
| ---------- | ------ | --------------------------- |
| url        | string | 当前问题URL                 |
| body       | string | 问题内容                    |
| author_url | string | 提问者URL                   |
| answer_num | number | 问题数量                    |
| answer_url | string | 若没有回答则显示"no answer" |

<a id="markdown-example-4" name="example-4"></a>
### example
```json
{
  "answer_num": 3, 
  "answer_url": "/api/answers/1", 
  "author_url": "/api/users/2", 
  "body": "test question", 
  "url": "/api/questions/1"
}
```

<a id="markdown-获得回答信息" name="获得回答信息"></a>
## 获得回答信息
<a id="markdown-url-5" name="url-5"></a>
### url
`/api/answers/<id>`
<a id="markdown-请求参数-5" name="请求参数-5"></a>
### 请求参数
`id`，在URL中
<a id="markdown-返回值-5" name="返回值-5"></a>
### 返回值
一个含有回答信息的json

| json字段       | 类型      | 备注                     |
| -------------- | --------- | ------------------------ |
| url            | string    | 当前回答URL              |
| body           | string    | 回答内容                 |
| author_url     | string    | 回答者URL                |
| question_url   | string    | 回答URL                  |
| next_answer_id | string/id | 没有显示"no more answer" |


<a id="markdown-example-5" name="example-5"></a>
### example
```json
{
  "author_url": "/api/users/2", 
  "body": "test answer", 
  "next_answer_id": 2, 
  "question_url": "/api/questions/1", 
  "url": "/api/answers/1"
}
```

<a id="markdown-提交问题" name="提交问题"></a>
## 提交问题
<a id="markdown-url-6" name="url-6"></a>
### url
`/api/questions/`
<a id="markdown-请求方法-5" name="请求方法-5"></a>
### 请求方法
`POST`
<a id="markdown-请求参数-6" name="请求参数-6"></a>
### 请求参数
- 验证token： 在请求中添加header：`Authorization: Token <token>`（用户在后台通过token来确定）
- body：json字段`"body":"<content>"`

ps：只能通过token来post（不能用basic）
<a id="markdown-返回值-6" name="返回值-6"></a>
### 返回值
一个含有问题信息的json

| json字段   | 类型   | 备注                        |
| ---------- | ------ | --------------------------- |
| url        | string | 当前问题URL                 |
| body       | string | 问题内容                    |
| author_url | string | 提问者URL                   |
| answer_num | number | 问题数量                    |
| answer_url | string | 若没有回答则显示"no answer" |

<a id="markdown-example-6" name="example-6"></a>
### example
```json
{
    "answer_num": 0,
    "answer_url": "no answer",
    "author_url": "/api/users/2",
    "body": "test question",
    "url": "/api/questions/2"
}
```

<a id="markdown-提交回答" name="提交回答"></a>
## 提交回答
<a id="markdown-url-7" name="url-7"></a>
### url
`/api/answers/`
<a id="markdown-请求类型" name="请求类型"></a>
### 请求类型
`POST`
<a id="markdown-请求参数-7" name="请求参数-7"></a>
### 请求参数
- 验证token： 在请求中添加header：`Authorization: Token <token>`（用户在后台通过token来确定）
- body：json字段`"body":"<content>"`

ps：也只能通过token来post
<a id="markdown-返回值-7" name="返回值-7"></a>
### 返回值
一个含有回答信息的json

| json字段       | 类型      | 备注                     |
| -------------- | --------- | ------------------------ |
| url            | string    | 当前回答URL              |
| body           | string    | 回答内容                 |
| author_url     | string    | 回答者URL                |
| question_url   | string    | 回答URL                  |
| next_answer_id | string/id | 没有显示"no more answer" |

<a id="markdown-example-7" name="example-7"></a>
### example
```json
{
    "author_url": "/api/users/2",
    "body": "test answer",
    "next_answer_id": "no more answer",
    "question_url": "/api/questions/1",
    "url": "/api/answers/3"
}
```

<a id="markdown-how-to-sign-in" name="how-to-sign-in"></a>
# how to sign in #
**using http basic authentication**
<a id="markdown-with-password-" name="with-password-"></a>
## with password ##
add a header when request: `Authorization: Basic <email>:<password>`
<a id="markdown-with-token" name="with-token"></a>
## with token ##
1. firstly, make `POST` request to `/api/tokens/ (using basic authentication)`
2. then, got the token and store it
3. finally, next time you can switch the header to `Authorization: Token <token>` when making requests


<a id="markdown-运行" name="运行"></a>
# 运行 #

<a id="markdown-develop" name="develop"></a>
## develop ##

在开发环境中使用虚拟环境中的开发服务器测试

<a id="markdown-Linux&macOs" name="Linux&macOs"></a>
### Linux&macOs

运行目录下的`start.sh`脚本即可

<a id="markdown-Windows" name="Windows"></a>
### Windows

1. 首先在命令行输入`venv\scripts\activate`
2. 在进入虚拟环境后，输入以下命令设置环境变量：
```sh
cd conch
set FLASK_APP=conch.py
set FLASK_ENV=development
```
3. 最后启动服务器
```py
flask run
```
<a id="markdown-product" name="product"></a>
## product ##

准备用`docker`，不知道有没有时间。。