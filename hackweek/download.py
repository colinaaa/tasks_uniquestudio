import requests
from lxml import etree
import re
import os
base_url = "https://www.douguo.com"


def one_category(title, href):
    url = base_url+href
    raw = requests.get(url).text
    html = etree.HTML(raw)
    try:
        last = html.xpath('//a[@class="alast"]/@href')[0]
        max_num = re.findall('/(\d+)$', last)[0]
    except:
        max_num = 0
    for p in range(0, int(max_num)+1, 24):
        print("  downloading page {}".format(str(p)))
        one_page(title, href+'/'+str(p))


def one_page(title, href):
    raw = requests.get(base_url+href).text
    html = etree.HTML(raw)
    imgs = html.xpath('//li[@class="item"]/a/img')
    for i in imgs:
        src = i.xpath('@src')[0]
        name = i.xpath('@alt')[0]
        try:
            if '/' in name:
                name.replace('/', '_')
        except:
            continue
        save(src, name, title)


def save(src, name, title):
    try:
        resp = requests.get(src)
        photo_data = resp.content
    except:
        print("save wrong")
    try:
        with open(title+'/{}.jpg'.format(name), 'wb') as photo_file:
            photo_file.write(photo_data)
            photo_file.close()
    except:
        pass
