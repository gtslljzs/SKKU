from bs4 import BeautifulSoup as bs
import requests
import re

class ArticleParser(object):
    def __init__(self):
        # 특정 문자( [], (), <> 등 )를 제거하는 코드
        # self.pre_remove = re.compile("[\()\<>\[].*?[\]<\>(\)]")

        self.special_symbol = re.compile('[\{\}\[\]\/?,;:|*~`!^\-_+<>@\#$%&n▲▶◆◀■\'\"\\\【】]')
        self.content_pattern = re.compile('본문 내용|TV플레이어|동영상 뉴스|flash 오류를 우회하기 위한 함수 추가fuctio flashremoveCallback|tt|t|앵커 멘트|xa0|앵커|(사진)')

    def clearContent(self, text):
        # 특정 문자( [], (), <> 등 )를 제거하는 코드
        # pre_text = re.sub(self.pre_remove, '', text)
        special_symbol_removed_content = re.sub(self.special_symbol, '', text)

        # 기사 본문에서 필요없는 특수문자 및 본문 양식 등을 다 지움
        # special_symbol_removed_content = re.sub(self.special_symbol, '', text)
        end_phrase_removed_content = re.sub(self.content_pattern, '', special_symbol_removed_content)
        blank_removed_content = end_phrase_removed_content.strip().replace('   ', '') # 공백 에러 삭제
        reversed_content = ''.join(reversed(blank_removed_content)) # 기사 내용을 reverse
        content = ''

        for i in range(0, len(blank_removed_content)):
            # reverse 된 기사 내용 중, ".다"로 끝나는 경우 기사 내용이 끝난 것이기 때문에
            # 기사 내용이 끝난 후의 광고, 기자 등의 정보는 삭제
            if reversed_content[i:i + 2] == '.다':
                content = ''.join(reversed(reversed_content[i:]))
                break

        for i in range(0, len(content)):
            if content[i:i + 4] == '기자 =':
                content = content[i + 4:]
            elif content[i: i + 2] == '기자':
                content = content[i + 2:]
                break
        
        return content

    def clearHeadline(self, text):
        # 기사 제목에서 필요없는 특수문자들을 지움
        special_symbol_removed_headline = re.sub(self.special_symbol, '', text)

        return special_symbol_removed_headline
    
    def findNewsTotalpage(self, url):
        # 당일 기사 목록 전체 페이지 수를 알아냄
        try:
            totalpage_url = url
            request_content = requests.get(totalpage_url)
            document_content = bs(request_content.content, 'html.parser')
            headline_tag = document_content.find('div', {'class': 'paging'}).find('strong')
            regex = re.compile(r'<strong>(?P<num>\d+)')
            match = regex.findall(str(headline_tag))
            
            return int(match[0])
        except Exception:

            return 0