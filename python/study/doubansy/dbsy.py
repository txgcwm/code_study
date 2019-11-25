from urllib import request
from collections import deque
from pybloom_live import BloomFilter
from lxml import etree
import hashlib
 
class crawel_bfs:  
    request_header={
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
            'Accept-Encoding': 'br',
            'Accept-Language': 'zh-CN,zh;q=0.9',
            'Cache-Control': 'max-age=0',
            'Connection': 'keep-alive',
            'Cookie': 'bid=Kn9AT5duD7k; gr_user_id=32e433a7-19f0-4e17-80c4-56b58d7c0056; _vwo_uuid_v2=5985FEE132C29EC9C840D6C5EDD95323|67c2ccc8467fc02a9cce6928e86ea013; ll="118281"; __yadk_uid=I4Ki5RUaEWOUdpVIjJfRYo1MEuaW36hA; __utmv=30149280.16369; viewed="10483489_1115600_2230208_26857712_1569487_1623164_26708119_26677686"; __utma=30149280.965685249.1516632348.1528892392.1530880979.81; __utmc=30149280; __utmz=30149280.1530880979.81.57.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; __utmt=1; __utmb=30149280.1.10.1530880979; _pk_ref.100001.4cf6=%5B%22%22%2C%22%22%2C1530880982%2C%22https%3A%2F%2Fwww.douban.com%2F%22%5D; _pk_ses.100001.4cf6=*; __utma=223695111.2038558801.1520348154.1528892435.1530880982.55; __utmb=223695111.0.10.1530880982; __utmc=223695111; __utmz=223695111.1530880982.55.51.utmcsr=douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/; _pk_id.100001.4cf6=da4243a2a9e242f1.1520348154.54.1530881042.1528892472.',
            'Host': 'movie.douban.com',
            'Referer': 'https://www.douban.com/',
            'Upgrade-Insecure-Requests': '1',
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36'
            }
    
    cur_level=0
    max_level=2
        
    download_bf=BloomFilter(1024*1024,0.01)
    
    childque=deque()
    nowque=deque()
    
    def __init__(self,url,file_md5name,file_urlname):
        self.file_urlNameMd5_name=file_md5name
        self.file_urlName_name=file_urlname
        self.deal_file_md5=open(self.file_urlNameMd5_name,'r')
        self.file_md5=self.deal_file_md5.readlines()

        for url_md5_name in self.file_md5:
            self.download_bf.add(url_md5_name[:-1])
        self.deal_file_md5.close()
        self.file_md5=open(self.file_urlNameMd5_name,'a')     
        self.file_url=open(self.file_urlName_name,'a')
        self.nowque.append(url)
    
    def indeque(self,url):
        self.nowque.append(url)
        
    def outdeque(self):
        try:
            url=self.nowque.popleft()
            return url
        except Exception:
            self.cur_level+=1
            if self.cur_level==self.max_level:
                return None
            if len(self.childque)==0:
                return None
            self.nowque=self.childque
            self.childque=deque()
            return self.nowque.popleft()
        
    def crawler(self,url):
          try:
            req=request.Request(url,headers=self.request_header)
            response=request.urlopen(req)
            html_page=response.read()
            html=etree.HTML(html_page.lower().decode('utf-8'))
            url_list=html.xpath('//a/@href')
            for url in url_list:
                if url.find('javascript:')!=-1:
                    continue
                if url.startswith('http://') is False:
                    if url.startswith('/') is True:
                        url='http://movie.douban.com'+url
                    else:
                        continue
                if url[-1]=='/':
                    url=url[:-1]
                temp=hashlib.md5(url.encode('utf-8')).hexdigest()
                if temp not in self.download_bf:
                    self.download_bf.add(url)
                    self.childque.append(url)
                    self.file_md5.write(temp+'\n')
                    self.file_url.write(url+'\n')
          except Exception:
            print("error")
        
    def startcrawler(self):
        while True:
            url=self.outdeque()
            if url!=None:
                print("now cat"+url+"link")
                self.crawler(url)
            else:
                break
        self.file_md5.close()
        self.file_url.close()
        
crawel=crawel_bfs("https://movie.douban.com/",'urlmd5.txt', 'urlname.txt')
crawel.startcrawler()
