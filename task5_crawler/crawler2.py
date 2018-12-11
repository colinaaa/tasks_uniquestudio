import requests
from lxml import etree
import db


base_url = 'https://book.douban.com/top250?start='


def download(start):
    response = requests.get(base_url+str(start))
    return response.text


rating = 1


def parse(raw):
    # titles and english names(with \n)
    html = etree.HTML(raw)
    tables = html.xpath('//td[@valign="top"]')
    for t in tables[1::2]:
        data = []
        data += (t.xpath('div[@class="pl2"]/a/@title'))
        foreign_name = (t.xpath('div[@class="pl2"]/span/text()'))
        if foreign_name != []:
            data += foreign_name
        else:
            data.append('无')
        info = t.xpath('p/text()')
        data.append(info[0])
        evaluation = t.xpath('div[@class="star clearfix"]/span/text()')
        data.append(' '.join(evaluation).replace(' ', '').replace('\n', ''))
        quote = t.xpath('p[@class="quote"]/span/text()')
        if quote == []:
            data.append('无')
        else:
            data += quote
        global rating
        data.append(str(rating))
        rating = rating+1
        columns = ['title', 'foreign_name',
                   'info', 'evaluation', 'quote', 'rate']
        db.insert(columns, data, 'book_douban')
        print(data)


def main():
    for i in range(0, 226, 25):
        raw = download(i)
        parse(raw)


if __name__ == "__main__":
    main()
