首先命令行创建`project`，然后依次编写各项文件。

# item文件
```
# -*- coding: utf-8 -*-

import scrapy


class CnblogItem(scrapy.Item):
    title = scrapy.Field()
    link  = scrapy.Field()
```

# spider文件
```
# -*- coding: utf-8 -*-

import scrapy
from cnblog.items import CnblogItem


class CnblogSpider(scrapy.Spider):
	name = "cnblog"
	allowed_domains = ["cnblogs.com"]
	url = 'https://www.cnblogs.com/sitehome/p/'
	offset = 1
	start_urls = [url+str(offset)]

	def parse(self, response):
		item = CnblogItem()

		item['title'] = response.xpath('//a[@class="titlelnk"]/text()').extract()
		item['link'] = response.xpath('//a[@class="titlelnk"]/@href').extract()

		yield item

		print("第{0}页爬取完成".format(self.offset))

		if self.offset < 15:
			self.offset += 1

		url2 = self.url+str(self.offset)
		print(url2)
		yield scrapy.Request(url=url2, callback=self.parse)
```

# pipelines文件
```
class FilePipeline(object):
	def process_item(self, item, spider):
		data = ''

		with open('cnblog.txt', 'a', encoding='utf-8') as f:
			titles = item['title']
			links  = item['link']

			for i, j in zip(titles, links):
				data += i+'     '+j+'\n'

			f.write(data)
			f.close()

		return item
```

# 更改setting文件 
```
# Override the default request headers:
DEFAULT_REQUEST_HEADERS = {
   'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
   'Accept-Language': 'en',
    'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"
}

ITEM_PIPELINES = {
    'cnblog.pipelines.FilePipeline': 300,    #实现保存到txt文件
}
```

# 编写main文件

`scrapy`是不能在编译器里面调试的，但可以写一个主文件，运行这个主文件就可以像普通的工程一样在编译器里调试了。代码如下：
```
# -*- coding: utf-8 -*-

from scrapy import cmdline


#--nolog是以不显示日志的形式运行，如果需要看详细信息，可以去掉
cmdline.execute("scrapy crawl cnblog --nolog".split())
```

