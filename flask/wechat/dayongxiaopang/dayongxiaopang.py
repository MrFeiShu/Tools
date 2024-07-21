from flask import Flask, request
import requests
from bs4 import BeautifulSoup
from urllib.parse import quote

app = Flask(__name__)

@app.route("/")
def hello():
    return "hello world" 

@app.route("/shiyi",methods=['GET'])
def shiyi():
    # 请求网页
    url = 'https://hanyu.baidu.com/s?wd='  # 将此URL替换为你想要抓取的网页地址
    retStr = ""
    wd = request.args.get('wd')  # 获取参数 wd 的值
    print(wd)
    # URL 编码汉字
    url_encoded_char = quote(wd)
    print(url_encoded_char)

    url += url_encoded_char
    print(url)

    response = requests.get(url)

    # 检查请求是否成功
    if response.status_code == 200:
        print("log 1.")

        # 解析HTML
        soup = BeautifulSoup(response.text, 'html.parser')
        print("log 2.")
        # 使用CSS选择器来选取ID为basicmean-wrapper的div下的所有p标签
        p_tags = soup.select('#basicmean-wrapper p')
        print("log 3.")
        # 打印每个p标签的文本内容
        for p in p_tags:
            print(p.text.strip())
            retStr += p.text.strip()
    else:
        print(f"Failed to retrieve the webpage: {response.status_code}")

    print(retStr)

    return retStr

if __name__ == "__main__":
    app.run(host='0.0.0.0',port =8080)