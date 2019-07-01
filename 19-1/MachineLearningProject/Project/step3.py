# -*- coding: utf-8 -*-
from romanizer import Romanizer as rm
import collections
import pickle
import re

# 본 코드의 기능
# 로마자로 변경된 한글 형태소의 집합을 INPUT값으로 받음
# 해당 INPUT값을 통해 Dictionary 제작
# Dictionary를 Pickle파일로 저장
# 문장을 형태소로 분리 및 romanize한 문장 csv output 파일
if __name__ == "__main__":
    words = list()
    word_dict = dict()
    word_dict["<padding>"] = 0  # 빈칸 채우기
    word_dict["<unk>"] = 1      # 딕셔너리에 없는 단어
    word_dict["<s>"] = 2        # 시작 표시
    word_dict["</s>"] = 3       # 끝 표시
    print('Start!!!')
    # 데이터를 가져올 파일
    with open("step2_output.csv", 'r') as f:
        page = 0
        with open("step3_output.csv", 'w') as t:
            while True:
                page = page+ 1
                # 한줄씩 뽑아 오기
                line = f.readline()
                if not line: break

                temp_list = re.split('[ \n]+',line)
                temp_len = len(temp_list)

                article = ''
                # print("make list in line %d" %page)
                for i in range(temp_len):
                    try:
                        txt = rm(temp_list[i]).romanize()
                        # print("데이터 %d = " %page, txt)
                        words.append(txt)
                        article += txt
                        if(i == temp_len-1):
                            article+='\n'
                        else:
                            article+=' '
                    except:
                        print("데이터 %d 실패\n" % page)
                        continue
                t.write(article)
    # 자주나오는 단어 고르기 (2000개로 설정)
    word_counter = collections.Counter(words).most_common(2000)
    # 딕셔너리 만들기
    for word, _ in word_counter:
        word_dict[word] = len(word_dict)

    # 파일로 쓰기
    with open("step3_output.pickle", 'wb') as t:
        pickle.dump(word_dict,t)

    # 확인용 테스트 코드
    # print("<<< DICTIONARY >>>")
    # with open("step3_output.pickle", 'rb') as f:
    #     word_dict = pickle.load(f)
    #     print(word_dict)

    print('Finish!!!')

