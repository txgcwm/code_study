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

