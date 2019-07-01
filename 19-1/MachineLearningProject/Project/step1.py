# -*- coding: utf-8 -*-
import re
import collections
import numpy as np
import pandas

# 문장에 영어, 한글, 기타 문단 부호만 남기고 모두 없애는 과정

if __name__ == "__main__":
    print('Start!!!')
    # 실패 카운트
    cnt = 1
    # 데이터를 가져올 파일
    #with open("input.csv", 'r') as f:
    with open("train_articles.csv", 'r') as f:
    # with pandas.read_csv('Articles_train.csv', names=['article']) as f:
        # 새로 만들 파일
        print('?')
        with open("step1_output.csv", 'w') as t:
            while True:
                try:
                    # 한줄씩 뽑아 오기
                    line = f.readline()
                    # 더 이상 문장이 없으면 종료
                    if not line:
                        print('??')
                        break
                    # 특수문자 제거 - 더 추가가 필요하면 추가 가능
                    refTxt = re.sub('[^가-힣a-zA-Z0-9 ,\.\'\"?!\n·]','',line)
                    t.write(refTxt)
                except:
                    print("실패 %d\n" %cnt)
                    cnt+=1
                    continue
    print('Finish!!!')