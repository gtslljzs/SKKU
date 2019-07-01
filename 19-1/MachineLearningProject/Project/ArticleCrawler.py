from time import sleep
from bs4 import BeautifulSoup as bs
from multiprocessing import Process
from Exceptions import *
from ArticleParser import ArticleParser
from SummaryParser import SummaryParser
import os
import calendar
import requests
import csv
import re
from selenium import webdriver

class ArticleCrawler(object):
    def __init__(self):
        self.parser = ArticleParser()
        self.summaryParser = SummaryParser()
        self.categories = {'정치': 100, '경제': 101, '사회': 102, '생활문화': 103, 'IT과학': 105,
                            'politics': 100, 'economy': 101, 'society': 102, 'living_curture': 103, 'IT_science': 105}
        self.selected_categories = []
        self.date = {'start_year': 0, 'start_month': 0, 'end_year': 0, 'end_month': 0}

    def setCategory(self, *args):
        for key in args:
            if self.categories.get(key) is None:
                raise InvalidCategory(key)
        
        self.selected_categories = args
    
    def setDateRange(self, start_year, start_month, end_year, end_month):
        args = [start_year, start_month, end_year, end_month]
        if start_year > end_year:
            raise InvalidYear(start_year, end_year)
        if start_month < 1 or start_month > 12:
            raise InvalidMonth(start_month)
        if end_month < 1 or end_month > 12:
            raise InvalidMonth(end_month)
        
        for key, date in zip(self.date, args):
            self.date[key] = date
        
        print(self.date)

    def makeNewsPageUrl(self, category_url, start_year, last_year, start_month, last_month):
        make_url = []
        final_startmonth = start_month
        final_lastmonth = last_month

        for year in range(start_year, last_year + 1):
            if year != last_year:
                start_month = start_month
                last_month = 12
            else:
                start_month = final_startmonth
                last_month = final_lastmonth
            
            for month in range(start_month, last_month + 1):
                for month_day in range(1, calendar.monthrange(year, month)[1] + 1):
                    url = category_url
                    
                    if len(str(month)) == 1:
                        month = "0" + str(month)
                    if len(str(month_day)) == 1:
                        month_day = "0" + str(month_day)
                    
                    url = url + str(year) + str(month) + str(month_day)
                    final_url = url # page 날짜 정보만 있고 page 정보가 없는 url 임시 저장
                    
                    # totalpage는 네이버 페이지 구조를 이용해서 page=1000으로 지정해 totalpage를 알아냄
                    # page=1000을 입력할 경우 페이지가 존재하지 않기 때문에 page=totalpage로 이동됨
                    totalpage = self.parser.findNewsTotalpage(final_url + "&page=1000")
                    for page in range(1, totalpage + 1):
                        url = final_url # url page 초기화
                        url = url + "&page=" + str(page)
                        make_url.append(url)
        return make_url

    def crawling(self, category_name):
        # 크롤링 횟수
        cnt = 1
        until = 4000

        # MultiThread PID
        print(category_name + " PID: " + str(os.getpid()))

        # 각 카테고리 기사를 저장할 CSV
        file = open('Articles_' + category_name + '.csv', 'w', encoding='euc_kr', newline='')
        wcsv = csv.writer(file)

        # 기사 URL 형식
        url = "http://news.naver.com/main/list.nhn?mode=LSD&mid=sec&sid1=" +\
                str(self.categories.get(category_name)) + "&date="
        # print(url)
        # start_year년 1월 ~ end_year의 end_month 날짜까지 기사를 수집합니다.
        final_urlday = self.makeNewsPageUrl(url, self.date['start_year'], self.date['end_year'],
                        self.date['start_month'], self.date['end_month'])
        print(category_name + " Urls are generated")
        print("The crawler starts")

        path = "G:/SKKU/19-1/MachineLearningProject/Project/chromedriver.exe"
        driver = webdriver.Chrome(path)
        
        for URL in final_urlday:
            # 크롤링 횟수를 채웠으면 종료
            if(cnt > until): break

            regex = re.compile("date=(\d+)")
            news_date = regex.findall(URL)[0]
            print(news_date)

            request = requests.get(URL)
            document = bs(request.content, 'html.parser')
            tag_documnet = document.find_all('dt', {'class': 'photo'})

            post = []
            for tag in tag_documnet:
                post.append(tag.a.get('href')) # 해당되는 page에서 모든 기사들의 URL을 post 리스트에 저장

            for content_url in post:
                # 크롤링 횟수를 채웠으면 종료
                if (cnt > until): break

                # 크롤링 대기
                # sleep(0.01)

                # 기사 HTML을 가져옴
                print(content_url)

                try:
                    print("*** driver.get ***")
                    driver.get(str(content_url))
                    driver.implicitly_wait(0.5)
                    print("*** find element ***")
                    driver.find_element_by_xpath("//a[@href='javascript:;']").click()
                    sleep(3)
                    print("*** page_source ***")
                    html = driver.page_source
                    print("*** html parser ***")
                    document_content = bs(html, 'html.parser')

                    # # 기사 제목을 가져옴
                    # print("*** Headline crawling ***")
                    # tag_headline = document_content.find_all('h3', {'id': 'articleTitle'}, {'class': 'tts_head'})
                    # text_headline = '' # 뉴스 기사 제목 초기화
                    # text_headline = text_headline + self.parser.clearHeadline(str(tag_headline[0].find_all(text=True)))
                    # if not text_headline: # 공백일 경우 기사 제외 처리
                    #     print("*** Error 02 ***")
                    #     continue
                    
                    # 기사 본문 가져옴
                    print("*** Article crawling ***")
                    tag_content = document_content.find_all('div', {'id': 'articleBodyContents'})
                    text_sentence = '' # 뉴스 기사 본문 초기화

                    for s in tag_content:
                        if (s != None):
                            for x in s('a'):
                                if (x != None):
                                    x.extract()
                            for x in s('script'):
                                if (x != None):
                                    x.extract()
                            for x in s('table'):
                                if (x != None):
                                    x.extract()
                            for x in s('strong'):
                                if (x != None):
                                    x.extract()
                            for x in s('em'):
                                if (x != None):
                                    x.extract()
                            for x in s('ul'):
                                if (x != None):
                                    x.extract()

                    print(tag_content)
                    text_sentence = text_sentence + self.parser.clearContent(str(tag_content[0].find_all(text=True)))
                    if not text_sentence: # 공백일 경우 기사 제외 처리
                        print("*** Error 03 ***")
                        continue
                    
                    # 요약문 가져옴
                    print("*** Summary crawling ***")
                    tag_summary = document_content.find_all('div', {'class': '_contents_body'})
                    text_summary = ''
                    text_summary = text_summary + self.summaryParser.clearContent(str(tag_summary[0].find_all(text=True)))
                    # print(text_summary)
                    if not text_summary:
                        print("*** Error 01 ***")
                        continue

                    # # 언론사 정보
                    # print("*** Company crawling ***")
                    # tag_company = document_content.find_all('meta', {'property': 'me2:category1'})
                    # text_company = '' # 언론사 초기화
                    # text_company = text_company + str(tag_company[0].get('content'))
                    # if not text_company: # 공백일 경우 기사 제외 처리
                    #     print("*** Error 04 ***")
                    #     continue

                    # CSV 작성
                    print("*** Writing ***")
                    wcsv.writerow([text_sentence, text_summary])
                    # wcsv.writerow([news_date, category_name, text_company, text_headline, text_sentence, text_summary, content_url])
                    print("### ", cnt, " ###")
                    cnt += 1

                except Exception as ex: # UnicodeEncodeError ..
                    print("*** Exception ***")
                    pass

            # break

        driver.close()
        file.close()

    def start(self):
        # Multiprocess 크롤링 시작
        for category_name in self.selected_categories:
            proc = Process(target=self.crawling, args=(category_name,))
            proc.start()
            proc.join()
            print("\n FINISH !")

if __name__ == "__main__":
    Crawler = ArticleCrawler()
    Crawler.setCategory("IT_science")
    Crawler.setDateRange(2018, 1, 2018, 12) # 2018년 1월부터 2019년 1월까지
    Crawler.start()
