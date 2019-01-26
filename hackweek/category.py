from lxml import etree
import requests
import os
import download


def first_class():
    raw = requests.get("https://www.douguo.com/caipu/fenlei").text
    html = etree.HTML(raw)
    items = html.xpath('//ul[@class="sortlist clearfix"]/li/a')
    titles = []
    hrefs = []
    for i in items:
        titles += i.xpath('text()')
        hrefs += (i.xpath('@href'))
    for i in range(0, len(titles)):
        title = titles[i]
        print(title)
        # if not os.path.exists(title):
        # os.mkdir(title)
        # print("start downloading category {}".format(title))
        # download.one_category(title, hrefs[i])


if __name__ == "__main__":
    first_class()
