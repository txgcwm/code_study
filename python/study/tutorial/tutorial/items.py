# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://docs.scrapy.org/en/latest/topics/items.html

import scrapy


class MaoyanItem(scrapy.Item):
	# 需要取哪些内容，就创建哪些容器
	index = scrapy.Field()
	title = scrapy.Field()
