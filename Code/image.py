#coding=utf-8

#urllib模块提供了读取Web页面数据的接口
import urllib.request
#re模块主要包含了正则表达式
import re
import requests
import urllib.error
import BitVector.BitVector
import socket
#如果连接时间超出2秒就跳过这个网址
socket.setdefaulttimeout(2.0)

urls = ["http://www.ivsky.com/"]  # 自定义要爬去的链接
depth = 5  # 自定义爬去的深度
picNum = 0

class MyCrawler:
    def __init__(self, seeds):
        # 初始化当前抓取的深度
        self.currentDepth = 1
        # 使用种子初始化url栈
        self.linkQuence = linkQuence()
        self.bloomfilter = BloomFilter()
        if isinstance(seeds, str):
            self.linkQuence.addUnvisitedUrl(seeds)
        if isinstance(seeds, list):
            for i in seeds:
                self.linkQuence.addUnvisitedUrl(i)


    # 抓取过程主函数
    def crawling(self, seeds, crawlDepth, static_url):
        # 循环条件：抓取深度不超过crawl_deepth
        while self.currentDepth <= crawlDepth:
            # 循环条件：待抓取的链接不空
            print("depth",self.currentDepth)
            while len(self.linkQuence.visted) <= 1023 and not self.linkQuence.unVisitedUrlsEnmpy():
                try:
                    # url出栈
                    visitUrl = self.linkQuence.unVisitedUrlDeQuence()
                    if visitUrl is None or visitUrl == "":
                        continue
                    elif (self.bloomfilter.isContaions(visitUrl) == True):
                        continue
                    self.bloomfilter.insert(visitUrl)
                    # 获取超链接
                    links = self.getHyperLinks(visitUrl, static_url)
                    # 将url放入已访问的url中
                    self.linkQuence.addVisitedUrl(visitUrl)
                    #print(len(self.linkQuence.visted))
                    # 未访问的url入列
                    for link in links:
                        if ("http" == str(i)[:4]):
                            self.linkQuence.addUnvisitedUrl(link)
                except:
                    continue
            self.currentDepth += 1
        print("urlTotal:",len(self.linkQuence.visted))




    # 获取源码中得超链接
    def getHyperLinks(self, url, static_url):
        result = []
        r = requests.get(url,allow_redirects=False)
        data = r.text
        #print(url)
        # 利用正则查找所有连接
        link_list = re.findall(r"(?<=href=\").+?(?=\")|(?<=href=\').+?(?=\')", data)
        for i in link_list:
            if ("http" == str(i)[:4]):
                result.append(i)
        return result

class linkQuence:
    def __init__(self):
        # 已访问的url集合
        self.visted = []
        # 待访问的url集合
        self.unVisited = []

    # 获取访问过的url栈
    def getVisitedUrl(self):
        return self.visted

    # 获取未访问的url栈
    def getUnvisitedUrl(self):
        return self.unVisited

    # 添加到访问过得url栈中
    def addVisitedUrl(self, url):
        self.visted.append(url)

    # 移除访问过得url
    def removeVisitedUrl(self, url):
        self.visted.remove(url)


    # 未访问过得url出栈
    def unVisitedUrlDeQuence(self):
        try:
            return self.unVisited.pop()
        except:
            return None

    # 保证每个url只被访问一次
    def addUnvisitedUrl(self, url):
        if url != "" and url not in self.visted and url not in self.unVisited:
            self.unVisited.insert(0, url)

    # 获得已访问的url数目
    def getVisitedUrlCount(self):
        return len(self.visted)

    # 获得未访问的url数目
    def getUnvistedUrlCount(self):
        return len(self.unVisited)

    # 判断未访问的url栈是否为空
    def unVisitedUrlsEnmpy(self):
        return len(self.unVisited) == 0


class SimpleHash():
    def __init__(self, capability, seed):
        self.capability = capability
        self.seed = seed

    # 传入的value即为url值，ord(value[i])表示第i位字符的ascii码值
    def hash(self, value):
        ret = 0
        for i in range(len(value)):
            ret += self.seed * ret + ord(value[i])
            # 最终产生的数是二进制向量最大下标与随机数的按位与结果
        return (self.capability - 1) & ret


class BloomFilter():
    #n = 1200, m = 15000, k = 7
    def __init__(self, BIT_SIZE=15000):
        self.BIT_SIZE = 15000
        self.seeds = [5, 7, 11, 13, 31, 37, 61]
        # 建立一个大小为15000位的二进制向量，分配内存
        self.bitset = BitVector.BitVector(size=self.BIT_SIZE)
        self.hashFunc = []
        # 利用7个素数初始化7个随机数生成器
        for i in range(len(self.seeds)):
            self.hashFunc.append(SimpleHash(self.BIT_SIZE, self.seeds[i]))

    def insert(self, value):
        for f in self.hashFunc:
            loc = f.hash(value)
            self.bitset[loc] = 1

    def isContaions(self, value):
        if value == None:
            return False
        ret = True
        for f in self.hashFunc:
            loc = f.hash(value)
            # 用同样的随机数产生方法对比相应位的二进制值，只要发现有一个不同即返回结果为假
            ret = ret & self.bitset[loc]
            if ret == False:
                return ret
                # 只有当7个二进制位都相等时才返回真
        return ret

    #定义一个getHtml()函数


def getHtml(url):
    global picNum
    try:
        headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:23.0) Gecko/20100101 Firefox/23.0'}
        url = urllib.request.Request(url=url, headers=headers)
        page = urllib.request.urlopen(url, timeout = 2)  #urllib.urlopen()方法用于打开一个URL地址
        html = page.read() #read()方法用于读取URL上的数据

        reg = r'src="([a-zA-Z0-9\.:/]+?\.jpg)"'  # 正则表达式，得到图片地址
        imgre = re.compile(reg)  # re.compile() 可以把正则表达式编译成一个正则表达式对象.
        html = html.decode("utf-8", "ignore")
        imglist = re.findall(imgre, html)  # re.findall() 方法读取html 中包含 imgre（正则表达式）的    数据
        # 把筛选的图片地址通过for循环遍历并保存到本地
        # 核心是urllib.urlretrieve()方法,直接将远程数据下载到本地，图片通过x依次递增命名
        for imgurl in imglist:
            try:
                if ("http" == str(imgurl)[:4]):
                    urllib.request.urlretrieve(imgurl, 'E:\images\%s.jpg' % picNum)
                    picNum += 1
            except:
                continue
    except:
        print("something is wrong")


def main(seeds, crawlDepth, static_url):

    craw = MyCrawler(seeds)
    craw.crawling(seeds, crawlDepth, static_url)

    for url in craw.linkQuence.visted:
        if(picNum <= 10032):
            getHtml(url)
        else:
            print("picTotal:",picNum)
            break



if __name__ == "__main__":
    for i in urls:
        main([i], depth, i)
    print("Done!")