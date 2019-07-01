from articlecrawler import ArticleCrawler

if __name__ == "__main__":
    Crawler = ArticleCrawler()
    Crawler.setCategory("IT과학", "정치")
    Crawler.setDateRange(2018, 2019, 4) # 2018년 1월부터 2019년 4월까지
    Crawler.start()