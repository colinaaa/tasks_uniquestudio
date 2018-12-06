import requests
import re
from lxml import etree

base_url = 'https://book.douban.com/top250?start='


def download(start):
    response = requests.get(base_url+str(start))
    return response.text


def parse(raw):
    # titles and english names(with \n)
    html = etree.HTML(raw)
    titles = html.xpath(
        '//div[@class="pl2"]/a/text() | //div[@class="pl2"]/span/text()')
    for t in titles:
        t = t.lstrip().rstrip()
        print(t)
    # TODO using database
    # basic information(with \n)
    infos = html.xpath('//td[@valign="top"]/p/text()')
    print(infos[::3])
    # TODO using database
    # evaluations
    evaluations = html.xpath('//div[@class="star clearfix"]/span/text()')
    res = []
    for e in evaluations:
        e = e.replace(' ', '')
        e = e.replace('\n', '')
        res.append(e)
    print(res)
    # TODO using database
    # quote
    quotes = html.xpath('//p[@class="quote"]/span/text()')
    print(quotes)
    # TODO using database


def main():
    for i in range(0, 226, 25):
        raw = download(i)
        parse(raw)


if __name__ == "__main__":
    main()
