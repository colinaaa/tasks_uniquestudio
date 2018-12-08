from selenium import webdriver
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.support.wait import WebDriverWait
import re
import getopt
import db
import sys
from urllib.parse import quote

base_url = 'https://search.jd.com/Search?keyword='


def download(item_name, page):
    browser = webdriver.Chrome()
    url = base_url + quote(item_name)+'&enc=utf-8&page='+str(page)
    try:
        browser.get(url)
        html = browser.page_source
    finally:
        browser.close()
    return html


def parse(html):
    items = re.findall('<div.*?gl-i-wrap.*?</li>', html, re.S)
    for i in items:
        price = re.search(r'<i>(\d+\.\d+)</i>', i, re.S)
        try:
            price = price.group(1)
        except AttributeError:
            continue
        promo_word = re.search('<a.*?_blank.*title="(.*?)"', i)
        try:
            promo_word = promo_word.group(1)
        except AttributeError:
            continue
        comment = re.search('comment.*>(.*)</a>', i)
        try:
            comment = comment.group(1)
        except AttributeError:
            continue
        shop = re.findall('<a.*_blank.*?onclick.*?href.*?title="(.*?)">', i)
        if shop == []:
            shop = ['第三方商家']
        data = [price, promo_word, comment+'条评论', shop[0]]
        print(data)
        columns = ['price', 'promo_word', 'comment', 'shop']
        db.insert(columns, data, 'jd')


def main(argv):
    try:
        opts = getopt.getopt(argv, "i:p:n:", ["item=", "page=", "number="])
    except getopt.GetoptError:
        print("usage: -i <item> -p <page> -n <number> --item <item> --page <page> --number <number>")
        sys.exit(2)
    args = ' '.join(opts[1])
    for opt, arg in opts[0]:
        if opt in('-i', '--item'):
            item = arg+' '+args
        if opt in('-p', '--page'):
            page = arg
        if opt in ('-n', '--number'):
            page = int(arg) % 30+1
    for p in range(1, int(page)*2, 2):
        html = download(item, int(p))
        parse(html)


if __name__ == "__main__":
    main(sys.argv[1:])
