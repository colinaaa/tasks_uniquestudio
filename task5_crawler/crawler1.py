import requests
import shutil
import getopt
import sys
import os
from lxml import etree

base_url = 'https://gratisography.com/'
data_folder = './crawler1_files/'


def download(page):
    response = requests.get(base_url+'page/'+str(page))
    return response.text


def parse(rawText):
    html = etree.HTML(rawText)
    result = html.xpath('//div/a/img/@src')
    return result


pics_saved = 0


def save(res, num):
    if os.path.exists(data_folder):
        shutil.rmtree(data_folder)
    os.mkdir(data_folder)
    for i in range(1, num+1):
        photo_data = requests.get(res[i-1]).content
        with open(data_folder+'{}.jpg'.format(str(i)), 'wb') as photo_file:
            photo_file.write(photo_data)


def main(argv):
    try:
        opts = getopt.getopt(argv, "p:n:", ["page=", "number="])
    except getopt.GetoptError:
        print("usage: -p <page> -n <number> --page <page> --number <number>")
        sys.exit(2)
    for opt, arg in opts[0]:
        n = int(arg)
        if opt in ('-p', '--page'):
            res = parse(download(1))
            for i in range(2, n+1):
                res += parse(download(i))
            save(res, 12*n)
        elif opt in ('-n', '--number'):
            page = n % 12+1
            res = parse(download(1))
            for i in range(2, page+1):
                res += parse(download(i))
                print(res, n)
            save(res, n)


if __name__ == "__main__":
    main(sys.argv[1:])
