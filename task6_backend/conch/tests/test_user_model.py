import unittest
from app import create_app, db
from app.models import User


class UserModelTestCase(unittest.TestCase):
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

    def test_token_random(self):
        u1 = User(id=2)
        u2 = User(id=1)
        self.assertTrue(
            u1.generate_auth_token(30) != u2.generate_auth_token(30))
