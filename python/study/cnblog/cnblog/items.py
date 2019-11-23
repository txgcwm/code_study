# -*- coding: utf-8 -*-

import scrapy


class CnblogItem(scrapy.Item):
    title = scrapy.Field()
    link  = scrapy.Field()

