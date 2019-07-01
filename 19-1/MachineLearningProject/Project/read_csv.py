# -*- coding: utf-8 -*-
import romanizer as rm
import re

# csv파일을 읽고 한글을 romanize한 다음 새롭게 csv파일을 생성해서 저장하는 프로그램
# encoding 오류를 해결하기 위해 반드시 꼭대기의 이상한 주석을 붙여야 한다.

if __name__ == "__main__":

    # 데이터를 가져올 파일
    with open("./input.csv", 'r') as f:
        # 새로 만들 파일
        with open("./output.csv", 'w') as t:
            page = 0
            while True:
                page = page + 1
                try:
                    # 한줄씩 뽑아 오기
                    line = f.readline()

                    # 더 이상 문장이 없으면 종료
                    if not line: break

                    # 특수문자 제거 - 더 추가가 필요하면 추가 가능
                    refTxt = re.sub('[▷]','',line)

                    # Romanize 실행 및 저장
                    s1 = re.split(',', refTxt)
                    for i in range(len(s1)):
                        if i != len(s1) - 1:
                            s1[i]+=','
                        s2 = re.split('\.',s1[i])
                        for j in range(len(s2)):
                            if j != len(s2)-1:
                                s2[j]+='.'
                            try:
                                rom = rm.Romanizer(s2[j])
                                t.write(rom.romanize())
                            except:
                                continue
                except:
                    print("데이터. %d 실패\n" %page)
                    continue