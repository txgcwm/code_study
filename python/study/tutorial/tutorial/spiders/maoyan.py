# -*- coding: utf-8 -*-
import scrapy
from tutorial.items import MaoyanItem


class MaoyanSpider(scrapy.Spider):
    # 爬虫的唯一标识，不能重复，启动爬虫的时候要用
	name = "maoyan"
	# 限定域名，只爬取该域名下的网页
	allowed_domains = ["maoyan.com"]
	# 开始爬取的链接
	start_urls = ["https://maoyan.com/board/7/"]

	def parse(self, response):
		dl = response.css('.board-wrapper dd')
		for dd in dl:
			item = MaoyanItem()
			item['index'] = dd.css('.board-index::text').extract_first()
			item['title'] = dd.css('.name a::text').extract_first()
			yield item
