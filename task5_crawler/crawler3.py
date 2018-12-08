import re
import getopt
import sys
import requests
from urllib.parse import quote
import db

base_url = 'https://search.jd.com/Search?keyword='
# file = open('./1.html', 'r', encoding='utf-8')
# html = file.read()
# req = requests.get(
#     'https://search.jd.com/Search?keyword=手套&enc=utf-8')
# req.encoding = 'utf-8'
# print(req.text)
# html = req.text
# item = re.findall('<div.*?gl-i-wrap.*?</li>', html, re.S)
# # print(item[1])
# for i in item:
#     price = re.search('<i>(\d+\.\d+)</i>', i, re.S)
#     print(price.group(1))
#     promo_word = re.search('<a.*?_blank.*title="(.*?)"', i)
#     print(promo_word.group(1))
#     comment = re.search('comment.*>(.*)</a>', i)
#     print(comment.group(1)+'条评论')
#     shop = re.findall('<a.*_blank.*?onclick.*?href.*?title="(.*?)">', i)
#     if shop == []:
#         shop = ['第三方商家']
#     print(shop[0])


def download(item_name):
    url = base_url + quote(item_name)+'&enc=utf-8'
    headers = {
        'user-agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.80 Safari/537.36'}
    req = requests.get(url, headers=headers)
    req.encoding = 'utf-8'
    return req.text


def parse(html):
    items = re.findall('<div.*?gl-i-wrap.*?</li>', html, re.S)
    for i in items:
        price = re.search(r'<i>(\d+\.\d+)</i>', i, re.S)
        promo_word = re.search('<a.*?_blank.*title="(.*?)"', i)
        comment = re.search('comment.*>(.*)</a>', i)
        shop = re.findall('<a.*_blank.*?onclick.*?href.*?title="(.*?)">', i)
        if shop == []:
            shop = ['第三方商家']
        data = [price.group(1), promo_word.group(
            1), comment.group(1)+'条评论', shop[0]]
        columns = ['price', 'promo_word', 'comment', 'shop']
        db.insert(columns, data, 'jd')


def main(argv):
    try:
        opts = getopt.getopt(argv, "i:p:n:", ["item=", "page=", "number="])
    except getopt.GetoptError:
        print("usage: -i <item> -p <page> -n <number> --item <item> --page <page> --number <number>")
        sys.exit(2)
    for opt, arg in opts[0]:
        if opt == '-i':
            print(opt, arg)
            html = download(arg)
            parse(html)


if __name__ == "__main__":
    main(sys.argv[1:])
