from bs4 import BeautifulSoup as bs
import requests
import re

class SummaryParser(object):
    def __init__(self):
        self.special_symbol = re.compile('[\[\],\']')
        
    def clearContent(self, text):
        special_symbol_removed_content = re.sub(self.special_symbol, '', text)
        blank_removed_content = special_symbol_removed_content.strip().replace('   ', '') # 공백 에러 삭제
        content = blank_removed_content

        return content