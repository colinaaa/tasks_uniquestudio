import unittest
from app import create_app, db
from app.models import User, Question, Answer


class ModelTestCase(unittest.TestCase):
    def setUp(self):
        self.app = create_app('testing')
        self.app_context = self.app.app_context()
        self.app_context.push()
        db.create_all()

    def tearDown(self):
        db.session.remove()
        db.drop_all()
        self.app_context.pop()

    def test_password_setter(self):
        u = User(password='test')
        self.assertTrue(u.password_hash is not None)

    def test_password_getter(self):
        u = User(password='test')
        with self.assertRaises(AttributeError):
            u.password

    def test_password_vertify(self):
        u = User(password='test')
        self.assertTrue(u.verify_password('test'))
        self.assertFalse(u.verify_password('not test'))

    def test_salt_random(self):
        u1 = User(password='test')
        u2 = User(password='test')
        self.assertTrue(u1.password_hash != u2.password_hash)

    def test_token_generate(self):
        u = User()
        db.session.add(u)
        db.session.commit()
        self.assertTrue(u.generate_auth_token() != None)

    def test_verify_token(self):
        u = User()
        db.session.add(u)
        db.session.commit()
        token = u.generate_auth_token()
        self.assertTrue(u == User.verify_token(token))

    def test_to_json(self):
        user_expected_keys = [
            'url', 'username', 'email', 'school', 'gps', 'questions', 'answers'
        ]
        question_expected_keys = [
            'url', 'body', 'author_url', 'answer_num', 'answer_url'
        ]

        u = User(username='test')
        q = Question(body='test', author_id=1)
        a = Answer(author_id=1, question_id=1)

        db.session.add(u)
        db.session.add(q)
        db.session.add(a)
        db.session.commit()
        with self.app.test_request_context('/'):
            self.assertEqual(
                sorted(user_expected_keys), sorted(u.to_json().keys()))
            self.assertEqual('/api/users/' + str(u.id), u.to_json()['url'])
            self.assertTrue(u.to_json()['answers'] != None)
            self.assertTrue(u.to_json()['questions'] != None)
