# -*- coding: utf-8 -*-

from scrapy import cmdline


#--nolog是以不显示日志的形式运行，如果需要看详细信息，可以去掉
cmdline.execute("scrapy crawl cnblog --nolog".split())
