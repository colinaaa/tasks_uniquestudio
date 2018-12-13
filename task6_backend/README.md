# interfaces #
## the return of each interface ##
| route                | method | return                                                               |
| -------------------- | :----: | :------------------------------------------------------------------- |
| /register            |  POST  | json with user info                                                  |
| /api/tokens/         |  POST  | json with token                                                      |
| /api/users/<:id>     |  GET   | json with user info by id                                            |
| /api/questions/<:id> |  GET   | json with url, body, author_url, answer_num and the first answer_url |
| /api/questions/      |  POST  | json of posted question                                              |
| /api/answers/<:id>   |  GET   | json with url, body, author_url, question_url and next_answer_id     |
| /api/answers/        |  POST  | json of posted answer                                                |
## the args each interface requires ##
| route                | method | args                                                |
| -------------------- | :----: | :-------------------------------------------------- |
| /register            |  POST  | username, password, email and optional[gps, school] |
| /api/tokens/         |  POST  | login required (only password)                      |
| /api/users/<:id>     |  GET   | the id of the user (in url) (login required)        |
| /api/questions/<:id> |  GET   | the id of the question (in url) (login required)    |
| /api/questions/      |  POST  | question in body field of json (login required)     |
| /api/answers/<:id>   |  GET   | the id of the answer (in url) (login required)      |
| /api/answers/        |  POST  | answer in body field of json (login required)       |
# how to sign in #
**using http basic authentication**
## with password ##
add a header when request: `Authorization: Basic <email>:<password>`
## with token ##
1. firstly, make `POST` request to `/api/tokens/ (using basic authentication)`
2. then, got the token and store it
3. finally, next time you can switch the header to `Authorization: Token <token>` when making requests

*the token doesn't have expiration for now*
# questions #
login in require ???
more interfaces like /api/questions/<:id>/answers/ ???