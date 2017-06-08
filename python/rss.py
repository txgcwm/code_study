from nntplib import NNTP
from time import strftime,time,localtime
from email import message_from_string
from urllib import urlopen
import textwrap
import re

day = 24*60*60

def wrap(string,max=70):
    '''
    '''
    return '\n'.join(textwrap.wrap(string)) + '\n'

class NewsAgent:
    '''
    '''
    def __init__(self):
        self.sources = []
        self.destinations = []

    def addSource(self,source):
        self.sources.append(source)

    def addDestination(self,dest):
        self.destinations.append(dest)

    def distribute(self):
        items = []
        for source in self.sources:
            items.extend(source.getItems())
        for dest in self.destinations:
            dest.receiveItems(items)

class NewsItem:
    def __init__(self,title,body):
        self.title = title
        self.body = body

class NNTPSource:
    def __init__(self,servername,group,window):
        self.servername = servername
        self.group = group
        self.window = window

    def getItems(self):
        start = localtime(time() - self.window*day)
        date = strftime('%y%m%d',start)
        hour = strftime('%H%M%S',start)

        server = NNTP(self.servername)

        ids = server.newnews(self.group,date,hour)[1]

        for id in ids:
            lines = server.article(id)[3]
            message = message_from_string('\n'.join(lines))

            title = message['subject']
            body = message.get_payload()
            if message.is_multipart():
                    body = body[0]

            yield NewsItem(title,body)

        server.quit()

class SimpleWebSource:

    def __init__(self,url,titlePattern,bodyPattern):
        self.url = url
        self.titlePattern = re.compile(titlePattern)
        self.bodyPattern = re.compile(bodyPattern)

    def getItems(self):
        text = urlopen(self.url).read()
        titles = self.titlePattern.findall(text)
        bodies = self.bodyPattern.findall(text)
        for title.body in zip(titles,bodies):
            yield NewsItem(title,wrap(body))

class PlainDestination:

    def receiveItems(self,items):
        for item in items:
            print item.title
            print '-'*len(item.title)
            print item.body

class HTMLDestination:

    def __init__(self,filename):
        self.filename = filename

    def receiveItems(self,items):
        out = open(self.filename,'w')
        print >> out,'''
        <html>
        <head>
         <title>Today's News</title>
        </head>
        <body>
        <h1>Today's News</hi>
        '''

        print >> out, '<ul>'
        id = 0
        for item in items:
            id += 1
            print >> out, '<li><a href="#">%s</a></li>' % (id,item.title)
        print >> out, '</ul>'

        id = 0
        for item in items:
            id += 1
            print >> out, '<h2><a name="%i">%s</a></h2>' % (id,item.title)
            print >> out, '<pre>%s</pre>' % item.body

        print >> out, '''
        </body>
        </html>
        '''

def runDefaultSetup():

    agent = NewsAgent()

    bbc_url = 'http://news.bbc.co.uk/text_only.stm'
    bbc_title = r'(?s)a href="[^"]*">\s*<b>\s*(.*?)\s*</b>'
    bbc_body = r'(?s)</a>\s*<br/>\s*(.*?)\s*<'
    bbc = SimpleWebSource(bbc_url, bbc_title, bbc_body)

    agent.addSource(bbc)

    clpa_server = 'news2.neva.ru'
    clpa_group = 'alt.sex.telephone'
    clpa_window = 1
    clpa = NNTPSource(clpa_server,clpa_group,clpa_window)

    agent.addSource(clpa)

    agent.addDestination(PlainDestination())
    agent.addDestination(HTMLDestination('news.html'))

    agent.distribute()

if __name__ == '__main__':
    runDefaultSetup()