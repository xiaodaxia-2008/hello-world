import requests
from lxml import html
import re
import urllib
import hashlib
import random

# 在华为内部需要设置代理
PASSWORD = "moyan*1984"
USERNAME = "xiaozhen"
USERID = "x00428488"

proxy = f"{USERID}:{PASSWORD}@proxy.huawei.com:8080"
proxies = {"http": "http://" + proxy, "https": "http://" + proxy}


def translate(q, fromLang='en', toLang='zh'):
    # 百度翻译api
    appid = '20190122000258402'  # 你的appid
    secretKey = 'MXWAfCH5BYVIqUxpZ4aB'  # 你的密钥

    myurl = 'http://api.fanyi.baidu.com/api/trans/vip/translate'
    salt = random.randint(32768, 65536)

    sign = appid+q+str(salt)+secretKey
    m1 = hashlib.md5()
    m1.update(sign.encode('utf8'))
    sign = m1.hexdigest()
    myurl = myurl+'?appid='+appid+'&q=' + \
        urllib.parse.quote(q)+'&from='+fromLang+'&to='+toLang + \
        '&salt='+str(salt)+'&sign='+sign
    r = requests.get(myurl, proxies=proxies)
    rst = r.json()
    return "\n".join([d['dst'] for d in rst['trans_result']])


if __name__ == "__main__":
    url_ini = "https://www.nobelprize.org/prizes/lists/all-nobel-prizes-in-literature/"
    response_ini = requests.get(url_ini, proxies=proxies)

    file = open("./nobel/literature_bilang.txt", 'w', encoding='utf8')
    author_dict = {}
    et = html.fromstring(response_ini.content)
    urls = et.xpath("//div[@class='by_year']/p/a")
    for num, url in enumerate(urls):
        name = url.text
        print(f"{num}/{len(urls)}: {name}")
        url_detail = url.attrib.get('href')
        file.write(f"\n{name}: {url_detail}\n")
        detail_info = requests.get(url_detail, proxies=proxies)
        tree = html.fromstring(detail_info.content)
        msg = "\n".join([re.sub("[\t\n]*", "", e.text)
                         for e in tree.xpath("//div[@class='content']/p")[:-1] +
                         tree.xpath("//div[@class='description border-top']/p[2]")])
        file.write(msg)
        msg_trans = translate(msg)
        file.write("\n"+msg_trans+"\n")
    file.close()
