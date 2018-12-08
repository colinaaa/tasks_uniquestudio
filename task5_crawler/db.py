import pymysql
# from pymysql import cursors
import os

host = os.environ.get('DATABASE_HOST')
user = os.environ.get('DATABASE_USER')
passwd = os.environ.get('DATABASE_PASSWD')
# type()=str


def insert(columns, data, table):
    db = pymysql.connect(host=host,
                         port=3306,
                         user=user,
                         passwd=passwd,
                         db='crawlers',
                         charset='utf8mb4'
                         )
    sql = 'INSERT INTO {} ({}) VALUES ("{}")'.format(table,
                                                     ','.join(columns), '","'.join(data))
    try:
        with db.cursor() as cursor:
            cursor.execute(sql)
            cursor.close()
        db.commit()
    finally:
        db.close()


def update(column, data):
    pass
