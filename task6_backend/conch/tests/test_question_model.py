from app.models import Question, User, Answer
import unittest
from app import create_app, db


class QuestionModelTestCase():
    def setUp(self):
        self.app = create_app('testing')
        self.app_context = self.app.app_context()
        self.app_context.push()
        db.create_all()

    def tearDown(self):
        self.app_context.pop()
        db.session.remove()
        db.drop_all()

    def test_to_json(self):
        expected_keys = [
            'url', 'body', 'author_url', 'answer_num', 'answer_url'
        ]
        q = Question(body='test', author_id=1)
        q2 = Question(author_id=1)
        db.session.add(q)
        db.session.add(q2)
        a = Answer(question_id=q2.id)
        db.session.add(a)
        db.session.commit()
        with self.app.test_request_context('/'):
            self.assertEqual(sorted(expected_keys), sorted(q.to_json()))
            self.assertEqual('no answer', q.to_json()['answer_url'])
            self.assertEqual('/api/answers/' + str(a.id),
                             q2.to_json()['answer_url'])
