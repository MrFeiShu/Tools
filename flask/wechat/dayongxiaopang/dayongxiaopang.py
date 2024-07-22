from flask import Flask, request, jsonify, Response  
import json
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
        retStr = "获取释义失败。"

    print(retStr)

    return retStr

# 获取二十四史目录
# 读取 JSON 文件
def load_json_data(filename):
    with open("24book/" + filename, 'r') as file:
        data = json.load(file)
    return data

# 加载 JSON 数据
data = load_json_data('data.json')

@app.route("/catalogue",methods=['GET'])
def getCatalogue():
    print("enter getCatalogue")
    return jsonify(data)

@app.route("/book",methods=['GET'])
def getBook():
    book = request.args.get('book')  # 获取参数 book 的值
    chapter = request.args.get('chapter')  # 获取参数 book 的值

    file_path = '24book/'
    file_path += book
    file_path += '/'
    file_path += chapter
    file_path += '.txt'

    print(file_path)
  
    try:  
        # 以只读模式打开文件  
        with open(file_path, 'r', encoding='utf-8') as file:  
            # 读取文件全部内容  
            content = file.read()  
            # 返回内容，这里使用Response对象，你可以根据需要设置MIME类型  
            return Response(content, mimetype='text/plain')  
    except FileNotFoundError:  
        # 如果文件不存在，返回错误信息  
        return "File not found", 404

if __name__ == "__main__":
    app.run(host='0.0.0.0',port =8080)