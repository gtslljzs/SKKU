from konlpy.tag import Kkma
# 한글 문장을 형태소 분석기를 통해 분리. 내용을 csv파일에 저장
kkma = Kkma()
if __name__ == "__main__":
    print('Start!!!')
    # 실패 카운트
    cnt = 1
    # 데이터를 가져올 파일
    with open("step1_output.csv", 'r') as f:
        # 새로 만들 파일
        with open("step2_output.csv", 'w') as t:
            while True:
                # 한줄씩 뽑아 오기
                line = f.readline()
                # 더 이상 문장이 없으면 종료
                if not line: break
                try:
                    # 형태소 분석
                    kolist = kkma.pos(line)
                    kolen = len(kolist)
                    st = ''
                    # 형태소로 구성된 문장 생성. space로 각 형태소 분리, 마지막엔 \n 추가
                    for i in range(kolen):
                        st += kolist[i][0]
                        if i == kolen-1:
                            st+='\n'
                        else:
                            st+=' '
                    # print(st)
                    # 형태소를 각각 파일에 쓴다.
                    t.write(st)
                except:
                    print("실패 %d\n" % cnt)
                    cnt += 1
                    continue
    print('Finish!!!')