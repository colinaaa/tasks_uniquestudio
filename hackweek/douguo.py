from lxml import etree
import requests
import os

base_url = "https://www.douguo.com/caipu/"


def category():
    raw = requests.get("https://www.douguo.com/caipu/fenlei").text
    html = etree.HTML(raw)
    categorys = html.xpath('//div[@id="content"]/h2/text()')
    for c in categorys:
        print(c)
        os.system("mkdir {}".format(c))


def main():
    category()


if __name__ == "__main__":
    main()
